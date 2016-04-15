#include <core.h>
#include <i2c_master.h>
#include <hmc5883.h>

/***************************************************************************
  This partly comes from a library for the HMC5883 magnentometer/compass
  Designed specifically to work with the Adafruit HMC5883 Breakout
  http://www.adafruit.com/products/1746
  These displays use I2C to communicate, 2 pins are required to interface.
  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!
  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************//*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define HMC5883_ADDRESS_MAG            (0x3C >> 1)         // 0011110x
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    typedef enum
    {
      HMC5883_REGISTER_MAG_CRA_REG_M             = 0x00,
      HMC5883_REGISTER_MAG_CRB_REG_M             = 0x01,
      HMC5883_REGISTER_MAG_MR_REG_M              = 0x02,
      HMC5883_REGISTER_MAG_OUT_X_H_M             = 0x03,
      HMC5883_REGISTER_MAG_OUT_X_L_M             = 0x04,
      HMC5883_REGISTER_MAG_OUT_Z_H_M             = 0x05,
      HMC5883_REGISTER_MAG_OUT_Z_L_M             = 0x06,
      HMC5883_REGISTER_MAG_OUT_Y_H_M             = 0x07,
      HMC5883_REGISTER_MAG_OUT_Y_L_M             = 0x08,
      HMC5883_REGISTER_MAG_SR_REG_Mg             = 0x09,
      HMC5883_REGISTER_MAG_IRA_REG_M             = 0x0A,
      HMC5883_REGISTER_MAG_IRB_REG_M             = 0x0B,
      HMC5883_REGISTER_MAG_IRC_REG_M             = 0x0C,
      HMC5883_REGISTER_MAG_TEMP_OUT_H_M          = 0x31,
      HMC5883_REGISTER_MAG_TEMP_OUT_L_M          = 0x32
    } hmc5883MagRegisters_t;
/*=========================================================================*/

/*=========================================================================
    MAGNETOMETER GAIN SETTINGS
    -----------------------------------------------------------------------*/
    typedef enum
    {
      HMC5883_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
      HMC5883_MAGGAIN_1_9                        = 0x40,  // +/- 1.9
      HMC5883_MAGGAIN_2_5                        = 0x60,  // +/- 2.5
      HMC5883_MAGGAIN_4_0                        = 0x80,  // +/- 4.0
      HMC5883_MAGGAIN_4_7                        = 0xA0,  // +/- 4.7
      HMC5883_MAGGAIN_5_6                        = 0xC0,  // +/- 5.6
      HMC5883_MAGGAIN_8_1                        = 0xE0   // +/- 8.1
    } hmc5883MagGain;	
/*=========================================================================*/

static WORD mX, mY, mZ;

void HMC5883_init()
{
	i2cm_begin(HMC5883_ADDRESS_MAG,0);
	i2cm_writechar(HMC5883_REGISTER_MAG_MR_REG_M);
	i2cm_writechar(0x00);
	i2cm_stop();
	mX._word = 500;
}

void HMC5883_service()
{
	i2cm_begin(HMC5883_ADDRESS_MAG,0);
	i2cm_writechar(HMC5883_REGISTER_MAG_OUT_X_H_M);
	i2cm_stop();

	i2cm_begin(HMC5883_ADDRESS_MAG,1);
	MSB(mX) = i2cm_readchar();
	i2cm_ack();
	LSB(mX) = i2cm_readchar();
	i2cm_ack();
	MSB(mZ) = i2cm_readchar();
	i2cm_ack();
	LSB(mZ) = i2cm_readchar();
	i2cm_ack();
	MSB(mY) = i2cm_readchar();
	i2cm_ack();
	LSB(mY) = i2cm_readchar();
	i2cm_stop();
}

unsigned int HMC5883_getX() { return mX._word; }
unsigned int HMC5883_getZ() { return mZ._word; }
unsigned int HMC5883_getY() { return mY._word; }

