#include <xc.h>
#include "main.h"
#include "setup_pic16f628a.h"
#include "led_display.h"
#include "clock_ds1307.h"
#include "key_flags.h"

static unsigned char count = 0;     //Счетчик опроса DS1307.

void main(void) {
    setup_pic();        //Конфигурирование регистров специального назначения.
    clock_init();       //Инициализация DS1307.
    TMR0+=100;          //Инициализировать таймер.
    ei();               //Глобально разрешить прерывания.
    while(1){           //Начало главного цикла.
        if(!count){     //Счетчик опроса DS1307.
            ds_read();  //Прочитать регистры DS1307.
            key_manager();
            //count=200;  //Задать время в течении которого опрос производиться не будет.
            count=100;  //Задать время в течении которого опрос производиться не будет.
        }
    }
}

void interrupt isr(void){               //Вход в прерывание.
    static unsigned char index = 0;     //Счетчик разрядов.
    T0IF=0;                             //Сбросить флаг прерывания.
    SIND = 0b11111110;                  //Отключение всех сегментов.
    RIND = arr_ind[index];              //Выбор активного разряда.
    SIND = arr_seg[ledx_arr[index]];    //Отрисовка значения в активном разряде.
    ++index;                            //Переключить на следующий разряд.
    if((BITTST1(cflags,F2))&&(BITTST0(cflags,F1))){
        if((BITTST0(cflags,F0))&&(BITTST0(cflags,F3))||
           (BITTST1(cflags,F0))&&(BITTST0(cflags,F4))){
            if(index<2||index>3){index=2;}
        }
        else{
            if(index>1){index=0;}
        }
    }
    else{
        index&=0x03;                   //Ограничить преращение index от 0 до 3.
    }
    --count;                            //Декрименировать значение счетчика.
    TMR0+=100;                          //Инициализировать таймер.
    ei();                               //Глобально разрешить прерывания.
}