#ifndef _RKNN_APP_LAYOUT_CONVERT_H_
#define _RKNN_APP_LAYOUT_CONVERT_H_

int
nchw_2_nhwc(rknn_tensor_attr *src,
  unsigned char* src_ptr,
  rknn_tensor_attr *dst,
  unsigned char* dst_ptr,
  int type_size)
{
    // transpose nchw to nhwc, consider w align.
    int ret = 0;
    int N = src->dims[0];
    int C = src->dims[1];
    int H = src->dims[2];
    int W = src->dims[3];

    int dst_W;
    if (dst->size != dst->size_with_stride){
        int attr_dtype_size = dst->size / dst->n_elems;
        dst_W = dst->size_with_stride / (attr_dtype_size * N * C * H);
    }
    else{
        dst_W = W;
    }

    int src_HW = H * W;
    int src_CHW = C * src_HW;
    int dst_WC = dst_W * C;
    int dst_HWC = H * dst_WC;

    for (int n = 0; n < N; n++){
        for (int h = 0; h < H; h++){
            for (int w = 0; w < W; w++){
                for (int c = 0; c < C; c++){
                    int src_idx = n * src_CHW + c * src_HW + h * W + w;
                    int dst_idx = n * dst_HWC + h * dst_WC + w * C + c;
                    memcpy(dst_ptr + dst_idx * type_size, src_ptr + src_idx * type_size, type_size);
                }
            }
        }
    }
    return ret;
}


int
nhwc_2_nchw(rknn_tensor_attr *src,
  unsigned char* src_ptr,
  rknn_tensor_attr *dst,
  unsigned char* dst_ptr,
  int type_size)
{
    // transpose nhwc to nchw, consider w align.
    int ret = 0;
    int N = src->dims[0];
    int H = src->dims[1];
    int W = src->dims[2];
    int C = src->dims[3];

    int dst_W = dst->dims[3];

    int src_WC = W * C;
    int src_HWC = H * src_WC;
    int dst_HW = H * dst_W;
    int dst_CHW = C * dst_HW;

    for (int n = 0; n < N; n++){
        for (int c = 0; c < C; c++){
            for (int h = 0; h < H; h++){
                for (int w = 0; w < dst_W; w++){
                    int src_idx = n * src_HWC + h * src_WC + w * C + c;
                    int dst_idx = n * dst_CHW + c * dst_HW + h * dst_W + w;
                    memcpy(dst_ptr + dst_idx * type_size, src_ptr + src_idx * type_size, type_size);
                }
            }
        }
    }
    return ret;
}


int
nchw_2_nc1hwc2(rknn_tensor_attr *src,
  unsigned char* src_ptr,
  rknn_tensor_attr *dst,
  unsigned char* dst_ptr,
  int type_size)
{
    // transpose nchw to nc1hwc2, consider c2 align
    int ret = 0;
    int N = src->dims[0];
    int C = src->dims[1];
    int H = src->dims[2];
    int W = src->dims[3];

    int C1 = dst->dims[1];
    int C2 = dst->dims[4];

    int src_HW = H * W;
    int src_CHW = C * src_HW;
    int dst_WC2 = W * C2;
    int dst_HWC2 = H * dst_WC2;
    int dst_C1HWC2 = C1 * dst_HWC2;

    for (int n = 0; n < N; n++){
        for (int c = 0; c < C; c++){
            for (int h = 0; h < H; h++){
                for (int w = 0; w < W; w++){
                    int c1 = c / C2;
                    int c2 = c % C2;
                    int src_idx = n * src_CHW + c * src_HW + h * W + w;
                    int dst_idx = n * dst_C1HWC2 + c1 * dst_HWC2 + h * dst_WC2 + w * C2 + c2;
                    memcpy(dst_ptr + dst_idx * type_size, src_ptr + src_idx * type_size, type_size);
                }
            }
        }
    }    
    return ret;
}


int
nc1hwc2_2_nchw(rknn_tensor_attr *src,
  unsigned char* src_ptr,
  rknn_tensor_attr *dst,
  unsigned char* dst_ptr,
  int type_size)
{
    int ret = 0;

    int N = src->dims[0];
    int C1 = src->dims[1];
    int H = src->dims[2];
    int W = src->dims[3];
    int C2 = src->dims[4];

    int C = dst->dims[1];

    int src_WC2 = W * C2;
    int src_HWC2 = H * src_WC2;
    int src_C1HWC2 = C1 * src_HWC2;
    int dst_HW = H * W;
    int dst_CHW = C * dst_HW;

    for (int n = 0; n < N; n++){
        for (int c = 0; c < C; c++){
            for (int h = 0; h < H; h++){
                for (int w = 0; w < W; w++){
                    int c1 = c / C2;
                    int c2 = c % C2;
                    int src_idx = n * src_C1HWC2 + c1 * src_HWC2 + h * src_WC2 + w * C2 + c2;
                    int dst_idx = n * dst_CHW + c * dst_HW + h * W + w;
                    memcpy(dst_ptr + dst_idx * type_size, src_ptr + src_idx * type_size, type_size);
                }
            }
        }
    }

    return ret;
}

int
nhwc_2_nc1hwc2(rknn_tensor_attr *src,
  unsigned char* src_ptr,
  rknn_tensor_attr *dst,
  unsigned char* dst_ptr,
  int type_size)
{
    int ret = 0;
    int N = src->dims[0];
    int H = src->dims[1];
    int W = src->dims[2];
    int C = src->dims[3];

    int C1 = dst->dims[1];
    int C2 = dst->dims[4];

    int src_WC = W * C;
    int src_HWC = H * src_WC;
    int dst_WC2 = W * C2;
    int dst_HWC2 = H * dst_WC2;
    int dst_C1HWC2 = C1 * dst_HWC2;

    for (int n = 0; n < N; n++){
        for (int c = 0; c < C; c++){
            for (int h = 0; h < H; h++){
                for (int w = 0; w < W; w++){
                    int c1 = c / C2;
                    int c2 = c % C2;
                    int src_idx = n * src_HWC + h * src_WC + w * C + c;
                    int dst_idx = n * dst_C1HWC2 + c1 * dst_HWC2 + h * dst_WC2 + w * C2 + c2;
                    memcpy(dst_ptr + dst_idx * type_size, src_ptr + src_idx * type_size, type_size);
                }
            }
        }
    }

    return ret;
}

#endif 
