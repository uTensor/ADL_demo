# uTensor ADL Demo

  Activity of Daily living work-in-progress repository. This means to serve as a time-series-data-processing reference implementation using uTensor. Datasets of different activities are group into 4 classes:

In [sample_generator.py](https://github.com/neil-tan/ADL_demo/blob/master/Train/sample_generator.py)
```
    [train, test] = self.split_file_sets(["Walk/", "Walk_MODEL/"]) #walking
  ...
    [train, test] = self.split_file_sets(["Climb_stairs/", "Climb_stairs_MODEL/",  "Standup_chair/", "Standup_chair_MODEL/", "Getup_bed/", "Getup_bed_MODEL/"]) #up
  ...
    [train, test] = self.split_file_sets(["Brush_teeth/",  "Use_telephone/", "Drink_glass/", "Drink_glass_MODEL/", "Eat_meat/", "Eat_soup/", "Pour_water/", "Pour_water_MODEL/"]) #daily activities
  ...
    [train, test] = self.split_file_sets(["Liedown_bed/", "Sitdown_chair/", "Sitdown_chair_MODEL/", "Descend_stairs/"]) #down
  ...
    [train, test] = self.split_file_sets(["Liedown_bed/", "Sitdown_chair/"]) #resting


## Build instruction
Run:

 ```
$ cd Train
$ python gen_rest.py
$ python train.py
 ```
Here's the expected output:

 ```
 ...
step 198000, training accuracy 0.972
step 199000, training accuracy 0.976
step 200000, training accuracy 0.982
test accuracy 0.872
saving checkpoint: chkps/adl_model
Converted 6 variables to const ops.
written graph to: adl_model/deep_mlp.pb
the output nodes: ['y_pred']
 ```

Using utensor-cli:

```
$ cd adl_model && utensor-cli deep_mlp.pb --output-nodes=y_pred
```

- Model parameters are saved into the PROJECTROOT/Train/constants/deep-mlp/ as idx files
- C++ graph descripts are saved as .cpp and .hpp files in PROJECTROOT/Train/models