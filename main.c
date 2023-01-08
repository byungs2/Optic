#include "include/optic_tensor.h"

int
main (int argc, char *agrv[])
{
  gint64 iter, time, max_iter, max_dim;
  gfloat diff1, diff2, dist;
  OpticTensor *self = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *other = g_object_new (OPTIC_TYPE_TENSOR, NULL);

  OpticTensor *self_1 = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *other_1 = g_object_new (OPTIC_TYPE_TENSOR, NULL);

  OpticTensor *self_2 = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *other_2 = g_object_new (OPTIC_TYPE_TENSOR, NULL);

  max_dim = 5;
  max_iter = 100;
  g_object_set (self, "dim", max_dim, NULL);
  g_object_set (other, "dim", max_dim, NULL);

  optic_tensor_add (self, 1.0);

  /* TODO check assembly between o0 and o3 */

  time = g_get_real_time ();
  for (iter = 0; iter < max_iter; iter++) {
    dist = optic_tensor_distance (self, other);
  }
  diff2 = ((g_get_real_time () - time)/1000.0);
  g_print ("AVX 256 TIME %f, dist : %f\n", diff2, dist);

  g_print ("Size of void * %ld\n", sizeof(void *));

  g_object_unref (self);
  g_object_unref (other);
  return 0;
}
