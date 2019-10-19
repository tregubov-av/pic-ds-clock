#ifndef CLOCK_2KEY_H
#define	CLOCK_2KEY_H

#define BITSET(var,bitno) ((var) |= 1 << (bitno))       //Установить бит № bitno в переменной var.
#define BITCLR(var,bitno) ((var) &= ~(1 << (bitno)))    //Сбросить бит № bitno в переменной var.
#define BITTST1(var,bitno) ((var) & (1 << (bitno)))     //Проверить бит № bitno в переменной var (TRUE=1).
#define BITTST0(var,bitno) (!((var) & (1 << (bitno))))  //Проверить бит № bitno в переменной var (TRUE=0).
#define BITINV(var,bitno) var=(var^(1 << (bitno)))      //Инвертировать бит № bitno в переменной var.

#define SELECT RA4  //Кнопка переключения между отображениями время/дата/год. В режиме SET управляет выбранным значением.
#define SET    RB0  //Кнопка перехода в режим SET / выбор значения для изменения.

#define F0 0x00 //Флаг выбора время/дата. Если 0 отображать время, если 1 дату.
#define F1 0x01 //Флаг выбора год/(время/дата). Если 0 отображать время или дату, если 1 год.
#define F2 0x02 //Флаг выбора режима SET. Если 0 нормальный режим если 1 режим установки
#define F3 0x03 //Флаг выбора для режима SET. Если 0 установка минут, если 1 часа.
#define F4 0x04 //Флаг выбора для режима SET. Если 1 установить месяц, если 0 число.

unsigned char cflags = 0;   //Переменная под флаги. По умолчанию все флаги сброшены.

unsigned char s1=0;         //Счетчик для кнопки SELECT.
unsigned char s2=0;         //Счетчик для кнопки SET.
unsigned char s3=0;         //Счетчик для второго состояния кнопки SELECT.

void key_manager(void);     //Функция отслеживания нажатия кнопок

#endif	/* CLOCK_2KEY_H */