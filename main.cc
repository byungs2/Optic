#include <iostream>
#include <opencv2/core.hpp>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
  int i = 0;
  int fd = 0;
  int ret = 0;
  struct v4l2_capability cap;

  (void) sizeof (argc);
  (void) sizeof (*argv);
  (void) sizeof (i);

  fd = open ("/dev/video0", O_RDWR);
  ret = ioctl (fd, VIDIOC_QUERYCAP, &cap);

  std::cout << fd << std::endl;
  std::cout << EACCES << " : EACCES" << std::endl;
  std::cout << EBUSY << " : EBUSY" << std::endl;
  std::cout << ENXIO << " : ENXIO" << std::endl;
  std::cout << ENOMEM << " : ENOMEM" << std::endl;
  std::cout << EMFILE << " : EMFILE" << std::endl;
  std::cout << ENFILE << " : ENFILE" << std::endl;
  std::cout << ret << std::endl;

  std::cout << cap.driver << " : driver" << std::endl;
  std::cout << cap.card << " : card" << std::endl;
  std::cout << cap.bus_info << " : bus info" << std::endl;
  std::cout << ((cap.version >> 16) & 0xFF) << std::endl;
  std::cout << ((cap.version >> 8) & 0xFF) << std::endl;
  std::cout << (cap.version & 0xFF) << std::endl;
  printf ("0x%x\n", cap.capabilities);
  std::cout << cap.device_caps << std::endl;
  std::cout << cap.reserved << std::endl;

  ret = close (fd);
  std::cout << ret << std::endl;
  return 0;
}
