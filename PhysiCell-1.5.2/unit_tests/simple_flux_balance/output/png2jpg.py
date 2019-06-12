# Use ImageMagick's "convert" to convert .png files to .jpg

import sys
import os

print(len(sys.argv))
if (len(sys.argv) < 5):
  usage_str = "Usage: %s start_idx stop_idx step resize_pct " % (sys.argv[0])
  print(usage_str)
  print("e.g.,")
  eg_str = "%s 0 1000 10 20" % (sys.argv[0])
  print(eg_str)
  exit(1)
else:
   start_idx = int(sys.argv[1])
   stop_idx = int(sys.argv[2])
   step = int(sys.argv[3])
   resize_pct = sys.argv[4]

for idx in range(start_idx,stop_idx+1,step):
  fname = "snapshot%08d" % idx
  fname = "aaa%08d" % idx
  cmd = "convert " + fname + ".png -resize " + resize_pct + "% " + fname + ".jpg"
  print(cmd)
  os.system(cmd)
