/* Includes */
#include "mbed.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_lcd.h"
#include "FATFileSystem.h"
#include "F413ZH_SD_BlockDevice.h"
#include "MMA7660FC.h"
#include "SensorQueue.hpp"

static const float G_VALUE[64] = { 0, 0.047, 0.094, 0.141, 0.188, 0.234, 0.281, 0.328,
                      0.375, 0.422, 0.469, 0.516, 0.563, 0.609, 0.656, 0.703,
                      0.750, 0.797, 0.844, 0.891, 0.938, 0.984, 1.031, 1.078,
                      1.125, 1.172, 1.219, 1.266, 1.313, 1.359, 1.406, 1.453,
                      -1.500, -1.453, -1.406, -1.359, -1.313, -1.266, -1.219,
                      -1.172, -1.125, -1.078, -1.031, -0.984, -0.938, -0.891,
                      -0.844, -0.797, -0.750, -0.703, -0.656, -0.609, -0.563,
                      -0.516, -0.469, -0.422, -0.375, -0.328, -0.281, -0.234,
                      -0.188, -0.141, -0.094, -0.047};

MMA7660FC Acc(I2C_SDA, I2C_SCL, ADDR_MMA7660);      //sda, scl, Addr

Serial pc(USBTX, USBRX, 115200);
// F413ZH_SD_BlockDevice bd;
// FATFileSystem fs("fs");

InterruptIn button(USER_BUTTON);

struct ACC_UINT8_VECTOR {
  uint8_t x;
  uint8_t y;
  uint8_t z;
};

SensorQueue<ACC_UINT8_VECTOR> buff(160, 32, 2);
ACC_UINT8_VECTOR sample;

void accTimerHandler(void) {
  static ACC_UINT8_VECTOR sample;
  sample.x = 0b111111 & (uint8_t) Acc.read_x();
  sample.y = 0b111111 & (uint8_t) Acc.read_y();
  sample.z = 0b111111 & (uint8_t) Acc.read_z();
  buff.append(sample);
}

void uTensorTrigger(void) {
  ACC_UINT8_VECTOR* tmp = (ACC_UINT8_VECTOR*) malloc(sizeof(ACC_UINT8_VECTOR) * 160 * 3);
  float data[160 * 3];
  
  buff.copyTo(tmp);

  for(uint8_t i = 0; i < 160; i++) {
    data[i*3] = G_VALUE[tmp[i].x];
    data[i*3+1] = G_VALUE[tmp[i].y];
    data[i*3+2] = G_VALUE[tmp[i].z];
  }

  free(tmp);

  //run inference in an event queue
  printf("test acc reading... x: %1.3f, y: %1.3f, z: %1.3f\r\n", data[0], data[1], data[2]);
  printf("test acc reading... x: %1.3f, y: %1.3f, z: %1.3f\r\n", data[79*3], data[79*3+1], data[79*3+2]);
  printf("test acc reading... x: %1.3f, y: %1.3f, z: %1.3f\r\n", data[159*3], data[159*3+1], data[159*3+2]);

}

int main() {

  pc.printf("program start\r\n");

  Acc.init();
  BSP_LCD_Init();
  if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_ERROR) {
      printf("BSP_TS_Init error\n");
  }
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  //setup buffer callback
  buff.setCallBack(uTensorTrigger);

  //Sensor Ticker
  Ticker sensorTick;
  sensorTick.attach(&accTimerHandler, 1.0f/32); //32Hz
 
  while(1) {
    // update LCD here
    // memory test
    uint32_t size = 1024 * 16; //16k
      for(int i = 0; i < 5; i++) {
      void* ptr = malloc(size);
      if (ptr == NULL) {
        printf("malloc %d kb failed\r\n", size / 1024);
        exit(0);
      } else {
        printf("malloc %d kb successful\r\n", size / 1024);
      }
      free(ptr);
      size *= 2;
    }


    wait(5);
  }
}