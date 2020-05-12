#if 1
//motor
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors
    
    while(SW1_Read()) {
        vTaskDelay(10);
    }
    
    //vTaskDelay(3000);
    
       // the straight track
    motor_forward(100,3225);     // moving forward
    motor_turn(36,10,2000);     // turn
    
    // the straight track on the left-side
    motor_forward(100,2355);     // moving forward
    motor_turn(36,10,2030);     // turn
    
    // the straight track coming back
    motor_forward(100,2600);     // moving forward
    motor_turn(26,00,2000);     // turn
	motor_turn(20,00,200);     // turn
    
    // the curved track
    motor_forward(100,100);     // moving forward
    motor_turn(35,5,100);     // turn
    motor_forward(100,300);     // moving forward
    motor_turn(30,5,300);     // turn
    motor_forward(100,200);     // moving forward
    motor_turn(15,5,300);     // turn
    motor_forward(100,200);     // moving forward
    motor_turn(25,5,300);     // turn
    motor_forward(100,200);     // moving forward
    motor_turn(15,5,300);     // turn
    motor_forward(100,100);     // moving forward
    motor_turn(15,5,300);     // turn
    motor_forward(100,100);     // moving forward
    motor_turn(15,5,300);     // turn
    motor_forward(100,100);     // moving forward
    motor_turn(20,5,300);     // turn
    motor_forward(100,100);     // moving forward
    motor_turn(15,5,300);     // turn
    motor_forward(100,100);     // moving forward
    motor_turn(15,5,300);     // turn
    motor_forward(100,100);     // moving forward
    motor_turn(15,5,300);     // turn
    motor_forward(100,200);     // moving forward
    motor_turn(10,5,300);     // turn
    motor_forward(100,200);     // moving forward
    motor_turn(20,5,300);     // turn
    motor_forward(100,200);     // moving forward
    motor_forward(100,500);     // moving forward
   
     
    motor_forward(0,0);         // stop motors

    motor_stop();               // disable motor controller
    

    
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif

/* [] END OF FILE */
