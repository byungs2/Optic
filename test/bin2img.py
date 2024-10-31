import numpy as np
import cv2

def nv12_to_bgr(nv12_data, width, height):
    """
    Convert NV12 format data to BGR image.
    
    :param nv12_data: NV12 raw bytes.
    :param width: Width of the image.
    :param height: Height of the image.
    :return: BGR image.
    """
    # Calculate the size of Y and UV planes
    y_size = width * height
    uv_size = width * height // 2
    
    # Separate Y and UV planes from NV12 data
    y_plane = np.frombuffer(nv12_data, dtype=np.uint8).reshape((height * 3 // 2, width))

    # Convert YUV to BGR using OpenCV
    bgr = cv2.cvtColor(y_plane, cv2.COLOR_YUV2BGR_NV12)
    print(bgr.shape)
    print(bgr.size)

    return bgr

def main():
    # Input NV12 binary file
    input_file = 'output.bin'

    # Output image file
    output_image = 'result.png'

    # Image dimensions (must be known)
    width = 2304
    height = 1296

    # Read NV12 data from binary file
    with open(input_file, 'rb') as f:
        nv12_data = f.read()

    # Convert NV12 to BGR
    bgr_image = nv12_to_bgr(nv12_data, width, height)

    # Save the BGR image using OpenCV
    cv2.imwrite(output_image, bgr_image)
    print(f"Image saved to {output_image}")

if __name__ == "__main__":
    main()

