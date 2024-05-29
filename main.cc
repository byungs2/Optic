#include <iostream>
#include <opencv2/core.hpp>
#include <fcntl.h>

int
main (int argc, char *argv[])
{
  int fd = 0;
  (void) sizeof (argc);
  (void) sizeof (*argv);

  fd = open ("/dev/video0", O_RDWR);
  std::cout << fd << std::endl;
  std::cout << EACCES << std::endl;
  std::cout << EBUSY << std::endl;
  std::cout << ENXIO << std::endl;
  std::cout << ENOMEM << std::endl;
  std::cout << EMFILE << std::endl;
  std::cout << ENFILE << std::endl;
  return 0;
}
