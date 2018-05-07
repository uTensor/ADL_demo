# uTensor ADL Demo

  This is a WIP repository laying the groudwork for machine learning on accelerometer data on MCUs. For initial feasbility study, we are using the Activity of Daily Living (ADL) dataset. It contains accelerometer recorded at 32 Hz and lengthing from seconds to minutes, some of the activities include: walking, climbing stairs, brushing teeth, etc.
  
## Build instruction
In [Cloud9 reference environment](https://github.com/uTensor/cloud9-installer), clone the repository.

```
$ git clone https://github.com/neil-tan/ADL_demo.git
```

Run:

 ```
$ python train.py
 ```
Here's the expected output:

 ```
step 97000, training accuracy 0.898
step 98000, training accuracy 0.918
step 99000, training accuracy 0.932
step 100000, training accuracy 0.928
test accuracy 0.826
saving checkpoint: chkps/adl_model
Converted 6 variables to const ops.
2018-05-07 09:59:23.567763: I tensorflow/tools/graph_transforms/transform_graph.cc:264] Applying quantize_weights
2018-05-07 09:59:23.569970: I tensorflow/tools/graph_transforms/transform_graph.cc:264] Applying quantize_nodes
written graph to: adl_model/deep_mlp.pb
the output nodes: [u'OutputLayer/y_pred']
$
 ```
## TODO

 - Merge Dropout support for uTensor-cli
 - Application code for sensor interface, capture and inference.