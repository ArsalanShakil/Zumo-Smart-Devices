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
//reflectance
void zmain(void)
{
    struct sensors_ dig;
    while (SW1_Read())
        vTaskDelay(100);
    IR_Start();
    reflectance_start();
    reflectance_set_threshold(13000, 13000, 13000, 13000, 13000, 13000); // set center sensor threshold to 11000 and others to 9000
    motor_start();
    motor_forward(0,0);
    bool run = true;
    while (run)
    {
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);

        if (dig.l3 == 1 && dig.r3 == 1)
        {
            motor_forward(0, 0);
            run = false;
            ;
        }
        else
        {
            motor_forward(50, 60);
        }
      vTaskDelay(100);
    }
    IR_wait(); // wait for IR command
    run = true;
    int cont = 0;
 
    while (run)
    {
       
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);
        vTaskDelay(100);
        if (dig.l3 == 1 && dig.r3 == 1)
        {
            motor_forward(50, 60);
            cont++; 
        }
        else if(cont >= 10 ){
            motor_forward(0,0);
            motor_stop();
            run = false;
        }
        else
        {
            motor_forward(50, 60);
            
        }
  
    }
     while (true)
    {
        vTaskDelay(50);
    }
}
#endif

/* [] END OF FILE */
