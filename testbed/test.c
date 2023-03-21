#include <stddef.h>
#include <stdio.h>
#include <glib.h>
#include <glib-object.h>

struct x {
  char a;
  int b;
  double c;
};

void *a;

void
test ()
{
  a = (void *) 20;
}

int 
main (int argc, char *argv[])
{
  test ();
  g_print (" %d \n", 
      (int) (long)a);
  return 0;
}
