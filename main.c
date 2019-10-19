#include <xc.h>
#include "main.h"
#include "led_display.h"
#include "clock_ds1307.h"
#include "key_flags.h"
#include "setup_pic16f628a.h"

static unsigned char hcount  = 0;   //Счетчик опроса DS1307.
static unsigned char kcount = 0;    //Счетчик опроса клавиатуры.

void main(void) {
    setup_pic();
    clock_init();
    TMR0+=100;
    ei();
    while (1) {
        if(!hcount){ds_read(); hcount=200;}
        do{
            if(!kcount){key_manager(); kcount=20;}
        }while((!SELECT)&&(BITTST1(cflags,F2)));
    }
}

void interrupt isr(void){
    static unsigned char index = 0;         //Счетчик переключения разрядов.
    T0IF=0;                                 //Сбросить флаг прерывания.
    SIND = 0b11111110;                      //Отключение всех сегментов.
    RIND = arr_ind[index];                  //Выбор активного разряда.
    SIND = arr_seg[ledx_arr[index]];        //Отрисовка значения в активном разряде.
    ++index;                                //Переключить на следующий разряд.
    if(index > inmax){index = inmin;}       //Ограничить преращение index(выбор активных разрядов).
    --hcount;                               //Декрименировать значение счетчика опроса.
    --kcount;
    TMR0+=100;                              //Инициализировать таймер.
}