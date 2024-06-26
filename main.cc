#include <iostream>
#include <opencv2/core.hpp>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

int
main (int argc, char *argv[])
{
  int fd = 0;
  int ret = 0;
  struct v4l2_capability cap;
  (void) sizeof (argc);
  (void) sizeof (*argv);

  fd = open ("/dev/video0", O_RDWR);
  ret = ioctl (fd, VIDIOC_QUERYCAP, &cap);

  std::cout << fd << std::endl;
  std::cout << EACCES << std::endl;
  std::cout << EBUSY << std::endl;
  std::cout << ENXIO << std::endl;
  std::cout << ENOMEM << std::endl;
  std::cout << EMFILE << std::endl;
  std::cout << ENFILE << std::endl;
  std::cout << ret << std::endl;

  std::cout << cap.driver << std::endl;
  std::cout << cap.card << std::endl;
  std::cout << cap.bus_info << std::endl;
  std::cout << ((cap.version >> 16) & 0xFF) << std::endl;
  std::cout << ((cap.version >> 8) & 0xFF) << std::endl;
  std::cout << (cap.version & 0xFF) << std::endl;
  std::cout << cap.capabilities << std::endl;
  std::cout << cap.device_caps << std::endl;
  std::cout << cap.reserved << std::endl;
  return 0;
}
