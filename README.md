# ADL

  uTensor enables motion recognition on microcontrollers. The model is trained with a modified Activity of Daily dataset recognizing 5 classes: 
  
  - Walking
  - Climbing
  - Activities
  - Descending
  - Resting
  
  The project is also a reference implementation of sequential data processing with Mbed and uTensor.
  
  ![Board, SD, grove shield and Accelerometer](/docs/images/boardResting.jpg)

For sensor setup, please refer to [Train/HMP_Dataset/MANUAL.txt](https://github.com/neil-tan/ADL_demo/blob/master/Train/HMP_Dataset/MANUAL.txt). The grove sensor is place flat on the back of user's right hand, with the connector socket oriented furthest away from the wrist.

## Hardware requirement:

  - Mbed F413ZH board
  - SD card (less than 32GB)
  - Grove Sheild
  - Grove 3D digital accelerometer

## Build Instruction
- Recommend [cloud9 environment](https://github.com/uTensor/cloud9-installer)
- Run:
```
$ mbed import https://github.com/uTensor/ADL_demo
$ cd ADL_demo
$ mbed compile -m DISCO_F413ZH -t GCC_ARM --profile=uTensor/build_profile/release.json
```
- Copy `Train\adl_model\constants` onto the root of a FAT32 formated SD card
- Insert the SD card into the board
- Ensure the Grove sensor is connected
- Locate the binary path from the terminal output, and flash it onto the board

## Training
For Training Instruction, please see [Train/README.md](https://github.com/neil-tan/ADL_demo/blob/master/Train/README.md)

