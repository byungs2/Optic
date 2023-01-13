#include "include/optic_tensor.h"
#include "include/optic_queue.h"

int
main (int argc, char *agrv[])
{
  gint64 time, max_dim;
  gfloat diff1, diff2, dist;
  guint64 shape[1] = { 960 * 540 };
  guint64 max_iter, iter;
  OpticTensor *self = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *other = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticQueue *queue = g_object_new (OPTIC_TYPE_QUEUE, NULL);

  max_dim = 1;
  max_iter = 50 * 70 * 3;
  g_object_set (self, 
      "dim", max_dim, 
      "shape", shape,
      "dtype", G_TYPE_FLOAT,
      "data", NULL,
      NULL);
  g_object_set (other, 
      "dim", max_dim, 
      "shape", shape,
      "dtype", G_TYPE_FLOAT,
      "data", NULL,
      NULL);

  optic_tensor_add (self, 1.0);

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
