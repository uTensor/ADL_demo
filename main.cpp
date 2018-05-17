/* Includes */
#include "mbed.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"
#include "FATFileSystem.h"
#include "F413ZH_SD_BlockDevice.h"
#include "MMA7660FC.h"
#include "SensorQueue.hpp"

#define ADDR_MMA7660 0x98                   // I2C SLAVE ADDR MMA7660FC
MMA7660FC Acc(I2C_SDA, I2C_SCL, ADDR_MMA7660);      //sda, scl, Addr
float G_VALUE[64] = { 0, 0.047, 0.094, 0.141, 0.188, 0.234, 0.281, 0.328,
                      0.375, 0.422, 0.469, 0.516, 0.563, 0.609, 0.656, 0.703,
                      0.750, 0.797, 0.844, 0.891, 0.938, 0.984, 1.031, 1.078,
                      1.125, 1.172, 1.219, 1.266, 1.313, 1.359, 1.406, 1.453,
                      -1.500, -1.453, -1.406, -1.359, -1.313, -1.266, -1.219,
                      -1.172, -1.125, -1.078, -1.031, -0.984, -0.938, -0.891,
                      -0.844, -0.797, -0.750, -0.703, -0.656, -0.609, -0.563,
                      -0.516, -0.469, -0.422, -0.375, -0.328, -0.281, -0.234,
                      -0.188, -0.141, -0.094, -0.047};

Serial pc(USBTX, USBRX, 115200);
// F413ZH_SD_BlockDevice bd;
// FATFileSystem fs("fs");

InterruptIn button(USER_BUTTON);

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

  Acc.init();                                                     // Initialization
  pc.printf("Value reg 0x06: %#x\n", Acc.read_reg(0x06));         // Test the correct value of the register 0x06
  pc.printf("Value reg 0x08: %#x\n", Acc.read_reg(0x08));         // Test the correct value of the register 0x08
  pc.printf("Value reg 0x07: %#x\n\r", Acc.read_reg(0x07));       // Test the correct value of the register 0x07
  
 
  while(1){
    float x=0, y=0, z=0;
    
    Acc.read_Tilt(&x, &y, &z);                                  // Read the acceleration                    
    pc.printf("Tilt x: %2.2f degree \n", x);                    // Print the tilt orientation of the X axis
    pc.printf("Tilt y: %2.2f degree \n", y);                    // Print the tilt orientation of the Y axis
    pc.printf("Tilt z: %2.2f degree \n", z);                    // Print the tilt orientation of the Z axis
 
    wait_ms(100);
 
    pc.printf("x: %1.3f g \n", G_VALUE[Acc.read_x()]);          // Print the X axis acceleration
    pc.printf("y: %1.3f g \n", G_VALUE[Acc.read_y()]);          // Print the Y axis acceleration
    pc.printf("z: %1.3f g \n", G_VALUE[Acc.read_z()]);          // Print the Z axis acceleration
    pc.printf("\n");
    wait(1);
  }
}