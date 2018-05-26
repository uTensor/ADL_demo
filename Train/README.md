# uTensor ADL Demo

  This is a WIP repository laying the groudwork for machine learning on accelerometer data on MCUs. For initial feasbility study, we are using the Activity of Daily Living (ADL) dataset. It contains accelerometer recorded at 32 Hz and lengthing from seconds to minutes, some of the activities include: walking, climbing stairs, brushing teeth, etc.
  
## Build instruction
In [Cloud9 reference environment](https://github.com/uTensor/cloud9-installer), clone the repository.

```
$ git clone https://github.com/neil-tan/ADL_demo.git
```

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