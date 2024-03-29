#include <Vector.h>
#include <Wire.h> //IIC库
#define _baudrate 115200
#include <math.h>
#include "AT_HTTP.h"

int BH1750address = 0x23; //芯片地址为16位23
#define BMP180ADD 0x77    // I2C address of BMP180

unsigned char OSS;
/**********************MSB      LSB******/
int ac1;          // 0xAA     0xAB
int ac2;          // 0xAC     0xAD
int ac3;          // 0xAE     0xAE
unsigned int ac4; // 0xB0     0xB1
unsigned int ac5; // 0xB2     0xB3
unsigned int ac6; // 0xB4     0xB5
int b1;           // 0xB6     0xB7
int b2;           // 0xB8     0xB9
int mb;           // 0xBA     0xBB
int mc;           // 0xBC     0xBD
int md;           // 0xBE     0xBF
float temperature;
double pressure;
double pressure2;
long b5;
double altitude;

byte buff[2];
#define SSID "N6-506-ARM"
#define PASS "armn6-506qwg"
#define HOST "10.18.52.137"
#define PORT 9997

void setup()
{
    Wire.begin();
    
    Serial.begin(_baudrate);
    OSS = 2; // Oversampling Setting           0: single    1: 2 times    2: 4 times   3: 8 times
    BMP180start();         //启动BMP180
    init_AT();
    connect_WiFi(SSID, PASS);
    
    
    delay(700);
}

void loop()
{
    calculate();   //循环 calculate
    show();        //定义下面定义的show函数，循环
    delay(5000); //循环发送延迟
}

int BH1750_Read(int address)
{
    int i = 0;
    Wire.beginTransmission(address);
    Wire.requestFrom(address, 2);
    while (Wire.available())
    {
        buff[i] = Wire.read(); // read one byte
        i++;
    }
    Wire.endTransmission();
    return i;
}
void BH1750_Init(int address)
{
    Wire.beginTransmission(address);
    Wire.write(0x10); //1lx reolution 120ms
    Wire.endTransmission();
} //BH1750光强数据的接收

/** calculate centure **/
void calculate()
{
    temperature = bmp180GetTemperature(bmp180ReadUT());
    temperature = temperature * 0.1;
    pressure = bmp180GetPressure(bmp180ReadUP());
    pressure2 = pressure / 101325;
    pressure2 = pow(pressure2, 0.19029496);
    altitude = 44330 * (1 - pressure2); //altitude = 44330*(1-(pressure/101325)^0.19029496);
}

/** print reslut **/

/**BMP180 satrt program**/
void BMP180start()
{                               /*MSB*/
    ac1 = bmp180ReadDate(0xAA); //get full data
    ac2 = bmp180ReadDate(0xAC);
    ac3 = bmp180ReadDate(0xAE);
    ac4 = bmp180ReadDate(0xB0);
    ac5 = bmp180ReadDate(0xB2);
    ac6 = bmp180ReadDate(0xB4);
    b1 = bmp180ReadDate(0xB6);
    b2 = bmp180ReadDate(0xB8);
    mb = bmp180ReadDate(0xBA);
    mc = bmp180ReadDate(0xBC);
    md = bmp180ReadDate(0xBE);
}

/***BMP180 temperature Calculate***/
short bmp180GetTemperature(unsigned int ut)
{
    long x1, x2;
    x1 = (((long)ut - (long)ac6) * (long)ac5) >> 15; //x1=((ut-ac6)*ac5)/(2^15)
    x2 = ((long)mc << 11) / (x1 + md);               //x2=(mc*2^11)/(x1+md)
    b5 = x1 + x2;                                    //b5=x1+x2
    return ((b5 + 8) >> 4);                          //t=(b5+8)/(2^4)
}

/***BMP180 pressure Calculate***/

long bmp180GetPressure(unsigned long up)
{
    long x1, x2, x3, b3, b6, p;
    unsigned long b4, b7;
    b6 = b5 - 4000;
    x1 = (b2 * (b6 * b6) >> 12) >> 11;
    x2 = (ac2 * b6) >> 11;
    x3 = x1 + x2;
    b3 = (((((long)ac1) * 4 + x3) << OSS) + 2) >> 2;

    x1 = (ac3 * b6) >> 13;
    x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (ac4 * (unsigned long)(x3 + 32768)) >> 15;

    b7 = ((unsigned long)(up - b3) * (50000 >> OSS));
    if (b7 < 0x80000000)
        p = (b7 << 1) / b4;
    else
        p = (b7 / b4) << 1;

    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p += (x1 + x2 + 3791) >> 4;

    return p;
}

/*** Read 1 bytes from the BMP180  ***/

int bmp180Read(unsigned char address)
{
    unsigned char data;
    Wire.beginTransmission(BMP180ADD);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(BMP180ADD, 1);
    while (!Wire.available())
        ;
    return Wire.read();
}

/*** Read 2 bytes from the BMP180 ***/
int bmp180ReadDate(unsigned char address)
{
    unsigned char msb, lsb;
    Wire.beginTransmission(BMP180ADD);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(BMP180ADD, 2);
    while (Wire.available() < 2)
        ;
    msb = Wire.read();
    lsb = Wire.read();
    return (int)msb << 8 | lsb;
}

/*** read uncompensated temperature value ***/
unsigned int bmp180ReadUT()
{
    unsigned int ut;
    Wire.beginTransmission(BMP180ADD);
    Wire.write(0xF4); // Write 0x2E into Register 0xF4
    Wire.write(0x2E); // This requests a temperature reading
    Wire.endTransmission();
    delay(5);                  // Wait at least 4.5ms
    ut = bmp180ReadDate(0xF6); // read MSB from 0xF6 read LSB from (16 bit)
    return ut;
}

/*** Read uncompensated pressure value from BMP180 ***/
unsigned long bmp180ReadUP()
{
    unsigned char msb, lsb, xlsb;
    unsigned long up = 0;

    Wire.beginTransmission(BMP180ADD);
    Wire.write(0xF4);              // Write 0x34+(OSS<<6) into register 0xF4
    Wire.write(0x34 + (OSS << 6)); // 0x34+oss*64
    Wire.endTransmission();
    delay(2 + (3 << OSS)); // Wait for conversion, delay time dependent on OSS

    Wire.beginTransmission(BMP180ADD);
    Wire.write(0xF6); // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
    Wire.endTransmission();

    Wire.requestFrom(BMP180ADD, 3);
    while (Wire.available() < 3)
        ; // Wait for data to become available
    msb = Wire.read();
    lsb = Wire.read();
    xlsb = Wire.read();
    up = (((unsigned long)msb << 16) | ((unsigned long)lsb << 8) | (unsigned long)xlsb) >> (8 - OSS); //16 to 19 bit
    return up;
}


void show()
{
    String data, Lig, temp, pre, alt;
    uint16_t val = 0;
    BH1750_Init(BH1750address);
    BH1750_Read(BH1750address);
    val = ((buff[0] << 8) | buff[1]) / 1.2;        
    Lig = val;
    temp = temperature;
    pre = pressure / 100;
    alt = altitude;
    delay(1000);     

    
    String data_array[4] = {Lig, temp, pre, alt};
    Vector<String> data_vector(data_array, 4);
    
    int project_array[4] = {4, 5, 6, 7};
    Vector<int> project_vector(project_array, 4);
    
    data = generate_data_3(HOST, String(4), project_vector, data_vector, "0");
    connect_TCP(HOST, String(PORT));
    send_data(data);  
    close_TCP();

    
}



//10.18.52.132
//192.168.43.249 热点电脑
//192.168.3.111 电脑506
