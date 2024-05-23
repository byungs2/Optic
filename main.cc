#include <iostream>
#include <opencv2/core.hpp>

int
main (int argc, char *argv[])
{
  cv::Mat a;
  (void) sizeof (argc);
  (void) sizeof (*argv);
  (void) sizeof (a);
  std::cout << a << std::endl;
  std::cout << "HELLO WORLD" << std::endl;
  return 0;
}
