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
#include <stdlib.h>
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
 * @details  * Enable global interrupt since Zumo library uses interrupts. *<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 1
//Maze Code 
void turn_right();  // for sharp 90 degree turn for right side 
void turn_right_2();  // for sharp 90 degree turn for right side 
void turn_right_180(); // for sharp 180 degree turn for right side 
void turn_left(); 	// for sharp 90 degree turn for left side 
void turn_left_2();  // for sharp 90 degree turn for right side 
void turn_left_180(); // for sharp 180 degree turn for left side 

void zmain(void)
{
   
    struct sensors_dig;
    struct sensors_ dig;
    Ultra_Start();  	// enable Ultra sencor controller 
    motor_start(); 		// enable motor controller
    IR_Start();  		// enable IR controller
    reflectance_start();  	// enable reflectance controller 
    bool startline = true; 	// Variable for breaking while loop that moving to starting line 
	
    int left = 0 , right = 0; 		//  Variable for decide which side the robot is  
	
    TickType_t start, end, time = 0; // Variable for sating, ending , total time 
	
    int lines = -1, cordi = 0, turn=0;  		//  Variable for counting lines and finding crdinate of robot 
    
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
	
   // while(SW1_Read() == 1) { vTaskDelay(5);}  // delay for reset button
    print_mqtt("Zumo04/Ready", " Maze");
    IR_wait();  // wait for IR command
    // Moving forward while searching for the signal
    motor_forward(60,0);
    while (startline)
    {
        reflectance_digital(&dig);
        if(dig.l3 == 1 && dig.r3 == 1 && dig.r2 == 1 && dig.l2 == 1 && dig.r1 == 1 && dig.l1 == 1){
            motor_forward(0,0);
        
            startline = false;
        }
    
    }
    
    IR_wait();  // wait for IR command
    start =	xTaskGetTickCount();  // Start time for total run
    print_mqtt("Zumo04/Start", " %d",start); // Print mqtt star
    int d=0;
    int c=0;

    bool direction = true;
    while(true)
    {   
        reflectance_digital(&dig);
        
        if(dig.l3 == 0 && dig.r3 == 1 && dig.r2 == 0 && dig.l2 == 1 && dig.r1 == 1 && dig.l1 == 1){
            motor_forward(0,0);
            motor_forward(60,750);
            turn_left();  // turn left side 
            left++;
        }
        if(dig.l3 == 1 && dig.r3 == 0 && dig.r2 == 1 && dig.l2 == 1 && dig.r1 == 1 && dig.l1 == 1){
            motor_forward(0,0);
            motor_forward(60,750);
            turn_right();  // turn left side 90
            right++;
        }
        if(dig.l3 == 1 && dig.r3 == 1 && dig.r2 == 1 && dig.l2 == 1 && dig.r1 == 1 && dig.l1 == 1){
           reflectance_digital(&dig); 
     
            motor_forward(0,0);
            motor_forward(60,750);
            if(left == 1){
            d =Ultra_GetDistance();
           
                motor_forward(0,0);    	// set speed to zero to stop motors
                motor_turn_180_right(200,200,131);   // enable right 90 degree turn
                motor_forward(0,0);     // set speed to zero to stop motors
                int d =Ultra_GetDistance();
                if(d <= 20 ){
                    turn_left();             
                }else{
                   left--;
                }
                
               
            }
            if(right == 1){
                motor_forward(0,0);  // set speed to zero to stop motors
                motor_turn_180_left(200,200,131);   // enable right 90 degree turn
                motor_forward(0,0);     // set speed to zero to stop motors
                int d =Ultra_GetDistance();
                if(d <= 20 ){
                    turn_right();             
                }
                else{
                    right--;
                }
                
            }
            
            
            while(d <= 20){
                motor_forward(0,1000);
               
            
                direction = (rand() > .5); 	// random true or false  
                	
				lines++;  //count number of lines  
                /* mqtt was added after testing so code might glitch
                */
                if(lines >=0){
                    print_mqtt("Zumo04/position ", " %d %d", cordi,lines); // Print mqtt 
                }
                if(direction == true){
                    turn_left();  // turn left side 
                    left++;
                    c++;
                }
                else{
                    turn_right();  // turn left side 90
                    right++;
                    c--;
                }
                d =Ultra_GetDistance();
                while((left == 1 || right ==1) && d <= 20 ){
                    if(left == 1){
                        turn_right();  // turn left side 90
                        right--;
                    }else{
                        turn_left();  // turn left side 
                        left--;
                    }
                }
                motor_forward(0,1000);
                d =Ultra_GetDistance();
                
            }
            
            
            reflectance_digital(&dig); 
            if(dig.l3 == 0 && dig.r3 == 0 && dig.r2 == 0 && dig.l2 == 0 && dig.r1 == 0 && dig.l1 == 0){
                turn_right_180();
                if(right == 1){
                    right--;
                    left++;
                }
                else{
                    right++;
                    left--;
                }
                
                
            }
            
            
        }
        else{
            motor_forward(60,0);
        }
            
       reflectance_digital(&dig);
    }
    /* mqtt was added after testing so code might glitch
    */
     while(lines >= 14){
 
            motor_turn(75,73,0);           // moving forward
            vTaskDelay(600);  // when its in final line move forward a bit
			motor_forward(0,0);        	// set speed to zero to stop motors
            end = xTaskGetTickCount();
            print_mqtt("Zumo04/Stop  ", " %d",end ); // print mqtt stop time 
            time = end  - start ; // counting total time 
            print_mqtt("Zumo04/time  ", " %d",time ); // print mqtt total time 
			IR_wait();// wait for IR 
        }
        
   

    
}

void turn_right(){
   
    Ultra_Start();  	// enable Ultra sencor controller 
    motor_start();  	// enable motor controller
	motor_forward(0,0);    	// set speed to zero to stop motors
    motor_turn_180_right(200,200,131);   // enable right 90 degree turn
    motor_forward(0,0);     // set speed to zero to stop motors
    int d =Ultra_GetDistance();
    if(d <= 20 ){
        turn_left();             
    }
    
}

void turn_right_2(){
    motor_start();  	// enable motor controller
	motor_forward(0,0);    	// set speed to zero to stop motors
    motor_forward(100,150);  // set speed to zero to stop motors
    motor_turn_180_right(200,200,130);   // enable right 90 degree turn
    
    motor_forward(0,0);     // set speed to zero to stop motors
}

void turn_right_180(){
   
    
    Ultra_Start();  	// enable Ultra sencor controller 
    motor_start();  		// enable motor controller
	motor_forward(0,0);     // set speed to zero to stop motors
    motor_turn_180_right(200,200,262);   // enable right 180 degree turn
    motor_forward(0,0);      // set speed to zero to stop motors
    int d =Ultra_GetDistance();
    if(d <= 20 ){
        turn_left();             
    }
}

void turn_left(){
    
    Ultra_Start();  	// enable Ultra sencor controller 
    motor_start();  	// enable motor controller
	motor_forward(0,0);  // set speed to zero to stop motors
    motor_turn_180_left(200,200,131);   // enable right 90 degree turn
    motor_forward(0,0);     // set speed to zero to stop motors
    int d =Ultra_GetDistance();
    if(d <= 20 ){
        turn_right();             
    }
}

void turn_left_2(){
    motor_start();  	// enable motor controller
	motor_forward(0,0);  // set speed to zero to stop motors
    motor_turn_180_left(200,200,130);   // enable right 90 degree turn
    motor_forward(0,0);     // set speed to zero to stop motors
}

void turn_left_180(){
	motor_start();  	// enable motor controller
	motor_forward(0,0);     // set speed to zero to stop motors
    motor_turn_180_left(200,200,262);   // enable right 180 degree turn
    motor_forward(0,0);     // set speed to zero to stop motors
}

#endif


/* [] END OF FILE */
