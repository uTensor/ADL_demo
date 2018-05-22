# ADL

  Activity of Daily living work-in-progress repository. This means to serve as a time-series-data-processing reference implementation using uTensor. Datasets of different activities are group into 4 classes:

In [Train/sample_generator.py](https://github.com/neil-tan/ADL_demo/blob/master/Train/sample_generator.py)
```
  [train, test] = self.split_file_sets(["Walk/", "Climb_stairs/", "Descend_stairs/", "Standup_chair/" "Getup_bed/"]) #moving
  ...
   [train, test] = self.split_file_sets(["Brush_teeth/",  "Use_telephone/"]) #daily activities
  ...
   [train, test] = self.split_file_sets(["Drink_glass/", "Eat_meat/", "Eat_soup/", "Pour_water/"]) #daily food activities
  ...
   [train, test] = self.split_file_sets(["Liedown_bed/", "Sitdown_chair/"]) #resting
```

For sensor setup, please refer to [Train/HMP_Dataset/MANUAL.txt](https://github.com/neil-tan/ADL_demo/blob/master/Train/HMP_Dataset/MANUAL.txt). The grove sensor is place flat on the back of user's right hand, with the connector socket oriented furthest away from the wrist.



## Hardware requirement:

  - Mbed F413ZH board
  - SD card (less than 32GB)
  - Grove Sheild
  - Grove 3D digital accelerometer

## Build Instruction
- Recommend [cloud9 environment](https://github.com/uTensor/cloud9-installer)
- Clone the repository
- Copy `Train\adl_model\constants` onto the root of a FAT32 formated SD card
- Insert the SD card into the board
- Run:
```
$ mbed deploy
$ mbed compile -m DISCO_F413ZH -t GCC_ARM --profile=uTensor/build_profile/release.json -f
```

## Training
For Training Instruction, please see [Train/README.md](https://github.com/neil-tan/ADL_demo/blob/master/Train/README.md)

## TODO:
- Fine tunning the model
- Add the ability to identify idle activities

