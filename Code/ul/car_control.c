
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include "i2c-dev.h"


/* watch code */
#include <math.h>
#include <unistd.h> //sleep
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h> // serial ports
#define BAUDRATE 115200         
#define DEVICEPORT "/dev/ttyACM0"


#define PCA9685_SUBADR1 0x2 /**< i2c bus address 1 */
#define PCA9685_SUBADR2 0x3 /**< i2c bus address 2 */
#define PCA9685_SUBADR3 0x4 /**< i2c bus address 3 */

#define PCA9685_MODE1 0x0 /**< Mode Register 1 */
#define PCA9685_PRESCALE 0xFE /**< Prescaler for PWM output frequency */

#define LED0_ON_L 0x6 /**< LED0 output and brightness control byte 0 */
#define LED0_ON_H 0x7 /**< LED0 output and brightness control byte 1 */
#define LED0_OFF_L 0x8 /**< LED0 output and brightness control byte 2 */
#define LED0_OFF_H 0x9 /**< LED0 output and brightness control byte 3 */

#define ALLLED_ON_L 0xFA /**< load all the LEDn_ON registers, byte 0 */
#define ALLLED_ON_H 0xFB /**< load all the LEDn_ON registers, byte 1 */
#define ALLLED_OFF_L 0xFC /**< load all the LEDn_OFF registers, byte 0 */
#define ALLLED_OFF_H 0xFD /**< load all the LEDn_OFF registers, byte 1 */

#define ADDRESS 0x40

/* function prototypes */
void pwmBegin(int file);
void delay(int milliseconds);
void pwmReset();
void setPwmFreq(int file, float freq);
void setPwm(int file, uint8_t num, uint16_t on, uint16_t off);
uint8_t read8(int file, uint8_t reg_addr);
void write8(int file, uint8_t reg_addr, uint8_t data);
void calibration(int fd);
void direction(int fd, int orientation);
void movement(int fd, int speed);
int set_speed(int x, int front_object);
int set_orientation(int y);


int pwmnum_b = 0;//backword 
int pwmnum_f = 1;//forward
/*int pwmnum_n = 2;//neutra */

int main(){
	int fd;
	float test = 9.0/2;
	int orientation =264;
	int speed = 305;
	char line[30];


	/*watch code*/
	char command[128];
	char response[2560];
	char start[3];
	char dataRequest[7];
	int cFlag = 0;
	int r, c;
	uint8_t x=0, y=0, z=0, u=0;
	unsigned int x_int = 0, y_int=0, z_int=0;

	start[0] = 0xFF;
	start[1] = 0x07;
	start[2] = 0x03; 

	dataRequest[0] = 0xFF;
	dataRequest[1] = 0x08;
	dataRequest[2] = 0x07; 
	dataRequest[3] = 0x00;
	dataRequest[4] = 0x00; 	
	dataRequest[5] = 0x00;
	dataRequest[6] = 0x00;

	struct termios  config;
	int wFile;
	wFile = open(DEVICEPORT, O_RDWR | O_NOCTTY | O_NDELAY);
	if(wFile == -1)
	{
		printf( "failed to open port\n" );
		return -1;
	}


	bzero(&config, sizeof(config));
	config.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	config.c_oflag = 0;
	tcflush(wFile, TCIFLUSH);
	tcsetattr(wFile,TCSANOW,&config);

	write(wFile,start,sizeof(start));
	printf("Sending Start\n");
	
	printf("test = %.2f \n", test);
	//delay(10000);
	printf("test = %.2f \n + 1 ", test + 1);
	pwmBegin(fd);
	calibration(fd); 
        
	FILE * pFile;
	pFile = fopen("/dev/mymodule", "r");
	if (pFile==NULL) {
		fputs("mymodule: module isn't loaded\n",stderr);
		return -1;
	}
	sleep(1);
	
	while (1) 
	{
		usleep(500000);
		//sleep(1);
		strcpy(response,"");

		//printf("Sending Req\n");
		//data request sequence: 0xFF 0x08 0x07 0x00 0x00 0x00 0x00;
		write(wFile,dataRequest,sizeof(dataRequest));
		//printf("\nRequest sent!\n");
	
		read(wFile,response,255);		
		u = response[3];
		x = response[4];
		y = response[5];
		z = response[6];
	

		x_int = (unsigned int) x;
		y_int = (unsigned int) y;
		z_int = (unsigned int) z;
		fgets(line, 30, pFile);
	      	printf("%s", line);
	    	

		if(u == 1)
		{
			printf("RES: %u %u %u\n", x, y, z);			
			speed = set_speed(x, atoi(line));
			orientation = set_orientation(y);
		}
		else 
		{
			speed = 305;
			orientation = 264;
		}
		movement(fd,speed);
		direction(fd, orientation);
		fclose(pFile);
		pFile = fopen("/dev/mymodule", "r");
	
	}

	close(wFile);
}
void calibration(int fd) 
{
	setPwm(fd, pwmnum_b, 0, 530); //full backward 
	sleep(1);
	setPwm(fd, pwmnum_b, 0, 105); //full forward 
	sleep(1);
	setPwm(fd, pwmnum_b, 0, 305); //neutral 
	sleep(1);
	setPwm(fd, pwmnum_b, 0, 305); //neutral 
}

void movement(int fd, int speed)
{
	setPwm(fd, pwmnum_b, 0, speed);

}

void direction(int fd, int orientation) 
{
	setPwm(fd, pwmnum_f, 0, orientation); 

}


int set_speed(int x, int front_object)
{
	int speed;
	if(x >= 235 && x < 245 && front_object == 0) //lowest speed forward one 
	{
		//speed = 280; 
		speed = 285;
		
	}
	else if(x >= 220 && x < 235 && front_object == 0) //medium speed forward two
	{
		//speed = 250; 
		//speed = 280; 
		speed = 280;
	}
	else if(x >= 205 && x < 220 && front_object == 0) //medium speed forward three
	{
		//speed = 220; 
		//speed = 280; 
		speed = 270;
	}
	else if(x >= 190 && x < 205 && front_object == 0) //full speed forward
	{
		//speed = 190;
		//speed = 220;
		 //speed = 280;
		speed = 260;
	}


						
	else if(x > 15 && x <= 25) // lowest speed backward 1 
	{
		speed = 330;//330 the slowest 
	}
	else if(x > 25 && x <= 40) // medium speed backward 2
	{
		//speed = 355;
		speed = 335;
	}
	else if(x > 40 && x <= 55) // medium speed backward 3
	{
		//speed = 380;
		//speed = 355;
		speed = 340;
	}
	else if(x > 55 && x <= 70) // full speed backward 
	{
		//speed = 405;
		//speed = 355;
		speed = 350;
	}			
	else // neutral
	{
		speed = 305; 
	}
	return speed;
}

int set_orientation(int y)
{
	int orientation;
	if(y >= 235 && y < 250) //lowest degree right turn
	{
		orientation = 240;	
	}
	else if(y >= 220 && y < 235) //medium degree one right turn 
	{
		orientation = 215;	
	}
	else if(y >= 205 && y < 220) //medium degree two right turn 
	{
		orientation = 190;	
	}
	else if (y >= 190 && y < 205)  //full degree right turn 
	{
		orientation = 170;	
	}

	// for turning left  
	
	else if(y >= 10 && y < 25) //lowest degree  left turn 
	{
		orientation = 288;	
	}
	else if(y >= 25 && y < 40) //medium degree one left turn 
	{
		orientation = 325;	
	}
	else if(y >= 40 && y < 55) //medium degree two left turn 
	{
		orientation = 360;	
	}
	else if (y >= 55 && y < 70)  //full degree left turn 
	{
		orientation = 410;	
	} 
	else //straight
	{
		orientation = 264;	
	}
	return orientation;
}

void pwmBegin(int file){
	pwmReset();
	setPwmFreq(file, 54);
}

void pwmReset(int file){
	write8(file, PCA9685_MODE1, 0x80);
	delay(10);
}

void write8(int file, uint8_t reg_addr, uint8_t data){
	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE_FORCE, ADDRESS ) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}
	
	i2c_smbus_write_byte_data(file, reg_addr, data);
	close(file);
	
}

uint8_t read8(int file, uint8_t reg_addr) {

	file = open("/dev/i2c-0", O_RDWR);

	if( ioctl(file, I2C_SLAVE_FORCE, ADDRESS ) < 0)
	{
		fprintf( stderr, "Failed to set slave address: %m\n" );
	}
	
	return i2c_smbus_read_byte_data(file, reg_addr);

}


void setPwmFreq(int file, float freq){
	// setPwmFreq() --> prescale = 125  and get rid of float and floor
	freq *= 0.9; // Correct for overshoot in the frequency setting (see issue #11).
	float prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;

	uint8_t prescale = floor(prescaleval + 0.5);
	uint8_t oldmode = read8(file, PCA9685_MODE1);
	close(file);
	uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
	write8(file, PCA9685_MODE1, newmode);            // go to sleep
	write8(file, PCA9685_PRESCALE, prescale);        // set the prescaler
	write8(file, PCA9685_MODE1, oldmode);
	delay(5);
	write8(file, PCA9685_MODE1, oldmode |0xa0); //  This sets the MODE1 register to turn on auto increment.

}

void setPwm(int file, uint8_t num, uint16_t on, uint16_t off){
	/*write8(file, LED0_ON_L + 4 * num);
	write8(file, on);
	write8(file, on >> 8);
	write8(file, off);
	write8(file, off >> 8);	*/
	write8(file, LED0_ON_L + 4 * num, on);
	write8(file, LED0_ON_H + 4 * num, on >> 8);
	write8(file, LED0_OFF_L + 4 * num, off);
	write8(file, LED0_OFF_H + 4 * num, off >> 8);
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}
