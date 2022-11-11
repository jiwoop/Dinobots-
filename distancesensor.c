/**
 * @file DistanceSensor.c
 * @author Dario Jimenez, Matthew Yu (matthewjkyu@gmail.com)
 * @brief Program that demonstrates the uses of the GP2Y0A60SZ distance sensor and its driver.
 * @version 0.1
 * @date 2021-09-28
 * @copyright Copyright (c) 2021
 * @note
 * Modify the value of "__MAIN__" on line 17 to choose which program to run:
 *
 * __MAIN__ = 0: demonstrates initialization and manual triggering of a distance sensor.
 * __MAIN__ = 1: demonstrates initialization and interrupt capability of a distance sensor.
 *
 * See raslib/DistanceSensor/DistanceSensor.h for wiring and power instructions.
 * Analog pins used: PE3. See lib/ADC/ADC.h for other AINx pins that can be used.
 */
#define __MAIN__ 0

/** General imports. */
#include <stdlib.h>

/** Device specific imports. */
#include <lib/PLL/PLL.h>
#include <lib/GPIO/GPIO.h>
#include <raslib/DistanceSensor/DistanceSensor.h>


/**
 * These function declarations are defined in the startup.s assembly file for
 * managing interrupts.
 */
void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

#if __MAIN__ == 0
int main(void) {
    /**
     * @brief This program demonstrates intializing a distance sensor on pin PE3
     * and reading from it manually on demand as both integer and boolean values.
     */
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();

        DistanceSensorConfig_t configFRONT = {
              .pin=AIN10,
              //.pin=AIN11
              //switch out pins since we are using AIN0 for linesensor;
          };
       DistanceSensor_t sensorFRONT = DistanceSensorInit(configFRONT);

//       //second sensor
//       DistanceSensorConfig_t configLEFT = {
//             .pin=AIN11,
//         };
//       DistanceSensor_t sensorLEFT = DistanceSensorInit(configLEFT);
//
//       //third sensor
//       DistanceSensorConfig_t configRIGHT = {
//             .pin=AIN9
//         };
//       DistanceSensor_t sensorRIGHT = DistanceSensorInit(configRIGHT);

    /* Initialize PF2 as a GPIO output. This is associated with the BLUE led on
       the TM4C. */
          //Blinky LED --> LED initialization
          GPIOConfig_t LED1 = {PIN_F1, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
          GPIOInit(LED1);

          GPIOConfig_t LED2 = {PIN_F2, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
          GPIOInit(LED2);

          GPIOConfig_t LED3 = {PIN_F3, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
          GPIOInit(LED3);

          GPIOConfig_t LED4 = {PIN_F4, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
          GPIOInit(LED4);


    EnableInterrupts();

    /* Main loop: read line sensor and get boolean value, turn on LEDs depending
       on values from boolean value. */
    while(1) {
        /* Read from the distance sensor. */
        DistanceSensorGetInt(&sensorFRONT);
//        DistanceSensorGetInt(&sensorLEFT);
//        DistanceSensorGetInt(&sensorRIGHT);



        /* Here, you should check your debugger to see what is inside the sensor
           values array! */

        /* Read from the distance sensor again, but this time using a threshold.
           This threshold corresponds to 2048 / 4095 * 3.3 V. */
        DistanceSensorGetBool(&sensorFRONT,2048);
//        DistanceSensorGetBool(&sensorLEFT,2048);
//        DistanceSensorGetBool(&sensorRIGHT,2048);



//        if (sensorLEFT.value == 1) {
//            GPIOSetBit(PIN_F1, 0);
//            GPIOSetBit(PIN_F2, 1);
//            GPIOSetBit(PIN_F3, 0);
//        }
//        else if (sensorLEFT.value == 0) {
//            GPIOSetBit(PIN_F1, 1);
//            GPIOSetBit(PIN_F2, 0);
//            GPIOSetBit(PIN_F3, 0);
//        }
        //        Turn on GREEN LED if the sensor value is more than set threshold

        if(sensorFRONT.value==1)
        {
                        GPIOSetBit(PIN_F1, 1);
                        GPIOSetBit(PIN_F2, 0);
                        GPIOSetBit(PIN_F3, 0);
                     // DelayMillisec(150);
        }
        else
        {
                        GPIOSetBit(PIN_F1, 0);
                        GPIOSetBit(PIN_F2, 0);
                        GPIOSetBit(PIN_F3, 0);
        }

//        else
//        {
//            GPIOSetBit(PIN_F1, 0);
//            GPIOSetBit(PIN_F2, 0);
//            GPIOSetBit(PIN_F3, 0);
//        }

//        if (sensorFRONT.value==1) {
//            GPIOSetBit(PIN_F1, 1);
//            GPIOSetBit(PIN_F2, 0);
//            GPIOSetBit(PIN_F3, 0);
//            if(sensorLEFT.value==1)
//            {
//                GPIOSetBit(PIN_F2, 1);
//            }
//            if(sensorRIGHT.value==1)
//            {
//                GPIOSetBit(PIN_F3, 1);
//
//            }
//        }
//
//    //   Turn on BLUE LED if the sensor value is less than the set threshold.
//        else {
//            GPIOSetBit(PIN_F1, 0);
//            GPIOSetBit(PIN_F2, 0);
//            GPIOSetBit(PIN_F3, 1);
//            if(sensorLEFT.value==1)
//            {
//                GPIOSetBit(PIN_F1, 1);
//            }
//            if(sensorRIGHT.value==1)
//            {
//                GPIOSetBit(PIN_F2, 1);
//
//            }

       //}
    }

}

#elif __MAIN__ == 1
int main(void) {
    /**
     * @brief This program demonstrates initializing a distance sensor on pin
     * PE3, and reading from the sensor automagically using an interrupt on
     * TIMER_0A. It reads at 50 Hz, and does not use thresholding, although it
     * can be enabled.
     */
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();

    DistanceSensorConfig_t config = {
        .pin=AIN0,
        .repeatFrequency=50,
        .isThresholded=false
    };

    DistanceSensor_t sensor = DistanceSensorInit(config);

    /* Initialize PF2 as a GPIO output. This is associated with the BLUE led on
       the TM4C. */
    GPIOConfig_t PF2Config = {
        .pin=PIN_F2,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };

    /* Initialize PF3 as a GPIO output. This is associated with the GREEN led on
       the TM4C. */
    GPIOConfig_t PF3Config = {
        PIN_F3,
        GPIO_PULL_DOWN,
        true
    };
    GPIOInit(PF2Config);
    GPIOInit(PF3Config);

    EnableInterrupts();

    /* Main loop. */
    while (1) {
        /* Turn on the blue LED if an analog value from sensor is below 2048
           (around 1.65 V). */
        if (sensor.value < 2048){
            GPIOSetBit(PIN_F2, 1); //set PF2 to high to turn on blue LED
            GPIOSetBit(PIN_F3, 0); //set PF3 to low to turn off green LED
        }

        /* Turn on green LED if an analog value from sensor is above or
           equal to 2048 (around 1.65 V). */
        else if (sensor.value >= 2048){
            GPIOSetBit(PIN_F2, 0); //set PF2 to low to turn off blue LED
            GPIOSetBit(PIN_F3, 1); //set PF3 to high to turn on gren LED
        }
    }
}
#endif
///**
// * main.c
// */
//#define __MAIN__ 0
//# include <stdlib.h>
//# include <stdio.h>
//# include <stdint.h>
//# include <stdbool.h>
//
//# include <lib/GPIO/GPIO.h>
//# include <lib/PLL/PLL.h>
//# include <lib/Timer/Timer.h>
//# include <raslib/Servo/Servo.h>
//# include <raslib/LineSensor/LineSensor.h>
//# include <lib/ADC/ADC.h>
//# include <lib/UART/UART.h>
//# include <raslib/DistanceSensor/DistanceSensor.h>
//
//
//
//void EnableInterrupts(void);
//void DisableInterrupts(void);
//void WaitForInterrupt(void);
//
////EnableInterrupts();
///**
// * main.c
// */
//#if __MAIN__ == 0
//int main(void)
//{
//    PLLInit(BUS_80_MHZ);
//    DisableInterrupts();
//    DelayInit();
//    //Code for the line sensor --> (Initialization)
//    //ADC pins for line sensor
//    //initialization of ADC
//
//    /* Configure a UART device for communication. */
//    UARTConfig_t config = {
//        .module=UART_MODULE_0,
//        .baudrate=UART_BAUD_9600,
//        .dataLength=UART_BITS_8,
//        .isFIFODisabled=false,
//        .isTwoStopBits=false,
//        .parity=UART_PARITY_DISABLED,
//        .isLoopback=false
//    };
//    UART_t uart = UARTInit(config);
//
//    LineSensorConfig_t configline = {
//         // .pins={AIN0, AIN1, AIN2, AIN3, AIN4, AIN5, AIN6, AIN7},
//         .pins={AIN1, AIN2, AIN3, AIN4, AIN5, AIN6, AIN7, AIN8},
//          .numPins=8
//       };
//    LineSensor_t sensor = LineSensorInit(configline);
//
//   //distance sensor FRONT initialization
//    DistanceSensorConfig_t configFRONT = {
//          .pin=AIN0,
//          //.pin=AIN11
//          //switch out pins since we are using AIN0 for linesensor;
//      };
//   DistanceSensor_t sensorFRONT = DistanceSensorInit(configFRONT);
//
//   //second sensor
//   DistanceSensorConfig_t configLEFT = {
//         .pin=AIN9,
//     };
//   DistanceSensor_t sensorLEFT = DistanceSensorInit(configLEFT);
//
//   //third sensor
//   DistanceSensorConfig_t configRIGHT = {
//         .pin=AIN10,
//     };
//   DistanceSensor_t sensorRIGHT = DistanceSensorInit(configRIGHT);
//
//
//
//
//
//   //Blinky LED --> LED initialization
//   GPIOConfig_t LED1 = {PIN_F1, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
//   GPIOInit(LED1);
//
//   GPIOConfig_t LED2 = {PIN_F2, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
//   GPIOInit(LED2);
//
//   GPIOConfig_t LED3 = {PIN_F3, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
//   GPIOInit(LED3);
//
//   GPIOConfig_t LED4 = {PIN_F4, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
//   GPIOInit(LED4);
//
//
//
//
//
//    uint8_t size1 = 14;
//    uint8_t message1[] = "Hello world!\r";
//    UARTSend(uart, message1, size1);
//
//    EnableInterrupts();
//    //servo initialization
//    PWM_t servo = ServoInit(M0_PB6);
//    PWM_t servo2 = ServoInit(M0_PB7);
//    ServoStop(servo);
//    ServoStop(servo2);
//
//    while(1) {
//
//
//                        //FRONT SENSOR CODE
//                            if (sensorFRONT.value < 2048){
//                                GPIOSetBit(PIN_F2, 1); //set PF2 to high to turn on blue LED
//                                GPIOSetBit(PIN_F3, 0); //set PF3 to low to turn off green LED
//                                DelayMillisec(150);
//
//
//                                if(sensorLEFT.value<2048)
//                                {
//                                ServoSetSpeed(servo, 30);
//                                ServoSetSpeed(servo2,5);
//                                DelayMillisec(500);
//                                ServoStop(servo);
//                                ServoStop(servo2);
//                                DelayMillisec(150);
//                                }
//                                else if(sensorRIGHT.value<2048)
//                                {
//                                    ServoSetSpeed(servo, 5);
//                                    ServoSetSpeed(servo2,30);
//                                    DelayMillisec(500);
//                                    ServoStop(servo);
//                                    ServoStop(servo2);
//                                    DelayMillisec(150);
//                                }
//                                else
//                                {
//                                    ServoSetSpeed(servo, 30);
//                                    ServoSetSpeed(servo2,5);
//                                    DelayMillisec(500);
//                                    ServoStop(servo);
//                                    ServoStop(servo2);
//                                    DelayMillisec(150);
//
//                                    if(sensorFRONT.value < 2048)
//                                    {
//                                        ServoSetSpeed(servo, 5);
//                                        ServoSetSpeed(servo2,30);
//                                        DelayMillisec(500);
//                                        ServoStop(servo);
//                                        ServoStop(servo2);
//                                        DelayMillisec(150);
//                                    }
//
//                                }
//                            }
//
//                            // Turn on green LED if an analog value from sensor is above or
//                            // equal to 2048 (around 1.65 V).
//                            else if (sensorFRONT.value >= 2048){
//                                GPIOSetBit(PIN_F2, 0); //set PF2 to low to turn off blue LED
//                                GPIOSetBit(PIN_F3, 1); //set PF3 to high to turn on green LED
//                                DelayMillisec(150);
//
//                                ServoSetSpeed(servo, 30);
//                                ServoSetSpeed(servo2,30);
//                                DelayMillisec(200);
//
//                                ServoStop(servo);
//                                ServoStop(servo2);
//                                DelayMillisec(150);
//                            }
//                            else//enter the fucked state
//                            {
//                                GPIOSetBit(PIN_F1, 1);//set PF2 to low to turn off blue LED
//                                GPIOSetBit(PIN_F2, 0); //set PF2 to low to turn off blue LED
//                                GPIOSetBit(PIN_F3, 0); //set PF3 to high to turn on green LED
//                                DelayMillisec(150);
//
//
//                                ServoSetSpeed(servo, 30);
//                                ServoSetSpeed(servo2,30);
//                                DelayMillisec(200);
//
//                                ServoStop(servo);
//                                ServoStop(servo2);
//                                DelayMillisec(150);
//
//                                GPIOSetBit(PIN_F1, 0);//set PF2 to low to turn off blue LED
//                                GPIOSetBit(PIN_F2, 0); //set PF2 to low to turn off blue LED
//                                GPIOSetBit(PIN_F3, 0); //set PF3 to high to turn on green LED
//                                DelayMillisec(150);
//
//
//                            }
//
//
//
//
//
//
//        }
//
//}
//#endif
