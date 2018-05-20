#!/usr/bin/python
# -*- coding: utf8 -*-
"""
# This script is based on:
# https://www.tensorflow.org/get_started/mnist/pros
"""
from __future__ import print_function
import argparse
import sys
import tensorflow as tf
from sample_generator import ALD_Data
from tensorflow.python.framework import graph_util as gu
from tensorflow.tools.graph_transforms import TransformGraph

FLAGS = None


# helper functions
def weight_variable(shape, name):
  """weight_variable generates a weight variable of a given shape."""
  initial = tf.truncated_normal(shape, stddev=0.1)
  return tf.Variable(initial, name)


def bias_variable(shape, name):
  """bias_variable generates a bias variable of a given shape."""
  initial = tf.constant(0.1, shape=shape)
  return tf.Variable(initial, name)


# Fully connected 2 layer NN
def deepnn(x):
  W_fc1 = weight_variable([450, 64], name='W_fc1')
  b_fc1 = bias_variable([64], name='b_fc1')
  a_fc1 = tf.add(tf.matmul(x, W_fc1), b_fc1, name="zscore")
  h_fc1 = tf.nn.relu(a_fc1)
  layer1 = tf.nn.dropout(h_fc1, 0.70)

  W_fc2 = weight_variable([64, 32], name='W_fc2')
  b_fc2 = bias_variable([32], name='b_fc2')
  a_fc2 = tf.add(tf.matmul(layer1, W_fc2), b_fc2, name="zscore")
  h_fc2 = tf.nn.relu(a_fc2)
  layer2 = tf.nn.dropout(h_fc2, 0.50)

  W_fc3 = weight_variable([32, 4], name='W_fc3')
  b_fc3 = bias_variable([4], name='b_fc3')
  logits = tf.add(tf.matmul(layer2, W_fc3), b_fc3, name="logits")
  y_pred = tf.argmax(logits, 1, name='y_pred')

  return y_pred, logits


def main(_):
  # Data generator settings
  resample_rate = 30 #Hz
  sample_period = 5 #seconds
  input_length = 150 #30 * 5
  classes = 4

  # Data generator
  adl_inputPipe = ALD_Data(FLAGS.data_dir, test_ratio = 0.3, resample_rate=resample_rate, sample_period=sample_period)
  
  print("train files")
  for i in adl_inputPipe.act_train_files:
    print(len(i))

  print("test files")
  for i in adl_inputPipe.act_test_files:
    print(len(i))

  # Specify inputs, outputs, and a cost function
  # placeholders
  x = tf.placeholder(tf.float32, [None, 450], name="x")
  y_ = tf.placeholder(tf.float32, [None, 4], name="y")

  # Build the graph for the deep net
  y_pred, logits = deepnn(x)

  with tf.name_scope("Loss"):
    cross_entropy = tf.nn.softmax_cross_entropy_with_logits_v2(labels=y_,
                                                               logits=logits)
    loss = tf.reduce_mean(cross_entropy, name="cross_entropy_loss")
  train_step = tf.train.AdamOptimizer(1e-4).minimize(loss, name="train_step")

  with tf.name_scope("Prediction"):
    correct_prediction = tf.equal(y_pred,
                                  tf.argmax(y_, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32), name="accuracy")

  # Start training session
  with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    saver = tf.train.Saver()

    # SGD
    for i in range(1, FLAGS.num_iter + 1):
      batch_data, batch_labels = adl_inputPipe.train.next_batch_flat(FLAGS.batch_size)
      train_step.run(feed_dict={x: batch_data, y_: batch_labels})
      if i % FLAGS.log_iter == 0:
        batch_data, batch_labels = adl_inputPipe.train.next_batch_flat(FLAGS.test_batch_size)
        train_accuracy = accuracy.eval(feed_dict={x: batch_data, y_: batch_labels})
        print('step %d, training accuracy %g' % (i, train_accuracy))


    batch_data, batch_labels = adl_inputPipe.test.next_batch_flat(FLAGS.test_batch_size)
    print('test accuracy %g' % accuracy.eval(feed_dict={x: batch_data, y_: batch_labels}))  ## TODO: Change this to use proper test data
    # Saving checkpoint and serialize the graph
    ckpt_path = saver.save(sess, FLAGS.chkp)
    print('saving checkpoint: %s' % ckpt_path)
    out_nodes = [y_pred.op.name]
    # Freeze graph and remove training nodes
    sub_graph_def = gu.remove_training_nodes(sess.graph_def)
    sub_graph_def = gu.convert_variables_to_constants(sess, sub_graph_def, out_nodes)
    if FLAGS.no_quant:
      graph_path = tf.train.write_graph(sub_graph_def,
                                        FLAGS.output_dir,
                                        FLAGS.pb_fname,
                                        as_text=False)
    else:
      # # quantize the graph
      # quant_graph_def = TransformGraph(sub_graph_def,
      #                                 [],
      #                                 out_nodes,
      #                                 ["quantize_weights", "quantize_nodes"])
      graph_path = tf.train.write_graph(sub_graph_def,
                                        FLAGS.output_dir,
                                        FLAGS.pb_fname,
                                        as_text=False)
    print('written graph to: %s' % graph_path)
    print('the output nodes: {!s}'.format(out_nodes))


if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('--data_dir', type=str,
                      default='./HMP_Dataset',
                      help='Directory for storing input data')
  parser.add_argument('--chkp', default='chkps/adl_model',
                      help='session check point (default: %(default)s)')
  parser.add_argument('-n', '--num-iteration', type=int,
                      dest='num_iter',
                      default=200000,
                      help='number of iterations (default: %(default)s)')
  parser.add_argument('--batch-size', dest='batch_size',
                      default=150, type=int,
                      help='batch size (default: %(default)s)')
  parser.add_argument('--test_batch_size', type=int,
                      default=500,
                      help='generated test batch size, from common data, default = 500')
  parser.add_argument('--log-every-iters', type=int,
                      dest='log_iter', default=1000,
                      help='logging the training accuracy per numbers of iteration %(default)s')
  parser.add_argument('--output-dir', default='adl_model',
                      dest='output_dir',
                      help='output directory directory (default: %(default)s)')
  parser.add_argument('--no-quantization', action='store_true',
                      dest='no_quant',
                      help='save the output graph pb file without quantization')
  parser.add_argument('-o', '--output', default='deep_mlp.pb',
                      dest='pb_fname',
                      help='output pb file (default: %(default)s)')
  FLAGS, unparsed = parser.parse_known_args()
  tf.app.run(main=main, argv=[sys.argv[0]] + unparsed)
  