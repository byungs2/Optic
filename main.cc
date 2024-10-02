#include <iostream>
#include <opencv2/core.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
  void *start;
  size_t length;
};

int
main (int argc, char *argv[])
{
  const char *dev_name = "/dev/video0";
  unsigned int index = 0;
  int fd = 0;
  int ret = 0;
  int size_plane = 1;
  static int outbuf = 1;
  struct v4l2_format fmt;
  struct v4l2_requestbuffers req;
  struct v4l2_capability cap;
  struct v4l2_plane *planes = NULL;
  struct v4l2_plane *out_planes = NULL;
  struct buffer *buffers = NULL;
  FILE *output = NULL;

  (void) sizeof (argc);
  (void) sizeof (*argv);
  (void) sizeof (index);
  (void) sizeof (req);

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
  buffers = (struct buffer *)calloc (req.count, sizeof (*buffers));
  if (!buffers) {
    printf ("OOM\n");
  } else {
    printf ("BUF ALLOC OK %d\n", req.count);
  }

  planes = (struct v4l2_plane *)calloc (size_plane, sizeof (*planes));
  out_planes = (struct v4l2_plane *)calloc (size_plane, sizeof (*planes));

  for (index = 0; index < req.count; index++) {
    struct v4l2_buffer buf;
    CLEAR (buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = index;
    buf.m.planes = planes;
    buf.length = size_plane;

    ret = ioctl (fd, VIDIOC_QUERYBUF, &buf);
    printf ("%d %d\n", ret, errno);
    buffers[index].length = buf.length;
    buffers[index].start = mmap (NULL, buf.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.planes[0].m.mem_offset);
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
    buf.length = size_plane;
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
  buf.length = size_plane;

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
  std::cout << ret << std::endl;
  return 0;
}
