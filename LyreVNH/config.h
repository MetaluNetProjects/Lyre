#ifndef _CONFIG_H_
#define _CONFIG_H_


//DCMotor config
#define UINCPOW 10

#define RAMP_UINCPOW 10
#define RAMP_MAXERROR 1  

//#define MOTC_PWM 1
//#define MOTD_PWM 2

#define MOTX_PWM 3
#define MXEN MBEN
#define MXEN1 MAEN
#define MXEN2 MA2
#define MX1 MB1
#define MX2 MB2
#define MXCS MA1

#define MOTX_END KZ1
#define MOTX_ENDLEVEL 0
#define MOTX_A K9
#define MOTX_B K8

#define MOTX_CSCHAN 0

#define MOTX_CONFIG() 
//do{/*init PWM1 to P1B for MOTB_PWM : */ PSTR1CON=0; PSTR1CONbits.STR1B=1;pinModeDigitalOut(K9);} while(0)

//MOT Y :
#define MOTY_PWM 2
#define MYEN MDEN
#define MYEN1 MCEN
#define MYEN2 MC2
#define MY1 MD1
#define MY2 MD2
#define MYCS MC1

#define MOTY_END KZ1
#define MOTY_ENDLEVEL 0
#define MOTY_A K5
#define MOTY_B K5

#define MOTY_CSCHAN 1
#define MOTY_CONFIG() do{/*init PWM2 to pulse MOTD_EN : */ PSTR2CON=0;PSTR2CONbits.STR2B=1;/*pinModeDigitalOut(K9);*/} while(0)

#define ANALOG_FILTER 5

#define ACCX K3
#define ACCX_CHAN 3
#define ACCZ K4
#define ACCZ_CHAN 4

/*----------- I2C --------------*/
//I2C port 2 : SCL=K8 SDA=K9
/*#define I2CMASTER_PORT 2

#define I2C1SCLPORT C
#define I2C1SCLBIT  3
#define I2C1SDAPORT C
#define I2C1SDABIT  4

#define I2C2SCLPORT B // vert, K8
#define I2C2SCLBIT  1
#define I2C2SDAPORT B // jaune, K9
#define I2C2SDABIT  2*/


#endif // _CONFIG_H_

