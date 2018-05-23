#importing stuff
import argparse
from io import StringIO
import numpy as np
import os

def genRestVector():
  ## Generating resting data
  ## x^2 + b^2 + c^2 = 1
  ## x, y, z all scaled by alpha s.t.
  ## (alpha*x)^2 + (alpha*x)^2 + (alpha*x)^2 = 1
  ## alpha = (x^2 + b^2 + c^2)^0.5

  rand_vect = np.random.rand(3)
  rand_vect = rand_vect - 0.5 #we want negatives
  rand_vect_tmp = np.square(rand_vect)
  rand_vect_tmp = np.sum(rand_vect_tmp)
  alpha = np.power(rand_vect_tmp, -0.5)
  rand_vect = rand_vect * alpha
  #np.sum(np.power(rand_vect, 2))
  return rand_vect

def vect2int6str(rand_vect):
  acc_sample = []
  for acc in rand_vect:
    acc_code = (64 + int(np.round(acc / 0.023438))) % 64
    acc_sample.append(acc_code)

  return str(acc_sample[0]) + " " + str(acc_sample[1]) + " " + str(acc_sample[2])
  
def genRestFiles(output_path, sample_len, n_files):
  if not os.path.exists(output_path):
    os.makedirs(output_path)

  for i in range(n_files):
    filename = "gen-resting-" + str(i) + ".txt"
    file_path = os.path.join(output_path, filename)
    with open(file_path, "w") as text_file:
      rand_vect = genRestVector()
      out_str = vect2int6str(rand_vect)
      for j in range(sample_len):
        text_file.write(out_str + "\r\n")


if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('--output_path', type=str,
                      default='./HMP_Dataset/rest-gen',
                      help='Directory for storing generated data')
  parser.add_argument('--sample_len', type=int,
                      default=32*60,
                      help='sample length for each generated file')
  parser.add_argument('-num_files', type=int,
                      dest='n_files',
                      default=200,
                      help='how many files to generate')
  FLAGS, unparsed = parser.parse_known_args()
  genRestFiles(FLAGS.output_path, FLAGS.sample_len, FLAGS.n_files)