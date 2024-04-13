/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

//RCC Register to enable Port C, Port D, PortG, andPort E
 #define rccAHB1Register *((volatile uint32_t*) 0x40023830)

// **** KEY PAD ***

// Pin numbers of input/output register setting
#define ROW_1 0x2 // PE2
#define ROW_2 0x3 // PE3
#define ROW_3 0x4 // PE4
#define ROW_4 0x5 // PE5

#define COL_1 0xD // PC13
#define COL_2 0xC // PC12
#define COL_3 0xB // PC11
#define COL_4 0x8 // PC8




// MODE register for Port C (Cols) and Port E (Rows)
#define modeRegPortC  *((volatile uint32_t*) 0x40020800)
#define modeRegPortE  *((volatile uint32_t*) 0x40021000)

// MODE register for Port C (Cols) and Port E (Rows)


// OUTPUT DATA register for Port E (Rows)
#define outputRegPortE *((volatile uint32_t*) 0x40021014)


// INPUT DATA register for Port C (Cols)
#define inputRegPortC *((volatile uint32_t* ) 0x40020810)



// PULlUP/PULLDOWN register for Port C (set to pull up)
#define pullRegPortC  *((volatile uint32_t*) 0x4002080C)




// *** LCD SCREEN ***


// Pin numbers of output register setting
#define PIN_D7 0x7 // PD7
#define PIN_D6 0x5 // PD5
#define PIN_D5 0x4 // PD4
#define PIN_D4 0x2 // PD2
#define PIN_RS 0x2 // PG2
#define PIN_E  0x3 // PG3

// MODE register for PORT G and PORT D

#define modeRegPortG  *((volatile uint32_t*) 0x40021800)
#define modeRegPortD  *((volatile uint32_t*) 0x40020C00)



// OUTPUT DATA register for Port G (PS and E Pins on LCD Display) and PORT D (Data pins)
#define outputRegPortG *((volatile uint32_t*) 0x40021814)
#define outputRegPortD  *((volatile uint32_t*) 0x40020C14) // This is used to send character data



void delay(uint32_t amount)
{
//	300,000 iterations ~= 150ms
	for(uint32_t i=0; i < amount; i++);
}

void enableEtoSend(){
	delay(50);
	outputRegPortG = outputRegPortG | (1 << PIN_E); // enabling E (PG3) to send command
	delay(50);
	outputRegPortG = outputRegPortG & ~(1 << PIN_E); // disabling E (PG3) to send command
	delay(50);

}

void clearDpins(){
	delay(50);
	outputRegPortD = outputRegPortD & ~(1 << PIN_D4);
	outputRegPortD = outputRegPortD & ~(1 << PIN_D5);
	outputRegPortD = outputRegPortD & ~(1 << PIN_D6);
	outputRegPortD = outputRegPortD & ~(1 << PIN_D7);
	delay(100);
}

void resetDisplay(){

		//	Sending 0000
		delay(150);
		outputRegPortD = outputRegPortD & ~(1 << PIN_D5); // clearing D5

		enableEtoSend();

		// Sending 0001
		delay(100);
		outputRegPortD = outputRegPortD | (1 << PIN_D4); // enabling D4

		enableEtoSend();

	// Return Home

		//	Sending 0000
		delay(100);
		outputRegPortD = outputRegPortD & ~(1 << PIN_D4); // disabling D4

		enableEtoSend();

		//	Sending 0010
		delay(100);
		outputRegPortD = outputRegPortD | (1 << PIN_D5); // enabling D5

		enableEtoSend();

		outputRegPortD = outputRegPortD & ~(1 << PIN_D5); // disabling D5
}


void sendLCDData(uint8_t data){

	uint8_t upperNibble = (data & 0xF0) >> 4;

	//	Enable RS
	outputRegPortG = outputRegPortG | (1 << PIN_RS); // enabling RS (PG2) to send data
	delay(100);


//	UPPER 4 bits
//	1st bit set
	if(upperNibble & 0x1 ){		outputRegPortD = outputRegPortD | (1 << PIN_D4);	}
//	Second bit set

	if (upperNibble & 0x2 ){	outputRegPortD = outputRegPortD | (1 << PIN_D5);	}

//	third bit set
	if (upperNibble & 0x4 ){	outputRegPortD = outputRegPortD | (1 << PIN_D6);	}

//	fourth bit set
	if (upperNibble & 0x8 ){	outputRegPortD = outputRegPortD | (1 << PIN_D7);	}

//	Send first 4 bits
	enableEtoSend();
	clearDpins();


//	LOWER 4 bits
//	1st bit set
	if(data & 0x1 ) {	outputRegPortD = outputRegPortD | (1 << PIN_D4);	}

//	Second bit set
	if (data & 0x2 ) {	outputRegPortD = outputRegPortD | (1 << PIN_D5);	}

//	third bit set
	if (data & 0x4 ) {	outputRegPortD = outputRegPortD | (1 << PIN_D6);	}

//	fourth bit set
	if (data & 0x8 ) {	outputRegPortD = outputRegPortD | (1 << PIN_D7);	}


//	Send second 4 bits
	enableEtoSend();
	clearDpins();

	delay(100);
	outputRegPortG = outputRegPortG & ~(1 << PIN_RS); // disabling RS (PG2)

}




int main(void)
{

//	Enable RCC for AHB1 (PORT E,PORT C, PORT D, PORT G)
	rccAHB1Register = rccAHB1Register | 0x5C;

//	Set ROWS (Port E) to OUTPUT mode (PE2,PE3,PE4,PE5)
	modeRegPortE = modeRegPortE | (1 << 4); //PE2
	modeRegPortE = modeRegPortE | (1 << 6); //PE3
	modeRegPortE = modeRegPortE | (1 << 8); //PE4
	modeRegPortE = modeRegPortE | (1 << 10); //PE5

// Set COLS to INPUT mode
	modeRegPortC = modeRegPortC | 0x00000000;

//	Set COLS (Port C) to PULL UP (PC8, PC11, PC12, PC13)
	pullRegPortC = pullRegPortC | (1 << 16); //PC8
	pullRegPortC = pullRegPortC | (1 << 22); //PC11
	pullRegPortC = pullRegPortC | (1 << 24); //PC12
	pullRegPortC = pullRegPortC | (1 << 26); //PC13

//	*** Setting up LCD Screen ***

// Set PG2, PG3 to Output Mode
	modeRegPortG = modeRegPortG | (1<<4); // PG2
	modeRegPortG = modeRegPortG | (1 << 6); // PG3

// Set PD2, PD4, PD5, and PD7 to output mode

	modeRegPortD = modeRegPortD | (1 << 4); // PD2
	modeRegPortD = modeRegPortD | (1 << 8); // PD4
	modeRegPortD = modeRegPortD | (1 << 10); // PD5
	modeRegPortD = modeRegPortD | (1 << 14); // PD7


// Change LCD Display into 4-bit mode

	delay(100);
	outputRegPortD = outputRegPortD | (1 << PIN_D5); // enabling D5 (PD4) to send 0100

	enableEtoSend();


//	Clear display
	resetDisplay();


// Display On cursor, Blinking
	//	Sending 0000 (D5 is reset in resetDisplay() so all pins are set LOW (0)  )
	enableEtoSend();


//			Sending 1111
	outputRegPortD = outputRegPortD | (1 << PIN_D7); // enabling D7 to send 1111
	outputRegPortD = outputRegPortD | (1 << PIN_D6); // enabling D6 to send 1111
	outputRegPortD = outputRegPortD | (1 << PIN_D5); // enabling D5 to send 1111
//	outputRegPortD = outputRegPortD | (1 << PIN_D4); // enabling D4 to send 1111

	enableEtoSend();

//	Clearing D pins
	outputRegPortD = outputRegPortD & ~(1 << PIN_D7); // disabling D7
	outputRegPortD = outputRegPortD & ~(1 << PIN_D6); // disabling D6
	outputRegPortD = outputRegPortD & ~(1 << PIN_D5); // disabling D5
//	outputRegPortD = outputRegPortD & ~(1 << PIN_D4); // disabling D4





	while(1){
//		Keep ROWS (PORT E) in HIGH mode (PE2,PE3,PE4,PE5)
		outputRegPortE = outputRegPortE | 0x3C;

//		Make R1 (PE2) LOW STATE
		outputRegPortE = outputRegPortE & ~(1 << ROW_1);

//
		if((inputRegPortC & (1 << COL_1)) == 0){
			delay(300000);
			sendLCDData(0x31);
			printf("1\n");

		} else if((inputRegPortC & (1 << COL_2)) == 0)
		{
			delay(300000);
			sendLCDData(0x32);
			printf("2\n");

		} else if((inputRegPortC & (1 << COL_3)) == 0)
		{
			delay(300000);
			sendLCDData(0x33);
			printf("3\n");

		}else if((inputRegPortC & (1 << COL_4)) == 0)
		{
			delay(300000);
			sendLCDData(0x41);
			printf("A\n");
		}


//		Keep ROWS (PORT E) in HIGH mode (PE2,PE3,PE4,PE5)
		outputRegPortE = outputRegPortE | 0x3C;

//		Make R2 (PE3) LOW STATE
		outputRegPortE = outputRegPortE & ~(1 << ROW_2);

//
		if((inputRegPortC & (1 << COL_1)) == 0){
			delay(300000);
			sendLCDData(0x34);
			printf("4\n");

		} else if((inputRegPortC & (1 << COL_2)) == 0)
		{
			delay(300000);
			sendLCDData(0x35);
			printf("5\n");

		} else if((inputRegPortC & (1 << COL_3)) == 0)
		{
			delay(300000);
			sendLCDData(0x36);
			printf("6\n");

		}else if((inputRegPortC & (1 << COL_4)) == 0)
		{
			delay(300000);
			sendLCDData(0x42);
			printf("B\n");
		}



//		Keep ROWS (PORT E) in HIGH mode (PE2,PE3,PE4,PE5)
		outputRegPortE = outputRegPortE | 0x3C;

//		Make R3 (PE4) LOW STATE
		outputRegPortE = outputRegPortE & ~(1 << ROW_3);

//
		if((inputRegPortC & (1 << COL_1)) == 0){
			delay(300000);
			sendLCDData(0x37);
			printf("7\n");

		} else if((inputRegPortC & (1 << COL_2)) == 0)
		{
			delay(300000);
			sendLCDData(0x38);
			printf("8\n");

		} else if((inputRegPortC & (1 << COL_3)) == 0)
		{
			delay(300000);
			sendLCDData(0x39);
			printf("9\n");

		}else if((inputRegPortC & (1 << COL_4)) == 0)
		{
			delay(300000);
			sendLCDData(0x43);
			printf("C\n");
		}


//		Keep ROWS (PORT E) in HIGH mode (PE2,PE3,PE4,PE5)
		outputRegPortE = outputRegPortE | 0x3C;

//		Make R4 (PE5) LOW STATE
		outputRegPortE = outputRegPortE & ~(1 << ROW_4);

//
		if((inputRegPortC & (1 << COL_1)) == 0){
			delay(300000);
			resetDisplay();
			printf("*\n");

		} else if((inputRegPortC & (1 << COL_2)) == 0)
		{
			delay(300000);
			sendLCDData(0x30);
			printf("0\n");

		} else if((inputRegPortC & (1 << COL_3)) == 0)
		{
			delay(300000);
			sendLCDData(0x23);
			printf("#\n");

		}else if((inputRegPortC & (1 << COL_4)) == 0)
		{
			delay(300000);
			sendLCDData(0x44);
			printf("D\n");
		}

	};

}
