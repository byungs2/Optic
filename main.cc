#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>

#include <linux/videodev2.h>
#include <rknn_api.h>
#include "dma_alloc.hpp"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

int
nc1hwc2_2_nchw(const int8_t *src_ptr,
		int8_t *dst_ptr,
		int *dims,
		int channel)
{
    int ret = 0;

    int N = dims[0];
    int C1 = dims[1];
    int H = dims[2];
    int W = dims[3];
    int C2 = dims[4];

    int C = channel;

    int src_WC2 = W * C2;
    int src_HWC2 = H * src_WC2;
    int src_C1HWC2 = C1 * src_HWC2;
    int dst_HW = H * W;
    int dst_CHW = C * dst_HW;

    int type_size = 1;

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
convert (const int8_t *src, float *dst, int size, int zp, float scale)
{
  for (int i = 0; i < size; i++) {
    dst[i] = (float)((int)src[i] - zp) * scale;
  }
  return 0;
}

int
NC1HWC2_i8_to_NCHW_i8 (const int8_t *src,
		float *dst,
		int *dims,
		int channel,
		int h,
		int w,
		int zp,
		float scale)
{
  int batch = dims[0];
  int C1 = dims[1];
  int C2 = dims[4];
  int hw_src = dims[2] * dims[3];
  int hw_dst = h * w;

  for (int i = 0; i < batch; i++) {
    const int8_t *src_b = src + i * C1 * hw_src * C2;
    float *dst_b = dst + i * channel * hw_dst;
    for (int c = 0; c < channel; c++) {
      int plane = c / C2;
      const int8_t *src_bc = plane * hw_src * C2 + src_b;
      int offset = c % C2;
      for (int cur_h = 0; cur_h < h; cur_h++) {
        for (int cur_w = 0; cur_w < w; cur_w++) {
          int cur_hw = cur_h * cur_w;
	  dst_b[c * hw_dst + cur_hw] = (float)(src_bc[C2 * cur_hw + offset] - zp) * scale;
	}
      }
    }
  }
  return 0;
}

inline static int32_t
__clip(float val, float min, float max)
{
    float f = val <= min ? min : (val >= max ? max : val);
    return f;
}

struct buffer {
  void *start;
  size_t length;
};

typedef struct {
  char *dma_buf_virt_addr;
  int dma_buf_fd;
  int size;
} rknn_dma_buf;

typedef struct {
  rknn_context rknn_ctx;
  rknn_input_output_num io_num;
  rknn_tensor_attr *input_attr;
  rknn_tensor_attr *output_attr;
  rknn_tensor_mem *input_mems[1];
  rknn_tensor_mem *output_mems[1];
  rknn_dma_buf img_dma_buf;
  int model_channel;
  int model_width;
  int model_height;
  bool is_quant;
} rknn_app_context_t;

void
get_single_image ()
{
  int fd = 0;
  int ret = 0;
  unsigned int num_plane = 0;
  static int outbuf = 1;
  struct v4l2_format fmt;
  struct v4l2_requestbuffers req;
  struct v4l2_capability cap;
  struct v4l2_plane *planes = NULL;
  struct v4l2_plane out_planes[VIDEO_MAX_PLANES];
  struct buffer *buffers = NULL;
  FILE *output = NULL;

  const char *dev_name = "/dev/video11";
  unsigned int index = 0;
  unsigned int j = 0;

  fd = open (dev_name, O_RDWR, 0);
  output = fopen ("output.bin", "wb");
  /* CHECK CAPS */
  ret = ioctl (fd, VIDIOC_QUERYCAP, &cap);
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
    printf ("No video capture mplane\n");
  }
  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    printf ("No video streaming\n");
  }
  printf ("%x\n", cap.capabilities);

  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  ret = ioctl (fd, VIDIOC_G_FMT, &fmt);
  num_plane = fmt.fmt.pix_mp.num_planes;

  CLEAR (req);
  req.count = 4;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  req.memory = V4L2_MEMORY_MMAP;
  ret = ioctl (fd, VIDIOC_REQBUFS, &req);
  printf ("%d\n", ret);
  if (EINVAL == errno) {
    printf ("REQ ERROR %d\n", errno);
  }

  /* ALLOC & INIT MEM */
  buffers = (struct buffer *)calloc (req.count * num_plane, sizeof (*buffers));
  if (!buffers) {
    printf ("OOM\n");
  } else {
    printf ("BUF ALLOC OK %d\n", req.count);
  }

  for (index = 0; index < req.count; index++) {
    struct v4l2_buffer buf;
    CLEAR (buf);
    planes = (struct v4l2_plane *)calloc (num_plane, sizeof (*planes));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = index;
    buf.m.planes = planes;
    buf.length = num_plane;

    ret = ioctl (fd, VIDIOC_QUERYBUF, &buf);
    printf ("%d %d\n", ret, errno);
    for (j = 0; j < num_plane; j++) {
      printf ("BUF lenfth %d index %d %d mem offset %d\n",
		      buf.m.planes[j].length,
		      index,
		      j,
		      buf.m.planes[j].m.mem_offset);
      buffers[index * num_plane + j].length = buf.m.planes[j].length;
      buffers[index * num_plane + j].start = mmap (NULL,
		      buf.m.planes[j].length,
		      PROT_READ | PROT_WRITE,
		      MAP_SHARED,
		      fd,
		      buf.m.planes[j].m.mem_offset);
    }

    if (MAP_FAILED == buffers[index].start) {
      fprintf (stderr, "%s error %d, %s\n", "mmap", errno, strerror(errno));
    } else {
      printf ("MMAP OK %d\n", index);
    }
  }

  /* GET STREAM ON */
  for (index = 0; index < req.count; index++) {
    struct v4l2_buffer buf;
    CLEAR (buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = index;
    buf.m.planes = planes;
    buf.length = num_plane;
    ret = ioctl (fd, VIDIOC_QBUF, &buf);
    printf ("%d %d\n", ret, errno);
  }
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  ret = ioctl (fd, VIDIOC_STREAMON, &type);
  printf ("STREAM ON %d\n", ret);

  /* MAIN SINGLE LOOP */
  fd_set fds;
  struct timeval tv;
  FD_ZERO (&fds);
  FD_SET (fd, &fds);
  tv.tv_sec = 10;
  tv.tv_usec = 0;
  ret = select (fd + 1, &fds, NULL, NULL, &tv);
  printf ("%d\n", ret);

  /* READ BUFFER FRAME */
  struct v4l2_buffer buf;
  CLEAR (buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.m.planes = out_planes;
  buf.length = num_plane;

  ret = ioctl (fd, VIDIOC_DQBUF, &buf);
  printf ("DEQ %d %d %d\n", ret, buf.bytesused, errno);
  printf ("%d %d\n", EAGAIN, EIO);

  if (outbuf) {
    ret = fwrite (buffers[buf.index].start, buf.m.planes[0].bytesused, 1, output);
    printf ("%d %d write ret\n", ret, buf.bytesused);
    fflush (stderr);
    fprintf (stderr, ".write");
    fflush (stdout);
  }
  ret = ioctl (fd, VIDIOC_QBUF, &buf);
  printf ("%d\n", ret);

  /* STOP STREAM */
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  ret = ioctl (fd, VIDIOC_STREAMOFF, &type);
  printf ("%d\n", ret);

  /* FREE MEM */
  for (index = 0; index < req.count; index++) {
    munmap (buffers[index].start, buffers[index].length);
  }

  free (buffers);
  
  ret = close (fd);
  fclose (output);
  printf ("%d\n", ret);
  printf ("%d %d %d %d %d %d %d %d %d %d %d %d\n",
		  V4L2_COLORSPACE_DEFAULT,
		  V4L2_COLORSPACE_SMPTE170M,
		  V4L2_COLORSPACE_REC709,
		  V4L2_COLORSPACE_SRGB,
		  V4L2_COLORSPACE_ADOBERGB,
		  V4L2_COLORSPACE_BT2020,
		  V4L2_COLORSPACE_DCI_P3,
		  V4L2_COLORSPACE_SMPTE240M,
		  V4L2_COLORSPACE_470_SYSTEM_M,
		  V4L2_COLORSPACE_470_SYSTEM_BG,
		  V4L2_COLORSPACE_JPEG,
		  V4L2_COLORSPACE_RAW);
}

/*
static float deqnt_affine_to_f32(int8_t qnt, int32_t zp, float scale) { return ((float)qnt - (float)zp) * scale; }
static float deqnt_affine_u8_to_f32(uint8_t qnt, int32_t zp, float scale) { return ((float)qnt - (float)zp) * scale; }
*/

static void
dump_tensor_attr(rknn_tensor_attr *attr)
{
    printf("  index=%d, name=%s, n_dims=%d, dims=[%d, %d, %d, %d], n_elems=%d, size=%d, fmt=%s, type=%s, qnt_type=%s, "
           "zp=%d, scale=%f\n",
           attr->index, attr->name, attr->n_dims, attr->dims[0], attr->dims[1], attr->dims[2], attr->dims[3],
           attr->n_elems, attr->size, get_format_string(attr->fmt), get_type_string(attr->type),
           get_qnt_type_string(attr->qnt_type), attr->zp, attr->scale);
}

int
release_model (rknn_app_context_t *app_ctx)
{
    if (app_ctx->input_attr != NULL)
    {
        free(app_ctx->input_attr);
        app_ctx->input_attr = NULL;
    }
    if (app_ctx->output_attr != NULL)
    {
        free(app_ctx->output_attr);
        app_ctx->output_attr = NULL;
    }
    for (uint32_t i = 0; i < app_ctx->io_num.n_input; i++) {
        if (app_ctx->input_mems[i] != NULL) {
            rknn_destroy_mem(app_ctx->rknn_ctx, app_ctx->input_mems[i]);
        }
    }
    for (uint32_t i = 0; i < app_ctx->io_num.n_output; i++) {
        if (app_ctx->output_mems[i] != NULL) {
            rknn_destroy_mem(app_ctx->rknn_ctx, app_ctx->output_mems[i]);
        }
    }
    if (app_ctx->rknn_ctx != 0)
    {
        rknn_destroy(app_ctx->rknn_ctx);
        app_ctx->rknn_ctx = 0;
    }
    return 0;
}

static uint8_t
qnt_f32_to_affine(float f32, int32_t zp, float scale)
{
    float dst_val = (f32 / scale) + zp;
    uint8_t res = (uint8_t)__clip(dst_val, 0, 255);
    return res;
}

int
main (int argc, char *argv[])
{
  int ret = 0;
  uint32_t i = 0;
  int model_width = 656;
  int model_height = 368;
  rknn_context ctx;
  rknn_input_output_num io_num;
  rknn_app_context_t appctx;
  rknn_tensor_attr input_attr[1];
  rknn_tensor_attr output_attr[1];
  const char *model_path = argv[1];
  const char *image_path = argv[2];
  FILE *output = NULL;

  (void)sizeof (argc);
  CLEAR (input_attr);
  CLEAR (output_attr);

  /* init rknn */
  ret = rknn_init(&ctx, (void *)model_path, 0, 0, NULL); 
  ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));

  for (i = 0; i < io_num.n_input; i++) {
    input_attr[i].index = i;
    ret = rknn_query (ctx, RKNN_QUERY_NATIVE_INPUT_ATTR, &(input_attr[i]), sizeof (rknn_tensor_attr));
    input_attr[i].type = RKNN_TENSOR_UINT8;
    input_attr[i].fmt = RKNN_TENSOR_NHWC;
    appctx.input_mems[i] = rknn_create_mem (ctx, input_attr[i].size_with_stride);
    ret = rknn_set_io_mem (ctx, appctx.input_mems[i], &input_attr[i]);
    dump_tensor_attr(&(input_attr[i]));
  }
  for (i = 0; i < io_num.n_output; i++) {
    output_attr[i].index = i;
    ret = rknn_query (ctx, RKNN_QUERY_NATIVE_OUTPUT_ATTR, &(output_attr[i]), sizeof (rknn_tensor_attr));
    appctx.output_mems[i] = rknn_create_mem (ctx, output_attr[i].size_with_stride);
    ret = rknn_set_io_mem (ctx, appctx.output_mems[i], &output_attr[i]);
    dump_tensor_attr(&(output_attr[i]));
  }

  appctx.rknn_ctx = ctx;
  appctx.io_num = io_num;
  appctx.input_attr = input_attr;
  appctx.output_attr = output_attr;

  if (input_attr[0].fmt == RKNN_TENSOR_NCHW) {
    printf("model is NCHW input fmt\n");
    appctx.model_channel = input_attr[0].dims[1];
    appctx.model_height  = input_attr[0].dims[2];
    appctx.model_width   = input_attr[0].dims[3];
  } else {
    printf("model is NHWC input fmt\n");
    appctx.model_height  = input_attr[0].dims[1];
    appctx.model_width   = input_attr[0].dims[2];
    appctx.model_channel = input_attr[0].dims[3];
  } 

  printf("model input height=%d, width=%d, channel=%d\n",
         appctx.model_height, appctx.model_width, appctx.model_channel);
  printf ("RET INIT RKNN %d %d\n", ret, io_num.n_output);

  cv::Mat img = cv::imread(image_path, 1);
  cv::Mat model_input (model_height, model_width, CV_8UC3, appctx.input_mems[0]->virt_addr);
  cv::resize (img, model_input, cv::Size (model_width, model_height), 0, 0, cv::INTER_LINEAR);

  uint8_t *data = (uint8_t *)img.data;
  int32_t zp = 0;
  float scale = 0.003906;

  for (i = 0; i < (uint32_t)model_input.rows; i++) {
    for (int j = 0; j < model_input.cols; j++) {
      uint8_t *pixel = data + i * img.step + j * img.elemSize();

      uint8_t before_val_b = pixel[0];
      uint8_t before_val_g = pixel[1];
      uint8_t before_val_r = pixel[2];

      float norm_input_b = ((float)pixel[0] - 128.0) / 256.0;
      float norm_input_g = ((float)pixel[1] - 128.0) / 256.0;
      float norm_input_r = ((float)pixel[2] - 128.0) / 256.0;

      pixel[0] = qnt_f32_to_affine (norm_input_r, zp, scale);
      pixel[0] = before_val_b;

      pixel[1] = qnt_f32_to_affine (norm_input_g, zp, scale);
      pixel[1] = before_val_g;

      pixel[2] = qnt_f32_to_affine (norm_input_b, zp, scale);
      pixel[2] = before_val_r;

      if (i%100 == 0 && j%100 == 0) {
        printf ("%f %f %f %d %d %d %d %d %d\n",
			norm_input_b,
			norm_input_g,
			norm_input_r,
			before_val_b,
			before_val_g,
			before_val_r,
			pixel[0],
			pixel[1],
			pixel[2]);
      }
    }
  }
  ret = rknn_run (appctx.rknn_ctx, nullptr);

  if (ret < 0) {
    printf ("rknn failed\n");
  }

  output = fopen ("output.bin", "wb");

  float dst[301760];
  int8_t dst_i8[301760];
  int dims[5] = {1, 5, 46, 82, 16};

  nc1hwc2_2_nchw ((int8_t *)appctx.output_mems[0]->virt_addr,
		  dst_i8,
		  dims,
		  80);
  convert (dst_i8, dst, 301760, appctx.output_attr[0].zp, appctx.output_attr[0].scale);
  /*
  NC1HWC2_i8_to_NCHW_i8 ((int8_t *)appctx.output_mems[0]->virt_addr,
		  dst,
		  dims,
		  80,
		  46,
		  82,
		  appctx.output_attr[0].zp,
		  appctx.output_attr[0].scale);
  */
  printf ("ZP %d SCALE %f\n",  appctx.output_attr[0].zp, appctx.output_attr[0].scale);

  ret = fwrite (dst, 301760, 4, output);
  fclose (output);

  release_model (&appctx);

  return 0;
}
