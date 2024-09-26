/** Device specific imports. */
# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>

# include <lib/GPIO/GPIO.h>
# include <lib/PLL/PLL.h>
# include <lib/Timer/Timer.h>
# include <raslib/Servo/Servo.h>
# include <raslib/LineSensor/LineSensor.h>
# include <lib/ADC/ADC.h>
# include <lib/UART/UART.h>
# include <raslib/DistanceSensor/DistanceSensor.h>



/**
 * These function declarations are defined in the startup.s assembly file for
 * managing interrupts.
 */
void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

int main(void) {
    /**
     * @brief This program demonstrates intializing a line sensor with 8 pins:
     * PE3, PE2, PE1, PE0, PD3, PD2, PD1, and PD0 and reading from it manually
     * on demand as both integer and boolean values.
     */
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();
    DelayInit();


    LineSensorConfig_t config = {
        .pins={AIN1, AIN2, AIN3, AIN4, AIN5, AIN6, AIN7, AIN8},
        .numPins=8,
    };

    /* Initialization of ADC */
    LineSensor_t sensor = LineSensorInit(config);

    GPIOConfig_t LED1 = {PIN_F1, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
    GPIOInit(LED1);

    GPIOConfig_t LED2 = {PIN_F2, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
    GPIOInit(LED2);

    GPIOConfig_t LED3 = {PIN_F3, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
    GPIOInit(LED3);

    GPIOConfig_t LED4 = {PIN_F4, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
    GPIOInit(LED4);



    EnableInterrupts();
    PWM_t servo = ServoInit(M0_PB6);
    PWM_t servo2 = ServoInit(M0_PB7);
    ServoStop(servo);
    ServoStop(servo2);


    /* Main loop: read line sensor and get boolean array, turn on LEDs depending
       on values from boolean array. */
    while(1) {
        /* Read from the line sensor. */
        LineSensorGetIntArray(&sensor);

        LineSensorGetBoolArray(&sensor, 2048);

        uint8_t avgSide = 0;
        uint8_t i;
        for (i = 0; i < 8; ++i) {
            avgSide += sensor.values[i] << i;
        }

        /* Turn on RED LED if sensor data is none across the board. */
        if (avgSide == 0) {
            GPIOSetBit(PIN_F1, 1);
            GPIOSetBit(PIN_F2, 0);
            GPIOSetBit(PIN_F3, 0);
            DelayMillisec(150);

           ServoSetSpeed(servo, -35);
           ServoSetSpeed(servo2,-35);
           DelayMillisec(300);


            ServoStop(servo);
            ServoStop(servo2);
            DelayMillisec(150);
        }
        /* Turn on GREEN LED if sensor data is tending towards the left side. */
        else if (avgSide >= 0x10) {
            GPIOSetBit(PIN_F1, 0);
            GPIOSetBit(PIN_F2, 0);
            GPIOSetBit(PIN_F3, 1);
            DelayMillisec(150);
            if(avgSide==0x18)
            {
                ServoSetSpeed(servo2, 30);
                ServoSetSpeed(servo, 30);
                DelayMillisec(100);
                ServoStop(servo);
                ServoStop(servo2);
                DelayMillisec(150);

            }
           ServoSetSpeed(servo2, 25);
           ServoSetSpeed(servo, 45);
           DelayMillisec(100);
           ServoStop(servo);
           ServoStop(servo2);
           DelayMillisec(150);

        }
        /* Turn on BLUE LED if sensor data is tending towards the right side. */
        else {
            GPIOSetBit(PIN_F1, 0);
            GPIOSetBit(PIN_F2, 1);
            GPIOSetBit(PIN_F3, 0);
            DelayMillisec(150);

           ServoSetSpeed(servo2, 40);
           ServoSetSpeed(servo, 25);
           DelayMillisec(150);
           ServoStop(servo);
           ServoStop(servo2);
           DelayMillisec(100);

        }
    }
}
