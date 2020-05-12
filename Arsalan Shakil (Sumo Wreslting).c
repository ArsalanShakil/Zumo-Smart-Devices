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
//SUMO WRESTLING 
void zmain(void)
{
    // initializing variables
    Ultra_Start();       // Ultra Sonic Start function
    struct sensors_ dig; // variable for the reflectance sensor
    TickType_t start;	//variable to save the value of start time
    TickType_t end;		//variable to save the value of end time
    int n = 0;			//variable to save the value of end - start
 
    
    // start the code if the user button is pressed
    while (SW1_Read()){
        
        vTaskDelay(100);    
    }
    
    // getting the robot ready to function properly 
    IR_Start();           // get the IR signal reicver ready
    reflectance_start();  //start the reflectance sensors
    reflectance_set_threshold(13000, 13000, 13000, 13000, 13000, 13000); // set center sensor threshold to 11000 and others to 9000
    motor_start();        // get the motors ready
    motor_forward(0,0);   //set the speed to 0 so robot doesnt move forward
    bool run = true;      // set variable to true to start the while loop
    
    // Moving forward while searching for the signal
    while (run)
    {
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);
        
        // Search circle 
        if (dig.l3 == 1 || dig.r3 == 1) //run if the circle is found
        {
            
            motor_forward(0, 0); //stop and set the speed to 0
            run = false; // break the loop
            
        }
        // Move forward if the circle is not found
        else // if circle not found
        {
            motor_forward(50, 60); // move forward
        }

      vTaskDelay(100);
    
    }
    
    //wait near circle
    print_mqtt("Zumo04/ready", "zumo"); //Print Mqtt at Ready
    IR_wait();   // wait for IR command
    
    //pass the circle line
    motor_forward(90, 100);
    
    //mqtt of the start
    start = xTaskGetTickCount();	// save the time at the start 
    print_mqtt("Zumo04/start", "%d", start);  // Print Mqtt at start
    run = true; // to start the while loop
    

    // start the loop 
    while (run)
    {
       
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);
        vTaskDelay(100);
        int d = Ultra_GetDistance();    //check the distance between the robot and the obstacle
        
        
        //detect the end of the cirlce and trun
        if (dig.l1 == 1 || dig.l2 == 1 || dig.l3 == 1||    // check if the edge of the cirlce is near
        dig.r1 == 1 || dig.r2 == 1 || dig.r3 == 1) 
        {
            
            motor_backward(70, 600);    //move back
            motor_forward(0, 0);        //stop
            motor_turn(26,0,600);       //turn right
            motor_forward(50,200);      //move forward
            
        }
        
        //detect the edge of the obstacle and turn
        else if(d<=5)   //  check if the distance between the robot and the obstacle is 5 or less.
        {

           //mqtt for the obstacle turn
          end = xTaskGetTickCount();		// get the time when the robot turns due an obstacle 
          n = end - start;				    // save the time between start of the robot and the turn 
          print_mqtt("Zumo04/obstacle", "%d", n); // Print Mqtt at the obstacle turn

		  //move back and trun
          motor_backward(70,600);             //move back
          motor_turn(26,0,600);               //turn right
          motor_forward(50,200);              //move forward
          d = Ultra_GetDistance();            //check the distance between the robot and the obstacle
          
        }
        
        //stop when the user button pressed
        else if (SW1_Read()== 0)
        {
            
            motor_forward(0,0);  //stop and set the speed to 0
            
            //mqtt for the end
            end = xTaskGetTickCount();		// get the time when the robot stops
            n = end - start;				// save the difference of start and end into variable
            print_mqtt("Zumo04/stop", "%d", end); // Print Mqtt at end
            print_mqtt("Zumo04/time", "%d", n); // Print Mqtt the runtime between the start and end
            break;  // end the loop
            
            //stop the robot
            motor_stop();
             while(true){
                vTaskDelay(100);
            }
            
        }
        else // if none of the condtions above are true keep moving forward
        {
          motor_forward(50,200); //move forward
        }

    }
    
    
    while(true){
        vTaskDelay(100);
    }
   
 }   
#endif

/* [] END OF FILE */
