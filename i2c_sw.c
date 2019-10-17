#include <xc.h>
#include "i2c_sw.h"

/*************************Управление линиями***********************************/
//------------------Установка "0" на линии CLOCK--------------------------------
void i2c_clock_null (void){
    TSCL = 0;   //Направление работы пина SCL вых.
    SCL  = 0;   //Установить пин SCL в 0
}
//-------------------Установка "1" на линии CLOCK-------------------------------
void i2c_clock_one (void){
    TSCL = 1;   //Направление работы пина SCL вх.
}
//---------------------Установка "0" на линии DATA------------------------------
void i2c_data_null (void){
    TSDA = 0;   //Направление работы пина SDA вых.
    SDA  = 0;   //Установить пин SDA в 0
}
//----------------------Установка "1" на линии DATA-----------------------------
void i2c_data_one (void){
    TSDA = 1;   //Направление работы пина SDA вх.
}
/******************************************************************************/
/**********************Формирование Старт-Стоп битов***************************/
//------------------------Формирование Старт бита-------------------------------
void i2c_bstart (void){
    if (!SDA){SDA = 1;} //Если SDA свободна (SDA=0), установить SDA в 1 (SDA=1)
    i2c_clock_one();    //Установить 1 на линии SCL
    i2c_data_null();    //Установить 0 на линии SDA
    i2c_clock_null();   //Установить 0 на линии SCL
}
//------------------------Формирование Стоп бита--------------------------------
void i2c_bstop (void){
    i2c_data_null();    //Установить 0 на линии SDA
    i2c_clock_one();    //Установить 1 на линии SCL
    while (!SCL);       //Продолжить если SCL не 0 (SCL != 0)
    i2c_data_one();     //Установить 1 на линии SDA
}
/******************************************************************************/
/***************************Прием/пересылка байта******************************/
//-----------------------------Пересылка байта----------------------------------
void i2c_tx(unsigned char txbuf){       //Передача осуществляется младшим битом вперед
    unsigned char count;                //Переменная для счетчика циклов
    i2cflag=0;                          //Сбросить i2cflag в 0
    for (count=8; count>0; --count){    //Выполнить цикл 8 раз
        if(txbuf&1<<7){i2c_data_one();} //Если 7ой бит переменной txbuf равен 1, установить 1 на линии SDA
        else {i2c_data_null();}         //Если 7ой бит переменной txbuf не равен 1, установить 0 на линии SDA
        i2c_clock_one();                //Установить 1 на линии SCL
        while(!SCL);                    //Продолжить если SCL не 0 (SCL != 0)
        i2c_clock_null();               //Установить 0 на линии SCL
        txbuf<<=1;                      //Сдвинуть влево биты переменной txbuf
    }
    i2c_data_one();                     //Установить 1 на линии SDA
    i2c_clock_one();                    //Установить 1 на линии SCL
    while(!SCL);                        //Продолжить если SCL не 0 (SCL != 0)
    if(SDA){i2cflag=1;}                 //Если SDA=1 установить i2cflag в 1
    i2c_clock_null();                   //Установить 0 на линии SCL
}
//--------------------------------Приём байта-----------------------------------
void i2c_rx(unsigned char ack){     //Прием осуществляется младшим битом вперед
    unsigned char count;            //Переменная для счетчика циклов
    i2cdatai=0;                     //Очистить переменную
    for (count=8; count>0; --count){//Выполнить цикл 8 раз
        i2cdatai<<=1;               //Сдвинуть влево биты переменной (установить 0ой бит в 0)
        i2c_data_one();             //Установить 1 на линии SDA
        i2c_clock_one();            //Установить 1 на линии SCL
        while(!SCL);                //Продолжить если SCL не 0 (SCL != 0)
        if(SDA)i2cdatai|=1;         //Если SDA=1 установить 0ой бит переменной в 1
        i2c_clock_null();           //Установить 0 на линии SCL
    }
    if(ack){i2c_data_one();}        //Если ack=1, то установить 1 на линии SDA
    else{i2c_data_null();}          //Если ack=0, то установить 0 на линии SDA
    i2c_clock_one();                //Установить 1 на линии SCL
    while(!SCL);                    //Продолжить если SCL не 0 (SCL != 0)
    i2c_clock_null();               //Установить 0 на линии SCL
}
/******************************************************************************/
/******************************Формирование запросов***************************/
//-------------------------Запрос на запись в устройство------------------------
void i2c_wr_byte(void){
    i2c_bstart();       //Отправить Старт-Бит
    i2c_tx(i2cdev);     //Отправить адрес устройства по шине I2C
    i2c_tx(i2caddr);    //Отправить адрес регистра по шине I2C
    i2c_tx(i2cdatao);   //Отправить данные по шине I2C
    i2c_bstop();        //Отправить Стоп-Бит
}
//-------------------------Запрос на чтение из устройства-----------------------
void i2c_rd_byte(void){
    i2c_bstart();       //Отправить Старт-Бит
    i2c_tx(i2cdev);     //Отправить адрес устройства по шине I2C
    i2c_tx(i2caddr);    //Отправить адрес регистра по шине I2C
    i2c_bstart();       //Отправить Старт-Бит
    i2cdev|=1;          //Установить режим чтения READ (R/W = 1)
    i2c_tx(i2cdev);     //Отправить адрес устройства по шине I2C
    i2c_rx(1);          //Выполнить чтение указанного регистра (Подтверждение 0-выдавать 1-нет).
    i2cdev&=~1;         //Установить режим записи WRITE (R/W = 0)
    i2c_bstop();        //Отправить Стоп-Бит
}