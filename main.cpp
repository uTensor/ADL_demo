/* Includes */
#include "mbed.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"
#include "FATFileSystem.h"
#include "F413ZH_SD_BlockDevice.h"
#include "SensorQueue.hpp"

Serial pc(USBTX, USBRX, 115200);
// F413ZH_SD_BlockDevice bd;
// FATFileSystem fs("fs");

InterruptIn button(USER_BUTTON);

/* Simple main function */
int main() {

  pc.printf("test start\r\n");
  
  BSP_LCD_Init();

  /* Touchscreen initialization */
  if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
      printf("BSP_TS_Init error\n");
  }

  /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

  SensorQueue_Test();
  printf("test finished\r\n");
  exit(0);
 
  // while(1) {
  //   printf("\r\n");

  //   acc_gyro.get_x_axes(axes);
  //   printf("LSM6DSL [acc/mg]:        %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
  //   acc_gyro.get_g_axes(axes);
  //   printf("LSM6DSL [gyro/mdps]:     %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

  //   printf("\033[8A");

  //   wait(0.5);
  // }
}