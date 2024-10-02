import numpy as np
from PIL import Image

w, h = 3072, 1296

with open('output.bin', mode='rb') as f:
    d = np.fromfile(f, dtype = np.uint8, count = w * h).reshape(h, w)

PILimage = Image.fromarray(d)
PILimage.save('result.png')
