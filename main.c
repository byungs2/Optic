#include "include/optic_point.h"
#include <xmmintrin.h>

int
main (int argc, char *agrv[])
{
  gint64 iter, time, max_iter, max_dim;
  gfloat diff1, diff2;
  OpticPoint *self = g_object_new (OPTIC_TYPE_POINT, NULL);
  OpticPoint *other = g_object_new (OPTIC_TYPE_POINT, NULL);

  max_dim = 50000;
  max_iter = 1000;
  g_object_set (self, "dim", max_dim, NULL);
  g_object_set (other, "dim", max_dim, NULL);

  optic_point_add (self, 1.0);
  time = g_get_real_time ();
  for (iter = 0; iter < max_iter; iter++) {
    optic_point_distance (self, other);
  }
  diff1 = ((g_get_real_time () - time)/1000.0);
  g_print ("TAKE TIME %f\n", diff1);

  time = g_get_real_time ();
  for (iter = 0; iter < max_iter; iter++) {
    optic_point_distance_sse (self, other);
  }
  diff2 = ((g_get_real_time () - time)/1000.0);
  g_print ("TAKE TIME %f\n", diff2);

  g_print ("RATIO %f\n", (diff1/1000.0)/(diff2/1000.0));

  g_object_unref (self);
  g_object_unref (other);
  return 0;
}
