/**
* main.c
*/
#define __MAIN__ 0
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



void EnableInterrupts(void);
void DisableInterrupts(void);
void WaitForInterrupt(void);

//EnableInterrupts();
/**
* main.c
*/
#if __MAIN__ == 0
int main(void)
{
   PLLInit(BUS_80_MHZ);
   DisableInterrupts();
   DelayInit();
   //Code for the line sensor --> (Initialization)
   //ADC pins for line sensor
   //initialization of ADC

   /* Configure a UART device for communication. */
   UARTConfig_t config = {
       .module=UART_MODULE_0,
       .baudrate=UART_BAUD_9600,
       .dataLength=UART_BITS_8,
       .isFIFODisabled=false,
       .isTwoStopBits=false,
       .parity=UART_PARITY_DISABLED,
       .isLoopback=false
   };
   UART_t uart = UARTInit(config);

   LineSensorConfig_t configline = {
        // .pins={AIN0, AIN1, AIN2, AIN3, AIN4, AIN5, AIN6, AIN7},
        .pins={AIN1, AIN2, AIN3, AIN4, AIN5, AIN6, AIN7, AIN8},
         .numPins=8
      };
   LineSensor_t sensor = LineSensorInit(configline);

  //distance sensor FRONT initialization
   DistanceSensorConfig_t configdis1 = {
         .pin=AIN0,
         //.pin=AIN11
         //switch out pins since we are using AIN0 for linesensor;
     };
  DistanceSensor_t sensorFRONT = DistanceSensorInit(configdis1);

  //second sensor
  DistanceSensorConfig_t configdis2 = {
        .pin=AIN9,
    };
  DistanceSensor_t sensorLEFT = DistanceSensorInit(configdis2);

  //third sensor
  DistanceSensorConfig_t configdis3 = {
        .pin=AIN10,
    };
  DistanceSensor_t sensorRIGHT = DistanceSensorInit(configdis3);





  //Blinky LED --> LED initialization
  GPIOConfig_t LED1 = {PIN_F1, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
  GPIOInit(LED1);

  GPIOConfig_t LED2 = {PIN_F2, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
  GPIOInit(LED2);

  GPIOConfig_t LED3 = {PIN_F3, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
  GPIOInit(LED3);

  GPIOConfig_t LED4 = {PIN_F4, GPIO_PULL_DOWN, true, 0, false, GPIO_DRIVE_2MA, false};
  GPIOInit(LED4);


   //servo initialization
   PWM_t servo = ServoInit(M0_PB6);
   PWM_t servo2 = ServoInit(M0_PB7);
   PWM_t axelServo = ServoInit(M0_PB5);

   uint8_t size1 = 14;
   uint8_t message1[] = "Hello world!\r";
   UARTSend(uart, message1, size1);

   EnableInterrupts();
   while(1) {

               

                               GPIOSetBit(PIN_F1, true);
                                DelayMillisec(500);
                                GPIOSetBit(PIN_F1, false);
                                DelayMillisec(500);
                                GPIOSetBit(PIN_F2, true);
                                DelayMillisec(500);
                                GPIOSetBit(PIN_F2, false);
                                DelayMillisec(500);
                                GPIOSetBit(PIN_F3, true);
                                DelayMillisec(500);
                                GPIOSetBit(PIN_F3, false);
                                DelayMillisec(500);
                                GPIOSetBit(PIN_F4, true);
                                DelayMillisec(500);
                                GPIOSetBit(PIN_F4, false);
                                DelayMillisec(500);




       }

}
#endif
