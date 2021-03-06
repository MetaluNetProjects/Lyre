/*********************************************************************
 *
 *                Lyre for 8X2A + 2 VNH
 *
 *********************************************************************
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Antoine Rousseau  aug 28 2015     Original.
                     apr 11 2016     Update to latest Fraise & change mag hmc5883 for incremental sensor + zero
 ********************************************************************/

/*
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301, USA.
*/
#define BOARD 8X2A

#include <fruit.h>


//#include <dcmotor.h>
#include <pid.h>
#include <ramp.h>
//#include <servo.h>
#include <dcmotor.h>
#include <analog.h>
#include <switch.h>

//#include <i2c_master.h>
//#include <hmc5883.h>

//-------------  Timer1 macros :  ---------------------------------------- 
//prescaler=PS fTMR1=FOSC/(4*PS) nbCycles=0xffff-TMR1init T=nbCycles/fTMR1=(0xffff-TMR1init)*4PS/FOSC
//TMR1init=0xffff-(T*FOSC/4PS) ; max=65536*4PS/FOSC : 
//ex: PS=8 : T=0.01s : TMR1init=0xffff-15000
//Maximum 1s !!
#define	TMR1init(T) (0xffff-((T*FOSC)/32000)) //ms ; maximum: 8MHz:262ms 48MHz:43ms 64MHz:32ms
#define	TMR1initUS(T) (0xffff-((T*FOSC)/32000000)) //us ; 
#define InitTimer(T) do{ TMR1H=TMR1init(T)/256 ; TMR1L=TMR1init(T)%256; PIR1bits.TMR1IF=0; }while(0)
#define InitTimerUS(T) do{ TMR1H=TMR1initUS(T)/256 ; TMR1L=TMR1initUS(T)%256; PIR1bits.TMR1IF=0; }while(0)
#define TimerOut() (PIR1bits.TMR1IF)

DCMOTOR_DECLARE(X);
DCMOTOR_DECLARE(Y);

long int TestVar,TestVar2;

void highInterrupts()
{
	//Servo_ISR();

	if(PIR1bits.TMR1IF) {
		//UpdateLeds();
		DCMOTOR_CAPTURE_SERVICE_SINGLE(X);
		InitTimerUS(10);
	}
	//IRbarrier_ISR();
}

//void lowInterrupts() {}

t_delay mainDelay;

unsigned char PERIOD=25;
unsigned char t=25,t2;

/*unsigned char buf[13] = { 'B' }; //'B' + 3x4 chars
unsigned char len = 0;
int anVal;*/

// --------- Fraise Watchdog : ----------------//
unsigned int wdC = 0; //watchdog count

void wdReset(void)
{
	wdC = 0;
}

#define wdOK() (wdC < (200*2)) // 2 seconds
#define wdService() do {if(wdOK()) wdC++;} while(0)
//----------------------------------------------//

void setup(void)
{
	//unsigned long t3,servoSOF;
		
	fruitInit();
	switchInit();
	
	//i2cm_init(I2C_MASTER, I2C_SLEW_ON, FOSC/400000/4-1) ;		//400kHz : clock = FOSC/(4 * (SSPADD + 1)) : SSPADD=FOSC/clock/4-1
	//HMC5883_init();
	
// --------- init motors PWM on TMR2 ----------- // now done by core & dcmotor
	// PWM on Timer2 : fPWM = fOSC / (TMR2prescale * 4 * (PR2+1))
	//						= 64000000Hz/(4 * 4 * 256) = 15625Hz
	/*CCP2CON=CCP3CON=0b00001100 ; // single PWM active high
	T2CONbits.T2CKPS0=1; //prescaler 4
	PR2=255;
	T2CONbits.TMR2ON=1;*/

// ------- init PWM1 to pulse MOTC_IN1
	//PSTR1CON=0;
	//PSTR1CONbits.STR1D=1;

// ------- init PWM2 to pulse MOTD_EN

	//PSTR2CON=0;
	//PSTR2CONbits.STR2B=1;

#if 1
// ---------- capture timer : TMR1 ------------
	T1CON=0b00110011;//src=fosc/4,ps=8,16bit r/w,on.
	PIE1bits.TMR1IE=1;  //1;
	IPR1bits.TMR1IP=1;


// ---------- Servos ------------
	//Servo_Init();
	
	//SERVO_SET_PORT(0,K3);
	
	dcmotorInit(X);
	DCMOTOR(X).Setting.PosWindow = 1;
	DCMOTOR(X).Setting.PwmMin = 50;
	DCMOTOR(X).Setting.PosErrorGain = 6;
	DCMOTOR(X).Setting.onlyPositive = 0;
	
	DCMOTOR(X).PosRamp.maxSpeed = 40;
	DCMOTOR(X).PosRamp.maxAccel = 50;
	DCMOTOR(X).PosRamp.maxDecel = 50;
	rampSetPos(&DCMOTOR(X).PosRamp, 0);

	DCMOTOR(X).PosPID.GainP = 120; //90
	DCMOTOR(X).PosPID.GainI = 10;
	DCMOTOR(X).PosPID.GainD = 0;
	DCMOTOR(X).PosPID.MaxOut = 1023;

	DCMOTOR(X).VolVars.homed = 1;
	pinModeDigitalOut(MXEN2);
	digitalSet(MXEN2);

	dcmotorInit(Y);
	DCMOTOR(Y).Setting.PosWindow = 100;
	DCMOTOR(Y).Setting.PwmMin = 50;
	DCMOTOR(Y).Setting.PosErrorGain = 1;
	
	DCMOTOR(Y).PosRamp.maxSpeed = 2000;
	DCMOTOR(Y).PosRamp.maxAccel = 5000;
	DCMOTOR(Y).PosRamp.maxDecel = 5000;
	rampSetPos(&DCMOTOR(Y).PosRamp, 16384);

	DCMOTOR(Y).PosPID.GainP = 200; //90
	DCMOTOR(Y).PosPID.GainI = 1;
	DCMOTOR(Y).PosPID.GainD = 0;
	DCMOTOR(Y).PosPID.MaxOut = 1023;

	DCMOTOR(Y).VolVars.homed = 1;
	pinModeDigitalOut(MYEN2);
	digitalSet(MYEN2);
#endif
	analogInit();
	//ANALOG_SELECT(MOTX_POSCHAN, MOTX_POS);
	analogSelect(MOTX_CSCHAN, MXCS);
	//ANALOG_SELECT(MOTY_POSCHAN, MOTY_POS);
	analogSelect(MOTY_CSCHAN, MYCS);

	analogSelect(ACCX_CHAN, ACCX);
	analogSelect(ACCZ_CHAN, ACCZ);
	
    switchSelect(0,K7); 
	//EEreadMain();
	delayStart(mainDelay, 5000); 	// init the mainDelay to 5 ms
}

// ---------- Main loop ------------

void loop() {
	//ADService();
	fraiseService();
	analogService();
	fraiseService();
	//Servo_Service();
	fraiseService();
	switchService();
	/*if(Elapsed(servoSOF)>Micros(18000UL)) {
		Servo_Rewind();
		servoSOF=GetTime();
	}*/

	if(delayFinished(mainDelay)) // when mainDelay triggers 
	{
		delayStart(mainDelay, 5000); 	// re-init mainDelay

		//DCMOT_GETPOS(X) = Analog_Get(0);
		if(!switchSend()) analogSend();

		/*anVal = Analog_Get(MOTX_POSCHAN);
		if((anVal<11000) || (anVal>21000)) {
			DCMOT(X).Vars.PWMConsign = 0; 
			DCMOT(X).Setting.Mode = 0;
		}
		else DCMOT_GETPOS(X) = anVal;

		anVal = Analog_Get(MOTY_POSCHAN);
		if((anVal<11000) || (anVal>21000)) {
			DCMOT(Y).Vars.PWMConsign = 0; 
			DCMOT(Y).Setting.Mode = 0;
		}
		else DCMOT_GETPOS(Y) = anVal;

		*/
		wdService();
		if(!wdOK()) {
			DCMOTOR(X).Vars.PWMConsign = 0;
			DCMOTOR(X).Setting.Mode = 0;
			DCMOTOR(Y).Vars.PWMConsign = 0;
			DCMOTOR(Y).Setting.Mode = 0;
		}

		DCMOTOR_COMPUTE_SINGLE(X, SYM);
		DCMOTOR_COMPUTE(Y, SYM);

		fraiseService();
		//UpdateMotorD();
		//DCMOT_COMPUTE(D, ASYM);
		//FraiseService();
		//ADService();
		//UpdateMotorB();
		//FraiseService();
		//ADService();
		//HMC5883_service();
		
		if(!t--){
			t=PERIOD;
			t2++;
			printf("C M %ld %ld %d\n",DCMOTOR_GETPOS(X),(long)(DCMOTOR(X).PosRamp.currentPos>>(RAMP_UINCPOW)),DCMOTOR(X).VolVars.homed);
			//printf("C M %d %d %d\n",HMC5883_getX(),HMC5883_getY(),HMC5883_getZ());
			/*len = 0;
			buf[len++] = 'B';
			buf[len++] = 200;
			buf[len++] = IRB1;
			buf[len++] = IRB2;
			buf[len++] = '\n';
			FrTXPacket(buf,len);*/
			#if 0
			if(t2%2) printf("C MX %ld %ld %d %d\n",DCMOT_GETPOS(X),(long)(DCMOT(X).PosRamp.ConsignPos>>RAMP_UINCPOW), DCMOT(X).Vars.PWMConsign,DCMOT(X).Setting.Mode/*.VolVars.homed*/);
			/*else printf("C MD %ld %ld %d %d\n",DCMOT_GETPOS(D),(long)(DCMOT(D).PosRamp.ConsignPos>>RAMP_UINCPOW), DCMOT(D).Vars.PWMConsign,DCMOT(D).VolVars.homed);*/
			#endif
		}
	}
}


void fraiseReceiveChar()
{
	unsigned char c;
	unsigned char l = fraiseGetLen();	
	c=fraiseGetChar();
	//printf("Cchar in:%c %c\n",c,c2);
	if(c=='L'){	
		c=fraiseGetChar();
		/*if(c=='0') 
			{LED=0;}
		else LED=1;*/
	}
	else if(c=='E') {
		printf("C");
		for(c=1;c<l;c++) printf("%c",fraiseGetChar());
		putchar('\n');
	}
	else if(c=='W') { //watchdog
		wdReset();
	}
	
}


void fraiseReceive()
{
	unsigned char c;//,c2;
	
	c=fraiseGetChar();

	switch(c) {
		PARAM_CHAR(1,t2); break;
		PARAM_CHAR(2,PERIOD); break;
		//case 20 : Servo_Input(); break;
		case 120 : DCMOTOR_INPUT(X); break;
		case 121 : DCMOTOR_INPUT(Y); break;
		
		//case 255 : EEwriteMain(); break;
	}
}

/*void EEdeclareMain()
{
	//DCMOT_DECLARE_EE(X);
	//DCMOT_DECLARE_EE(D);
}*/
