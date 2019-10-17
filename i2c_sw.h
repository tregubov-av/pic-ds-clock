#ifndef I2C_SW_H
#define	I2C_SW_H

#define SCL  RA2        //Порт Clock I2C line.
#define SDA  RA3        //Порт Data  I2C line.
#define TSCL TRISA2     //Направление порта Clock I2C line.
#define TSDA TRISA3     //Направление порта Data  I2C line.

volatile unsigned char i2cdev;   //Адрес I2C устройства
volatile unsigned char i2caddr;  //Адрес I2C регистра
volatile unsigned char i2cdatao; //Переменная для хранения пересылаемых данных
volatile unsigned char i2cdatai; //Переменная для хранения принимаемых данных
volatile unsigned char i2cflag;  //Флаг подтверждения (ACK)

void i2c_wr_byte(void); //Запрос на запись в устройство
void i2c_rd_byte(void); //Запрос на чтение из устройства
#endif	/* I2C_SW_H */

