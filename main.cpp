/* Includes */
#include "mbed.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"
#include "FATFileSystem.h"
#include "F413ZH_SD_BlockDevice.h"
#include "MMA7660.h"
#include "SensorQueue.hpp"

Serial pc(USBTX, USBRX, 115200);
// F413ZH_SD_BlockDevice bd;
// FATFileSystem fs("fs");

InterruptIn button(USER_BUTTON);
MMA7660 accelemeter;

int main() {

  pc.printf("test start\r\n");
  int8_t x, y, z;
  float ax,ay,az;

  BSP_LCD_Init();

  /* Touchscreen initialization */
  if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
      printf("BSP_TS_Init error\n");
  }

  /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

  SensorQueue_Test();
  printf("test finished\r\n");

  accelemeter.init();
 
  while(1){
    accelemeter.getXYZ(&x,&y,&z);
    printf("X=%d, Y=%d, Z=%d, ", x, y, z);
    
    accelemeter.getAcceleration(&ax,&ay,&az);
    printf("Accleration of X=%2.2fg, Y=%2.2fg, Z=%2.2fg\n\r",ax,ay,az);
    wait(0.5);
  }
}