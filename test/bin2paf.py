import numpy as np
import matplotlib.pyplot as plt
import cv2

def deqnt_affine (i8, zp, scale):
    f32 = (int(i8) - int(zp)) * scale
    return f32

if __name__ == '__main__':
    zp = -1
    scale = 0.010398
    input_file = 'output.bin'
    i8_arr = []
    f32_arr = []

    data = np.fromfile(input_file, dtype=np.float32)
    for i in data:
        if i > 0.5:
            print(i)
    hist = data.flatten()
    fig = plt.hist(hist, bins=256, edgecolor='red', label='rknn', alpha=0.5)
    plt.savefig('./poseoutput_hist.png')

    '''
    with open(input_file, 'r') as f:
        output = f.read()

        outer = []
        for out in output:
            i8_arr.append(out)
            f32 = deqnt_affine(out, zp, scale)
            f32_arr.append(f32)

        data = np.array(f32_arr)
        data = data.reshape((1, 80, 46, 82))

        output_heatmap = data[:, :26, :, :]
        output_paf = data[:, 26:78, :, :]
        origin = np.load('./output_np.npy').reshape((1, 78, 46, 82))

        histdata_origin = origin.flatten()
        histdata = data[:, :78, :, :].flatten()
        histdata_heatmap = output_heatmap.flatten()
        histdata_paf = output_paf.flatten()

        fig = plt.hist(histdata, bins=256, edgecolor='red', label='rknn', alpha=0.5)
        #fig = plt.hist(histdata_origin, bins=256, edgecolor='black', label='origin', alpha=0.5)
        plt.savefig('./poseoutput_hist.png')
    '''
