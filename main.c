

/**
 * @file main.c
 * @author Jiwoo Park (jiwooo.park04@gmail.com)
 * @brief Main driver for the practice
 * @version 1.0
 * @date 2022-10-02
 * @copyright Copyright (c) 2021
 */

/** General imports. **/
#include <stdio.h>
#include <stdlib.h>

/** Device specific imports. **/
#include <lib/PLL/PLL.h>
#include <lib/GPIO/GPIO.h>
#include <lib/PWM/PWM.h>
#include <lib/Timer/Timer.h>
#include <raslib/Servo/Servo.h>
#include <raslib/LineSensor/LineSensor.h>
#include <lib/ADC/ADC.h>

void EnableInterrupts(void);
void DisableInterrupts(void);
void WaitForInterrupt(void);


int main(void)
{
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();

    DelayInit();

    // Configure Pins and their LEDs.
    /* Red onboard LED. */
    GPIOConfig_t PF1Config = {
          .pin=PIN_F1,
          .pull=GPIO_PULL_DOWN,
          .isOutput=true
    };
    /* Blue onboard LED. */
    GPIOConfig_t PF2Config = {
              .pin=PIN_F2,
              .pull=GPIO_PULL_DOWN,
              .isOutput=true
    };
    /* Green onboard LED. */
    GPIOConfig_t PF3Config = {
              .pin=PIN_F3,
              .pull=GPIO_PULL_DOWN,
              .isOutput=true
    };


    // Initialize the LED
    GPIOInit(PF1Config);
    GPIOInit(PF2Config);
    GPIOInit(PF3Config);


    /* Initialize servos. */
    PWM_t servo = ServoInit(M0_PB6);
    PWM_t servo2 = ServoInit(M0_PB7);
    PWM_t axel = ServoInit(M0_PB5);

    EnableInterrupts();

    // Make R,G,B alternate every 0.5 second

    while(1) {
        /*
        GPIOSetBit(PIN_F1, true);
        DelayMillisec(500);
        GPIOSetBit(PIN_F1, false);

        GPIOSetBit(PIN_F2, true);
        DelayMillisec(500);
        GPIOSetBit(PIN_F2, false);

        GPIOSetBit(PIN_F3, true);
        DelayMillisec(500);
        GPIOSetBit(PIN_F3, false);
        */

        // WaitForInterrupt();


        /*GPIOSetBit(PIN_F1, 1);
        ServoSetSpeed(axel, 10);
        DelayMillisec(5000);*/


        //SERVO CODE

        /* Make the servo stall for 5 seconds. This should be not moving. */

        GPIOSetBit(PIN_F1, 0);
        GPIOSetBit(PIN_F2, 0);
        ServoSetSpeed(servo, 0);
        ServoSetSpeed(servo2, 0);

        DelayMillisec(5000);

        //Make the servo go forward for 5 seconds.

        GPIOSetBit(PIN_F1, 0);
        GPIOSetBit(PIN_F2, 1);
        ServoSetSpeed(servo, 20);
        ServoSetSpeed(servo2, 20);
        DelayMillisec(2500);

        ServoSetSpeed(servo, 100);
        ServoSetSpeed(servo2, 100);
        DelayMillisec(2500);
        ServoSetSpeed(servo, 20);
        ServoSetSpeed(servo2, 20);
        DelayMillisec(500);


       /* Make the servo go backward for 5 seconds. */
        //first servo

        GPIOSetBit(PIN_F1, 1);
        GPIOSetBit(PIN_F2, 0);
        ServoSetSpeed(servo, -20);
        ServoSetSpeed(servo2, -20);
        DelayMillisec(2500);
        ServoSetSpeed(servo, -100);
        ServoSetSpeed(servo2, -100);
        DelayMillisec(2500);
        ServoSetSpeed(servo, -20);
        ServoSetSpeed(servo2, -20);
        DelayMillisec(500);



        /* Play around with the servo speed values. Note from Servo.h that
           the bounds are [-100, 100]. Are there some speeds where it's not
           moving as expected, even with tuning? Plot this out and make a graph
           of expected speed vs actual speed! Is the graph linear or nonlinear!
           Your observations will be useful in controlling your robot to move in
           straight lines in exact distances. */



/*        // LINE SENSOR

        // Analog pins used: PE3, PE2, PE1, PE0, PD3, PD2, PD1, and PD0.

        LineSensorConfig_t config = {
             .pins={AIN0, AIN1, AIN2, AIN3, AIN4, AIN5, AIN6, AIN7},
             .numPins=8,
        };

         Initialization of ADC
        LineSensor_t sensor = LineSensorInit(config);

         All the LEDs have been configured & Initialized above.

         Main loop: read line sensor and get boolean array, turn on LEDs depending
               on values from boolean array.

        while(1) {

             Read from the line sensor.
            LineSensorGetIntArray(&sensor);

             Here, you should check your debugger to see what is inside the sensor
               values array!
               I am assuming it will give the value between [0, 4095]

             Read from the line sensor again, but this time using a threshold.
               This threshold corresponds to 2048 / 4095 * 3.3 V.
               (Which is bascially 1/2 of 3.3V)
               Threshold is for deciding high and low
               I am assuming it will give the value between [0,1]

            LineSensorGetBoolArray(&sensor, 2048);

             Go through 8 sensors and the sum of them tells which side
             Declared as 8 bits
            uint8_t avgSide = 0;
            uint8_t i;
            for (i = 0; i < 8; ++i) {
                avgSide += sensor.values[i] << i;
            }

             Turn on RED LED if sensor data is none across the board.
             FUCKED
            if (avgSide == 0) {
                GPIOSetBit(PIN_F1, 1);
                GPIOSetBit(PIN_F2, 0);
                GPIOSetBit(PIN_F3, 0);
            }

             Turn on GREEN LED if sensor data is tending towards the left side.
             * Servo needs to turn left -- increase right side servo
            else if (avgSide >= 0x10) {
                GPIOSetBit(PIN_F1, 0);
                GPIOSetBit(PIN_F2, 0);
                GPIOSetBit(PIN_F3, 1);
            }

             Turn on BLUE LED if sensor data is tending towards the right side.
             * Servo needs to turn right -- increase left side servo
            else {
                GPIOSetBit(PIN_F1, 0);
                GPIOSetBit(PIN_F2, 1);
                GPIOSetBit(PIN_F3, 0);
            }
        }


        // Configure UART, set font, print array values
        UARTConfig_t uart_config = {};
        UART_t uart = UARTInit(uart_config);
        uint8_t size1 = 14;
        uint8_t message1[] = "Hello world!\r";
        UARTSend(uart, message1, size1);

        char buffer[8];
        char* avgSide;

        // Store hex number in buffer using snprintf
        char j = snprintf(buffer, 8, "%i\n", avgSide);

        // Send out detection result to PUTTY
        UARTSend(uart, (uint8_t*)buffer, 80);*/

    }
}
