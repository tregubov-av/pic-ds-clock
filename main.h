#ifndef MAIN_H
#define	MAIN_H

//Частота опорного генератора в Гц (4мГц). Используется при расчете пауз.
#define _XTAL_FREQ 4000000

//Значение для инициализации TMR0
#define TMR0INI 100

//Значение счетчика опроса DS1307 
// в течении которого устройство не опрашивается.
#define HCOUNT 200 

//Значение глобального счетчика опроса клавиатуры 
// в течении которого клавиатура не опрашивается
// внезависимости от состояния локальных счетчиков.
#define KCOUNT 20

static unsigned char hcount = 0;    //Счетчик опроса DS1307.
static unsigned char kcount = 0;    //Счетчик опроса клавиатуры.

#endif	/* MAIN_H */
