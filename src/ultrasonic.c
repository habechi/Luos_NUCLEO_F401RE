#include "ultrasonic.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

const float speedOfSound = 0.0343/2;
float distance;
char uartBuf[100];

	uint32_t numTicks = 0;


void usDelay(uint32_t uSec)
{
	if(uSec < 2) uSec = 2;
	usTIM->ARR = uSec - 1; 	/*sets the value in the auto-reload register*/
	usTIM->EGR = 1; 			/*Re-initialises the timer*/
	usTIM->SR &= ~1; 		//Resets the flag
	usTIM->CR1 |= 1; 		//Enables the counter
	while((usTIM->SR&0x0001) != 1);
	usTIM->SR &= ~(0x0001);
}

float calculerrrr(void)
{
//Set TRIG to LOW for few uSec
		HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
		usDelay(3);
		
		//*** START Ultrasonic measure routine ***//
		//1. Output 10 usec TRIG
		HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
		usDelay(10);
		HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
		
		//2. Wait for ECHO pin rising edge
		while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET);
		
		//3. Start measuring ECHO pulse width in usec
		numTicks = 0;
		while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET)
		{
			numTicks++;
			usDelay(2); //2.8usec
		};
		
		//4. Estimate distance in cm
		distance = (numTicks + 0.0f)*2.8*speedOfSound;
		
		//5. Print to UART terminal for debugging
		//sprintf(uartBuf, "Distance (cm)  = %.1f\r\n", distance);
   /* char *tmpSign = (distance < 0) ? "-" : "";
    float tmpVal = (distance < 0) ? -distance : distance;
    int tmpInt1 = tmpVal;                  // Get the integer (678).
    float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
    int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).
    
    sprintf (uartBuf, "Distance (cm)  = %s%d.%04d\n", tmpSign, tmpInt1, tmpInt2);

		HAL_UART_Transmit(&huart2, (uint8_t *)uartBuf, strlen(uartBuf), 100);*/
    
		
		//HAL_Delay(1000);
		return distance;
		
 
}



