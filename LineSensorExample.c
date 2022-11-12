/* This code outputs sensor values into PuttY */

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


/* These function declarations are defined in the startup.s assembly file for managing interrupts. */

void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

int main(void) {

    PLLInit(BUS_80_MHZ);
    DisableInterrupts();
    DelayInit();

    UARTConfig_t uart_conf = {
        .module=UART_MODULE_0,
        .baudrate=UART_BAUD_9600,
        .dataLength=UART_BITS_8,
        .isFIFODisabled=false,
        .isTwoStopBits=false,
        .parity=UART_PARITY_DISABLED,
        .isLoopback=false
    };
    UART_t uart = UARTInit(uart_conf);

    LineSensorConfig_t config = {
        .pins={AIN1, AIN2, AIN3, AIN4, AIN5, AIN10, AIN11, AIN8},
        .numPins=8,
    };

    LineSensor_t sensor = LineSensorInit(config);

    GPIOConfig_t LED1 = {PIN_F1, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
    GPIOInit(LED1);

    GPIOConfig_t LED2 = {PIN_F2, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
    GPIOInit(LED2);

    GPIOConfig_t LED3 = {PIN_F3, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
    GPIOInit(LED3);

    EnableInterrupts();

    PWM_t servo = ServoInit(M0_PB6);
    PWM_t servo2 = ServoInit(M0_PB7);
    ServoStop(servo);
    ServoStop(servo2);


    /* Main loop */
    while(1) {

        LineSensorGetBoolArray(&sensor, 2000);

        uint8_t sum = 0;
        uint8_t count = 0;
        uint8_t i;

        for (i = 0; i < 8; ++i) {
            if (!sensor.values[i]) {
                sum += i + 1;
                count++;
            }
        }

        float avg = (float)sum/(float)count;

        uint8_t msg[100] = { '\0' };
        snprintf(msg, 100,
            "%i %i %i %i %i %i %i %i %f\r\n",
            sensor.values[0],
            sensor.values[1],
            sensor.values[2],
            sensor.values[3],
            sensor.values[4],
            sensor.values[5],
            sensor.values[6],
            sensor.values[7],
            avg);
        UARTSend(uart, msg, 100);


    }
}
