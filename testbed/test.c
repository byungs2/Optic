#include <stddef.h>
#include <stdio.h>
#include <glib.h>
#include <glib-object.h>

struct x {
  char a;
  int b;
  double c;
};

int 
main (int argc, char *argv[])
{
  int a = 0;
  size_t offset = offsetof (struct x, b);
  printf ("%ld\n", offset);
  printf ("%ld\n", G_TYPE_INT);

  return 0;
}
