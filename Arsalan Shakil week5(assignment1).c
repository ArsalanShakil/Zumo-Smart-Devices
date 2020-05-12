/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 1
// MQTT test
void zmain(void)
{
    TickType_t start;	//variable to save the value of start time
    TickType_t end;		//variable to save the value of end time
    int ctr = 0;		//counter to count the number of cycles
    int n = 0;			//variable to save the value of end - start

    printf("\nBoot\n");	// prompt that the code is starting
   

    while(SW1_Read() == 0){		// wait for the button to be pressed  
                vTaskDelay(10);  // delay task  
            }
    
    start = xTaskGetTickCount();	// start the timer and the save the value to variable

    while(true)		// start the infinite loop
	 {

        if(SW1_Read() == 0)		// check if the the button is pressed or not
		 {

			end = xTaskGetTickCount();		// end the timer and the save the value to variable
            n = end - start;				// save the difference of start and end into variable
            vTaskDelay(250);				// delay task 
            print_mqtt("Zumo01/button", "Time:%d", n); // publish to the broker and the time interval between the button presses

            while(SW1_Read() == 0){			// start the loop again if the button is pressed again
                vTaskDelay(10);				// delay task 
            }

        start = xTaskGetTickCount();	//start the counter again and the save the value to variable

        }

        vTaskDelay(100);				// delay task 
        ctr++;					//increment the value of ctr counter when one cycle is complete
    
    }   

}   
#endif

/* [] END OF FILE */
