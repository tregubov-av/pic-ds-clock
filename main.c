#include <xc.h>
#include "main.h"
#include "led_display.h"
#include "clock_ds1307.h"
#include "key_flags.h"
#include "setup_pic16f628a.h"

void main(void) {
    setup_pic();                                //Конфигурирование регистров специального назначения.
    clock_init();                               //Инициализация DS1307.
    TMR0+=TMR0INI;                              //Инициализировать таймер.
    ei();                                       //Глобально разрешить прерывания.
    while (1) {                                 //Начало главного цикла.
        if(!hcount){ds_read(); hcount=HCOUNT;}
        do{
            if(!kcount){key_manager(); kcount=KCOUNT;}
        }while((!SELECT)&&(BITTST1(cflags,F2)));
    }
}

void interrupt isr(void){
    static unsigned char index = 0;         //Счетчик переключения разрядов.
    T0IF=0;                                 //Сбросить флаг прерывания.
    SIND = SINDOFF;                         //Отключение всех сегментов.
    RIND = arr_ind[index];                  //Выбор активного разряда.
    SIND = arr_seg[ledx_arr[index]];        //Отрисовка значения в активном разряде.
    ++index;                                //Переключить на следующий разряд.
    if(index > inmax){index = inmin;}       //Ограничить преращение index(выбор активных разрядов).
    --hcount;                               //Декрименировать значение счетчика опроса.
    --kcount;
    TMR0+=TMR0INI;                          //Инициализировать таймер.
}
