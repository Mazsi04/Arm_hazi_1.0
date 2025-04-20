/*
 * cc1101.c
 *
 *  Created on: Mar 31, 2025
 *      Author: mazsi
 */
//SPI1_NSS SPI1_MISO SPI1_MOSI SPI1_SCK

#include <main.h>
#include "cc1101.h"



   // PA4     ------> SPI1_NSS
   // PA5     ------> SPI1_SCK
   // PA6     ------> SPI1_MISO
   // PA7     ------> SPI1_MOSI
#define GDO0 GDO0_Pin
#define GDO0_PORT GPIOG
#define SPI1_NSS GPIO_PIN_8
#define SPI1_SCK GPIO_PIN_10
#define SPI1_MISO GPIO_PIN_11
#define SPI1_MOSI GPIO_PIN_12
#define GPIO GPIOC
extern SPI_HandleTypeDef hspi3;
#define hspi &hspi3

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define   WRITE_BURST       0x40            //write burst
#define   READ_SINGLE       0x80            //read single
#define   READ_BURST        0xC0            //read burst
#define   BYTES_IN_RXFIFO   0x7F            //byte number in RXfifo
#define   max_modul 6

uint8_t modulation = 2;
uint8_t frend0;
uint8_t chan = 0;
int pa = 12;
uint8_t last_pa;
uint8_t SCK_PIN_M[max_modul];
uint8_t MISO_PIN_M[max_modul];
uint8_t MOSI_PIN_M[max_modul];
uint8_t SS_PIN_M[max_modul];
uint8_t GDO0_M[max_modul];
uint8_t GDO2_M[max_modul];
uint8_t gdo_set=0;
bool spi = 0;
bool ccmode = 0;
float MHz = 433.92;
uint8_t m4RxBw = 0;
uint8_t m4DaRa;
uint8_t m2DCOFF;
uint8_t m2MODFM;
uint8_t m2MANCH;
uint8_t m2SYNCM;
uint8_t m1FEC;
uint8_t m1PRE;
uint8_t m1CHSP;
uint8_t pc1PQT;
uint8_t pc1CRC_AF;
uint8_t pc1APP_ST;
uint8_t pc1ADRCHK;
uint8_t pc0WDATA;
uint8_t pc0PktForm;
uint8_t pc0CRC_EN;
uint8_t pc0LenConf;
uint8_t trxstate = 0;
uint8_t clb1[2]= {24,28};
uint8_t clb2[2]= {31,38};
uint8_t clb3[2]= {65,76};
uint8_t clb4[2]= {77,79};

/****************************************************************/
uint8_t PA_TABLE[8] =    {0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00};
//                       -30  -20  -15  -10   0    5    7    10
uint8_t PA_TABLE_315[8] = {0x12,0x0D,0x1C,0x34,0x51,0x85,0xCB,0xC2,};             //300 - 348
uint8_t PA_TABLE_433[8] = {0x12,0x0E,0x1D,0x34,0x60,0x84,0xC8,0xC0,};             //387 - 464
//                        -30  -20  -15  -10  -6    0    5    7    10   12
uint8_t PA_TABLE_868[10] = {0x03,0x17,0x1D,0x26,0x37,0x50,0x86,0xCD,0xC5,0xC0,};  //779 - 899.99
//                        -30  -20  -15  -10  -6    0    5    7    10   11
uint8_t PA_TABLE_915[10] = {0x03,0x0E,0x1E,0x27,0x38,0x8E,0x84,0xCC,0xC3,0xC0,};  //900 - 928


int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t spi_transfer(uint8_t data) {
    uint8_t receivedData;
    HAL_SPI_TransmitReceive(&hspi3, &data, &receivedData, 1, HAL_MAX_DELAY);
    return receivedData;
}


void setMHZ(float mhz){
	uint8_t freq2 = 0;
	uint8_t freq1 = 0;
	uint8_t freq0 = 0;

	MHz = mhz;

	for (bool i = 0; i==0;){
	if (mhz >= 26){
	mhz-=26;
	freq2+=1;
	}
	else if (mhz >= 0.1015625){
	mhz-=0.1015625;
	freq1+=1;
	}
	else if (mhz >= 0.00039675){
	mhz-=0.00039675;
	freq0+=1;
	}
	else{i=1;}
	}
	if (freq0 > 255){freq1+=1;freq0-=256;}

	SpiWriteReg(CC1101_FREQ2, freq2);
	SpiWriteReg(CC1101_FREQ1, freq1);
	SpiWriteReg(CC1101_FREQ0, freq0);

	Calibrate();
}
//ezt még meg kell csinálni a SpiWriteReg és ki kell taláni map() függvényt
void Calibrate(void){

	if (MHz >= 300 && MHz <= 348){
	SpiWriteReg(CC1101_FSCTRL0, map(MHz, 300, 348, clb1[0], clb1[1]));
	if (MHz < 322.88){SpiWriteReg(CC1101_TEST0,0x0B);}
	else{
	SpiWriteReg(CC1101_TEST0,0x09);
	int s = SpiReadStatus(CC1101_FSCAL2);
	if (s<32){SpiWriteReg(CC1101_FSCAL2, s+32);}
	if (last_pa != 1){setPA(pa);}
	}
	}
	else if (MHz >= 378 && MHz <= 464){
	SpiWriteReg(CC1101_FSCTRL0, map(MHz, 378, 464, clb2[0], clb2[1]));
	if (MHz < 430.5){SpiWriteReg(CC1101_TEST0,0x0B);}
	else{
	SpiWriteReg(CC1101_TEST0,0x09);
	int s = SpiReadStatus(CC1101_FSCAL2);
	if (s<32){SpiWriteReg(CC1101_FSCAL2, s+32);}
	if (last_pa != 2){setPA(pa);}
	}
	}
	else if (MHz >= 779 && MHz <= 899.99){
	SpiWriteReg(CC1101_FSCTRL0, map(MHz, 779, 899, clb3[0], clb3[1]));
	if (MHz < 861){SpiWriteReg(CC1101_TEST0,0x0B);}
	else{
	SpiWriteReg(CC1101_TEST0,0x09);
	int s = SpiReadStatus(CC1101_FSCAL2);
	if (s<32){SpiWriteReg(CC1101_FSCAL2, s+32);}
	if (last_pa != 3){setPA(pa);}
	}
	}
	else if (MHz >= 900 && MHz <= 928){
	SpiWriteReg(CC1101_FSCTRL0, map(MHz, 900, 928, clb4[0], clb4[1]));
	SpiWriteReg(CC1101_TEST0,0x09);
	int s = SpiReadStatus(CC1101_FSCAL2);
	if (s<32){SpiWriteReg(CC1101_FSCAL2, s+32);}
	if (last_pa != 4){setPA(pa);}
	}
}


uint8_t SpiReadStatus(uint8_t addr)
{
  uint8_t value,temp;
  //SpiStart();
  temp = addr | READ_BURST;
  //digitalWrite(SS_PIN, LOW);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
  //while(digitalRead(MISO_PIN));
  while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
  //SPI.transfer(temp);
  spi_transfer(temp);
  //value=SPI.transfer(0);
  value = spi_transfer( 0 );
  //digitalWrite(SS_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
  //SpiEnd();
  return value;
}


void RegConfigSettings(void)
{
    SpiWriteReg(CC1101_FSCTRL1,  0x06);

    setCCMode(ccmode);
    setMHZ(MHz);

    SpiWriteReg(CC1101_MDMCFG1,  0x02);
    SpiWriteReg(CC1101_MDMCFG0,  0xF8);
    SpiWriteReg(CC1101_CHANNR,   chan);
    SpiWriteReg(CC1101_DEVIATN,  0x47);
    SpiWriteReg(CC1101_FREND1,   0x56);
    SpiWriteReg(CC1101_MCSM0 ,   0x18);
    SpiWriteReg(CC1101_FOCCFG,   0x16);
    SpiWriteReg(CC1101_BSCFG,    0x1C);
    SpiWriteReg(CC1101_AGCCTRL2, 0xC7);
    SpiWriteReg(CC1101_AGCCTRL1, 0x00);
    SpiWriteReg(CC1101_AGCCTRL0, 0xB2);
    SpiWriteReg(CC1101_FSCAL3,   0xE9);
    SpiWriteReg(CC1101_FSCAL2,   0x2A);
    SpiWriteReg(CC1101_FSCAL1,   0x00);
    SpiWriteReg(CC1101_FSCAL0,   0x1F);
    SpiWriteReg(CC1101_FSTEST,   0x59);
    SpiWriteReg(CC1101_TEST2,    0x81);
    SpiWriteReg(CC1101_TEST1,    0x35);
    SpiWriteReg(CC1101_TEST0,    0x09);
    SpiWriteReg(CC1101_PKTCTRL1, 0x04);
    SpiWriteReg(CC1101_ADDR,     0x00);
    SpiWriteReg(CC1101_PKTLEN,   0x00);
}

//void SpiWriteReg(byte addr, byte value)
void SpiWriteReg(uint8_t addr, uint8_t value)
{
  //SpiStart();
  //digitalWrite(SS_PIN, LOW);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
  //while(digitalRead(MISO_PIN));
  while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
  //SPI.transfer(addr);
  spi_transfer( addr );
  //SPI.transfer(value);
  spi_transfer(value);
  //digitalWrite(SS_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
  //SpiEnd();
}


void Reset (void)
{
	//digitalWrite(SS_PIN, LOW);
	HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
	//delay(1);
	HAL_Delay(1);
	//ddigitalWrite(SS_PIN, HIGH);
	HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
	//delay(1);
	HAL_Delay(1);
	//digitalWrite(SS_PIN, LOW);
	HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
	//while(digitalRead(MISO_PIN));
	//HAL_StatusTypeDef
	while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
	//SPI.transfer(CC1101_SRES);
	spi_transfer(CC1101_SRES);
	//while(digitalRead(MISO_PIN));
	while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
	//digitalWrite(SS_PIN, HIGH);
	HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
}

void Init(void)
{
  //setSpi();
  //SpiStart();                   //spi initialization
  //digitalWrite(SS_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
  //digitalWrite(SCK_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_SCK,SET);
  //digitalWrite(MOSI_PIN, LOW);
  HAL_GPIO_WritePin(GPIO,SPI1_MOSI,RESET);
  Reset();                    //CC1101 reset
  RegConfigSettings(); //még nincs megírva           //CC1101 register config
  //SpiEnd();
}

void Split_PKTCTRL1(void){
int calc = SpiReadStatus(7);
pc1PQT = 0;
pc1CRC_AF = 0;
pc1APP_ST = 0;
pc1ADRCHK = 0;
for (bool i = 0; i == 0;){
	if (calc >= 32){
		calc-=32;
		pc1PQT+=32;
	}
	else
		if (calc >= 8){
			calc-=8;
			pc1CRC_AF+=8;
		}
		else
			if (calc >= 4){
					calc-=4;
					pc1APP_ST+=4;
			}
			else {
				pc1ADRCHK = calc;
				i=1;
			}
	}
}


void Split_PKTCTRL0(void){
int calc = SpiReadStatus(8);
pc0WDATA = 0;
pc0PktForm = 0;
pc0CRC_EN = 0;
pc0LenConf = 0;
for (bool i = 0; i==0;){
if (calc >= 64){calc-=64; pc0WDATA+=64;}
else if (calc >= 16){calc-=16; pc0PktForm+=16;}
else if (calc >= 4){calc-=4; pc0CRC_EN+=4;}
else {pc0LenConf = calc; i=1;}
}
}


void Split_MDMCFG1(void){
int calc = SpiReadStatus(19);
m1FEC = 0;
m1PRE = 0;
m1CHSP = 0;
int s2 = 0;
for (bool i = 0; i==0;){
if (calc >= 128){calc-=128; m1FEC+=128;}
else if (calc >= 16){calc-=16; m1PRE+=16;}
else {m1CHSP = calc; i=1;}
}
}


void Split_MDMCFG2(void){
int calc = SpiReadStatus(18);
m2DCOFF = 0;
m2MODFM = 0;
m2MANCH = 0;
m2SYNCM = 0;
for (bool i = 0; i==0;){
if (calc >= 128){calc-=128; m2DCOFF+=128;}
else if (calc >= 16){calc-=16; m2MODFM+=16;}
else if (calc >= 8){calc-=8; m2MANCH+=8;}
else{m2SYNCM = calc; i=1;}
}
}


void Split_MDMCFG4(void){
int calc = SpiReadStatus(16);
m4RxBw = 0;
m4DaRa = 0;
for (bool i = 0; i==0;){
if (calc >= 64){calc-=64; m4RxBw+=64;}
else if (calc >= 16){calc -= 16; m4RxBw+=16;}
else{m4DaRa = calc; i=1;}
}
}

void setCCMode(bool s){
ccmode = s;
if (ccmode == 1){
SpiWriteReg(CC1101_IOCFG2,      0x0B);
SpiWriteReg(CC1101_IOCFG0,      0x06);
SpiWriteReg(CC1101_PKTCTRL0,    0x05);
SpiWriteReg(CC1101_MDMCFG3,     0xF8);
SpiWriteReg(CC1101_MDMCFG4,11+m4RxBw);
}else{
SpiWriteReg(CC1101_IOCFG2,      0x0D);
SpiWriteReg(CC1101_IOCFG0,      0x0D);
SpiWriteReg(CC1101_PKTCTRL0,    0x32);
SpiWriteReg(CC1101_MDMCFG3,     0x93);
SpiWriteReg(CC1101_MDMCFG4, 7+m4RxBw);
}
setModulation(modulation);
}

void setModulation(uint8_t m){
if (m>4){m=4;}
modulation = m;
Split_MDMCFG2();
switch (m)
{
case 0: m2MODFM=0x00; frend0=0x10; break; // 2-FSK
case 1: m2MODFM=0x10; frend0=0x10; break; // GFSK
case 2: m2MODFM=0x30; frend0=0x11; break; // ASK
case 3: m2MODFM=0x40; frend0=0x10; break; // 4-FSK
case 4: m2MODFM=0x70; frend0=0x10; break; // MSK
}
SpiWriteReg(CC1101_MDMCFG2, m2DCOFF+m2MODFM+m2MANCH+m2SYNCM);
SpiWriteReg(CC1101_FREND0,   frend0);
setPA(pa);
}


void setPA(int p)
{
int a;
pa = p;

if (MHz >= 300 && MHz <= 348){
if (pa <= -30){a = PA_TABLE_315[0];}
else if (pa > -30 && pa <= -20){a = PA_TABLE_315[1];}
else if (pa > -20 && pa <= -15){a = PA_TABLE_315[2];}
else if (pa > -15 && pa <= -10){a = PA_TABLE_315[3];}
else if (pa > -10 && pa <= 0){a = PA_TABLE_315[4];}
else if (pa > 0 && pa <= 5){a = PA_TABLE_315[5];}
else if (pa > 5 && pa <= 7){a = PA_TABLE_315[6];}
else if (pa > 7){a = PA_TABLE_315[7];}
last_pa = 1;
}
else if (MHz >= 378 && MHz <= 464){
if (pa <= -30){a = PA_TABLE_433[0];}
else if (pa > -30 && pa <= -20){a = PA_TABLE_433[1];}
else if (pa > -20 && pa <= -15){a = PA_TABLE_433[2];}
else if (pa > -15 && pa <= -10){a = PA_TABLE_433[3];}
else if (pa > -10 && pa <= 0){a = PA_TABLE_433[4];}
else if (pa > 0 && pa <= 5){a = PA_TABLE_433[5];}
else if (pa > 5 && pa <= 7){a = PA_TABLE_433[6];}
else if (pa > 7){a = PA_TABLE_433[7];}
last_pa = 2;
}
else if (MHz >= 779 && MHz <= 899.99){
if (pa <= -30){a = PA_TABLE_868[0];}
else if (pa > -30 && pa <= -20){a = PA_TABLE_868[1];}
else if (pa > -20 && pa <= -15){a = PA_TABLE_868[2];}
else if (pa > -15 && pa <= -10){a = PA_TABLE_868[3];}
else if (pa > -10 && pa <= -6){a = PA_TABLE_868[4];}
else if (pa > -6 && pa <= 0){a = PA_TABLE_868[5];}
else if (pa > 0 && pa <= 5){a = PA_TABLE_868[6];}
else if (pa > 5 && pa <= 7){a = PA_TABLE_868[7];}
else if (pa > 7 && pa <= 10){a = PA_TABLE_868[8];}
else if (pa > 10){a = PA_TABLE_868[9];}
last_pa = 3;
}
else if (MHz >= 900 && MHz <= 928){
if (pa <= -30){a = PA_TABLE_915[0];}
else if (pa > -30 && pa <= -20){a = PA_TABLE_915[1];}
else if (pa > -20 && pa <= -15){a = PA_TABLE_915[2];}
else if (pa > -15 && pa <= -10){a = PA_TABLE_915[3];}
else if (pa > -10 && pa <= -6){a = PA_TABLE_915[4];}
else if (pa > -6 && pa <= 0){a = PA_TABLE_915[5];}
else if (pa > 0 && pa <= 5){a = PA_TABLE_915[6];}
else if (pa > 5 && pa <= 7){a = PA_TABLE_915[7];}
else if (pa > 7 && pa <= 10){a = PA_TABLE_915[8];}
else if (pa > 10){a = PA_TABLE_915[9];}
last_pa = 4;
}
if (modulation == 2){
PA_TABLE[0] = 0;
PA_TABLE[1] = a;
}else{
PA_TABLE[0] = a;
PA_TABLE[1] = 0;
}
SpiWriteBurstReg(CC1101_PATABLE,PA_TABLE,8);
}

void SpiWriteBurstReg(uint8_t addr, uint8_t *buffer, uint8_t num)
{
  uint8_t i, temp;
  //SpiStart();
  temp = addr | WRITE_BURST;
  //digitalWrite(SS_PIN, LOW);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
  //while(digitalRead(MISO_PIN));
  while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
  //SPI.transfer(temp);
  spi_transfer(temp);
  for (i = 0; i < num; i++)
  {
  //SPI.transfer(buffer[i]);
	  spi_transfer( buffer[i]);
  }
  //digitalWrite(SS_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
  //SpiEnd();
}



void setCrc(bool v){
Split_PKTCTRL0();
pc0CRC_EN = 0;
if (v==1){pc0CRC_EN=4;}
SpiWriteReg(CC1101_PKTCTRL0, pc0WDATA+pc0PktForm+pc0CRC_EN+pc0LenConf);
}

bool CheckRxFifo(int t){
if(trxstate!=2){SetRx();}
if(SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO){
HAL_Delay(t);
return 1;
}else{
return 0;
}
}

void SetRx(void)
{
  SpiStrobe(CC1101_SIDLE);
  SpiStrobe(CC1101_SRX);        //start receive
  trxstate=2;
}

void SpiStrobe(uint8_t strobe)
{
  //SpiStart();
  //digitalWrite(SS_PIN, LOW);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
  //while(digitalRead(MISO_PIN));
  while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
  //SPI.transfer(strobe);
  spi_transfer( strobe );
  //digitalWrite(SS_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
  //SpiEnd();
}

bool CheckCRC(void){
uint8_t lqi=SpiReadStatus(CC1101_LQI);
 bool crc_ok = bitRead(lqi,7);

//bool crc_ok = (lqi == '\001');

if (crc_ok == 1){
return 1;
}else{
SpiStrobe(CC1101_SFRX);
SpiStrobe(CC1101_SRX);
return 0;
}
}


int getRssi(void)
{
	int rssi;
	rssi=SpiReadStatus(CC1101_RSSI);
	if (rssi >= 128){
		rssi = (rssi-256)/2-74;
	}
	else{
		rssi = (rssi/2)-74;
	}
	return rssi;
}

uint8_t getLqi(void)
{
uint8_t lqi;
lqi=SpiReadStatus(CC1101_LQI);
return lqi;
}


uint8_t ReceiveData(uint8_t *rxBuffer)
{
	uint8_t size;
	uint8_t status[2];

	if(SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO)
	{
		size=SpiReadReg(CC1101_RXFIFO);
		SpiReadBurstReg(CC1101_RXFIFO,rxBuffer,size);
		SpiReadBurstReg(CC1101_RXFIFO,status,2);
		SpiStrobe(CC1101_SFRX);
    SpiStrobe(CC1101_SRX);
		return size;
	}
	else
	{
		SpiStrobe(CC1101_SFRX);
    SpiStrobe(CC1101_SRX);
 		return 0;
	}
}


uint8_t SpiReadReg(uint8_t addr)
{
	uint8_t temp, value;
  //SpiStart();
  temp = addr| READ_SINGLE;
  //digitalWrite(SS_PIN, LOW);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
  //while(digitalRead(MISO_PIN));
  while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
  //SPI.transfer(temp);
  spi_transfer( temp );
  //value=SPI.transfer(0);
  value = spi_transfer( 0 );
  //digitalWrite(SS_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
  //SpiEnd();
  return value;
}
void SpiReadBurstReg(uint8_t addr, uint8_t *buffer, uint8_t num)
{
  uint8_t i,temp;
  //SpiStart();
  temp = addr | READ_BURST;
  //digitalWrite(SS_PIN, LOW);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,RESET);
  //while(digitalRead(MISO_PIN));
  while(HAL_GPIO_ReadPin(GPIO,SPI1_MISO));
  //SPI.transfer(temp);
  spi_transfer( temp );
  for(i=0;i<num;i++)
  {
  //buffer[i]=SPI.transfer(0);
  buffer[i] = spi_transfer( 0 );
  }
  //digitalWrite(SS_PIN, HIGH);
  HAL_GPIO_WritePin(GPIO,SPI1_NSS,SET);
  //SpiEnd();
}


bool getCC1101(void){
//setSpi();
	if (SpiReadStatus(0x31)>0){
		return 1;
	}else{
		return 0;
	}
}

uint8_t getMode(void){
	return trxstate;
}

bool setClb(uint8_t b, uint8_t s, uint8_t e){
	if (b == 1){
		clb1[0]=s;
		clb1[1]=e;
	}
	else if (b == 2){
		clb2[0]=s;
		clb2[1]=e;
	}
	else if (b == 3){
		clb3[0]=s;
		clb3[1]=e;
	}
	else if (b == 4){
		clb4[0]=s;
		clb4[1]=e;
	}
}


void setSyncWord(uint8_t sh, uint8_t sl){
	SpiWriteReg(CC1101_SYNC1, sh);
	SpiWriteReg(CC1101_SYNC0, sl);
}


void setAddr(uint8_t v){
	SpiWriteReg(CC1101_ADDR, v);
}


void setPQT(uint8_t v){
	Split_PKTCTRL1();
	pc1PQT = 0;
	if (v>7){
		v=7;
	}
	pc1PQT = v*32;
	SpiWriteReg(CC1101_PKTCTRL1, pc1PQT+pc1CRC_AF+pc1APP_ST+pc1ADRCHK);
}


void setCRC_AF(bool v){
	Split_PKTCTRL1();
	pc1CRC_AF = 0;
	if (v==1){
		pc1CRC_AF=8;
	}
	SpiWriteReg(CC1101_PKTCTRL1, pc1PQT+pc1CRC_AF+pc1APP_ST+pc1ADRCHK);
}

void setAppendStatus(bool v){
	Split_PKTCTRL1();
	pc1APP_ST = 0;
	if (v==1){
		pc1APP_ST=4;
	}
	SpiWriteReg(CC1101_PKTCTRL1, pc1PQT+pc1CRC_AF+pc1APP_ST+pc1ADRCHK);
}

void setAdrChk(uint8_t v){
	Split_PKTCTRL1();
	pc1ADRCHK = 0;
	if (v>3){
		v=3;
	}
	pc1ADRCHK = v;
	SpiWriteReg(CC1101_PKTCTRL1, pc1PQT+pc1CRC_AF+pc1APP_ST+pc1ADRCHK);
}

void setWhiteData(bool v){
	Split_PKTCTRL0();
	pc0WDATA = 0;
	if (v == 1){
		pc0WDATA=64;
	}
	SpiWriteReg(CC1101_PKTCTRL0, pc0WDATA+pc0PktForm+pc0CRC_EN+pc0LenConf);
}

void setPktFormat(uint8_t v){
	Split_PKTCTRL0();
	pc0PktForm = 0;
	if (v>3){
		v=3;
	}
	pc0PktForm = v*16;
	SpiWriteReg(CC1101_PKTCTRL0, pc0WDATA+pc0PktForm+pc0CRC_EN+pc0LenConf);
}

void setLengthConfig(uint8_t v){
	Split_PKTCTRL0();
	pc0LenConf = 0;
	if (v>3){
		v=3;
	}
	pc0LenConf = v;
	SpiWriteReg(CC1101_PKTCTRL0, pc0WDATA+pc0PktForm+pc0CRC_EN+pc0LenConf);
}

void setPacketLength(uint8_t v){
	SpiWriteReg(CC1101_PKTLEN, v);
}

void setDcFilterOff(bool v){
	Split_MDMCFG2();
	m2DCOFF = 0;
	if (v==1){
		m2DCOFF=128;
	}
	SpiWriteReg(CC1101_MDMCFG2, m2DCOFF+m2MODFM+m2MANCH+m2SYNCM);
}

void setManchester(bool v){
	Split_MDMCFG2();
	m2MANCH = 0;
	if (v==1){
		m2MANCH=8;
	}
	SpiWriteReg(CC1101_MDMCFG2, m2DCOFF+m2MODFM+m2MANCH+m2SYNCM);
}

void setSyncMode(uint8_t v){
	Split_MDMCFG2();
	m2SYNCM = 0;
	if (v>7){
		v=7;
	}
	m2SYNCM=v;
	SpiWriteReg(CC1101_MDMCFG2, m2DCOFF+m2MODFM+m2MANCH+m2SYNCM);
}

void setFEC(bool v){
	Split_MDMCFG1();
	m1FEC=0;
	if (v==1){
		m1FEC=128;
	}
	SpiWriteReg(CC1101_MDMCFG1, m1FEC+m1PRE+m1CHSP);
}

void setPRE(uint8_t v){
	Split_MDMCFG1();
	m1PRE=0;
	if (v>7){
		v=7;
	}
	m1PRE = v*16;
	SpiWriteReg(CC1101_MDMCFG1, m1FEC+m1PRE+m1CHSP);
}

void setChannel(uint8_t ch){
	chan = ch;
	SpiWriteReg(CC1101_CHANNR,   chan);
}

void setChsp(float f){
	Split_MDMCFG1();
	uint8_t MDMCFG0 = 0;
	m1CHSP = 0;
	if (f > 405.456543){
		f = 405.456543;
	}
	if (f < 25.390625){
		f = 25.390625;
	}
	for (int i = 0; i<5; i++){
		if (f <= 50.682068){
			f -= 25.390625;
			f /= 0.0991825;
			MDMCFG0 = f;
			float s1 = (f - MDMCFG0) *10;
			if (s1 >= 5){
				MDMCFG0++;
			}
			i = 5;
		}
		else{
			m1CHSP++;
			f/=2;
		}
	}
	SpiWriteReg(19,m1CHSP+m1FEC+m1PRE);
	SpiWriteReg(20,MDMCFG0);
}

void setRxBW(float f){
	Split_MDMCFG4();
	int s1 = 3;
	int s2 = 3;
	for (int i = 0; i<3; i++){
		if (f > 101.5625){
			f/=2; s1--;
		}
		else{
			i=3;
		}
	}
	for (int i = 0; i<3; i++){
		if (f > 58.1){
			f/=1.25; s2--;
		}
		else{
			i=3;
		}
	}
	s1 *= 64;
	s2 *= 16;
	m4RxBw = s1 + s2;
	SpiWriteReg(16,m4RxBw+m4DaRa);
}

void setDRate(float d){
	Split_MDMCFG4();
	float c = d;
	uint8_t MDMCFG3 = 0;
	if (c > 1621.83){
		c = 1621.83;
	}
	if (c < 0.0247955){
		c = 0.0247955;
	}
	m4DaRa = 0;
	for (int i = 0; i<20; i++){
		if (c <= 0.0494942){
			c = c - 0.0247955;
			c = c / 0.00009685;
			MDMCFG3 = c;
			float s1 = (c - MDMCFG3) *10;
			if (s1 >= 5){
				MDMCFG3++;
			}
			i = 20;
		}
		else{
			m4DaRa++;
			c = c/2;
		}
	}
	SpiWriteReg(16,  m4RxBw+m4DaRa);
	SpiWriteReg(17,  MDMCFG3);
}

void setDeviation(float d){
	float f = 1.586914;
	float v = 0.19836425;
	int c = 0;
	if (d > 380.859375){
		d = 380.859375;
	}
	if (d < 1.586914){
		d = 1.586914;
	}
	for (int i = 0; i<255; i++){
		f+=v;
		if (c==7){
			v*=2;c=-1;i+=8;
		}
		if (f>=d){
			c=i;i=255;
		}
		c++;
	}
	SpiWriteReg(21,c);
}

void SetTx(void)
{
  SpiStrobe(CC1101_SIDLE);
  SpiStrobe(CC1101_STX);        //start send
  trxstate=1;
}

void SetTx_MHz(float mhz)
{
  SpiStrobe(CC1101_SIDLE);
  setMHZ(mhz);
  SpiStrobe(CC1101_STX);        //start send
  trxstate=1;
}

void SetRx_MHz(float mhz)
{
  SpiStrobe(CC1101_SIDLE);
  setMHZ(mhz);
  SpiStrobe(CC1101_SRX);        //start receive
  trxstate=2;
}

void setSres(void)
{
  SpiStrobe(CC1101_SRES);
  trxstate=0;
}

void setSidle(void)
{
  SpiStrobe(CC1101_SIDLE);
  trxstate=0;
}

void goSleep(void){
  trxstate=0;
  SpiStrobe(0x36);//Exit RX / TX, turn off frequency synthesizer and exit
  SpiStrobe(0x39);//Enter power down mode when CSn goes high.
}

void SendData(char *txchar)
{
	int len = strlen(txchar);
	uint8_t chartobyte[len];
	for (int i = 0; i<len; i++){
		chartobyte[i] = txchar[i];
	}
	SendData_buffersize(chartobyte,len);
}

void SendData_buffersize(uint8_t *txBuffer,uint8_t size)
{
  SpiWriteReg(CC1101_TXFIFO,size);
  SpiWriteBurstReg(CC1101_TXFIFO,txBuffer,size);      //write data to send
  SpiStrobe(CC1101_SIDLE);
  SpiStrobe(CC1101_STX);                  //start send
    while (!digitalRead(GDO0));               // Wait for GDO0 to be set -> sync transmitted
    while (digitalRead(GDO0));                // Wait for GDO0 to be cleared -> end of packet
  SpiStrobe(CC1101_SFTX);                 //flush TXfifo
  trxstate=1;
}

void SendData_txsize(char *txchar,int t)
{
	int len = strlen(txchar);
	uint8_t chartobyte[len];
	for (int i = 0; i<len; i++){
		chartobyte[i] = txchar[i];
	}
	SendData_tx_buffer(chartobyte,len,t);
}

void SendData_tx_buffer(uint8_t *txBuffer,uint8_t size,int t)
{
  SpiWriteReg(CC1101_TXFIFO,size);
  SpiWriteBurstReg(CC1101_TXFIFO,txBuffer,size);      //write data to send
  SpiStrobe(CC1101_SIDLE);
  SpiStrobe(CC1101_STX);                  //start send
  HAL_Delay(t);
  SpiStrobe(CC1101_SFTX);                 //flush TXfifo
  trxstate=1;
}

uint8_t CheckReceiveFlag(void)
{
  if(trxstate!=2){SetRx();}
	if(HAL_GPIO_ReadPin(GDO0_PORT,GDO0))			//receive data
	{
		while (HAL_GPIO_ReadPin(GDO0_PORT,GDO0));
		return 1;
	}
	else							// no data
	{
		return 0;
	}
}
