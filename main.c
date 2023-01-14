#include "include/optic_tensor.h"
#include "include/optic_queue.h"
#include "include/optic_threadpool.h"
#include <time.h>

int
main (int argc, char *argv[])
{
  gint64 time, max_dim;
  gfloat diff1, diff2, dist;
  guint64 shape[1] = { 50000 };
  guint64 max_iter, iter;
  OpticTensor *self = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *other = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *tensor_list[2];
  OpticThreadPool *threadpool = g_object_new (OPTIC_TYPE_THREADPOOL, NULL);
  OpticThreadPoolWork work_;

  max_dim = 1;
  max_iter = 255;
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

  g_object_set (threadpool,
      "num_thread", 4, NULL);

  optic_threadpool_hire_workers (threadpool);
  optic_tensor_add (self, 1.0);

  tensor_list[0] = self;
  tensor_list[1] = other;

  time = g_get_real_time ();
  for (iter = 0; iter < max_iter; iter++) {
    dist = optic_tensor_distance (self, other);
  }
  diff2 = ((g_get_real_time () - time)/1000.0);
  g_print ("AVX 256 TIME %f\n", diff2);

  g_object_unref (self);
  g_object_unref (other);
  return 0;
}
