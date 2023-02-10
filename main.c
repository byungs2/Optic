#include "include/optic_tensor.h"
#include "include/optic_queue.h"
#include "include/optic_threadpool.h"
#include "include/optic_voxel.h"
#include <time.h>

void thread_test (gpointer data);

void
thread_test (gpointer data)
{
  gfloat dist;
  OpticTensor **tensor = (OpticTensor **)data;
  dist = optic_tensor_distance (tensor[0], tensor[1]);
}

int
main (int argc, char *argv[])
{
  gint64 time, max_dim;
  gfloat diff1, diff2, dist;
  guint64 shape[1] = { 3 };
  guint64 max_iter, iter;
  OpticTensor *self = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *other = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  OpticTensor *tensor_list[2];
  OpticThreadPool *threadpool = g_object_new (OPTIC_TYPE_THREADPOOL, NULL);
  OpticVoxel **voxels = NULL;
  GMainLoop *loop = NULL;
  gboolean res = 0;
  gint a, b;

  voxels = (OpticVoxel **)g_malloc0 (sizeof (OpticVoxel *) * 2);
  for (iter = 0; iter < 2; iter++) {
    voxels[iter] = g_object_new (OPTIC_TYPE_VOXEL, NULL); 
    g_object_set (voxels[iter],
        "point",
        NULL,
        NULL); /* set point 0 */
  }

  res = optic_voxel_is_collision (G_OBJECT (voxels[0]), G_OBJECT (voxels[1]), 0.2);
  if (res) {
    g_object_get (voxels[0], "collision-count", &a, NULL);
    g_object_get (voxels[1], "collision-count", &b, NULL);
    g_print ("COLISION %d %d\n", a, b);
  }

  max_dim = 1;
  max_iter = 1;
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
      "num-thread", 4, 
      "work-func", thread_test, 
      NULL);

  optic_threadpool_hire_workers (threadpool);
  optic_tensor_add (self, 1.0);

  tensor_list[0] = self;
  tensor_list[1] = other;

  time = g_get_real_time ();
  for (iter = 0; iter < max_iter; iter++) {
    dist = optic_tensor_distance (self, other);
  }
  diff2 = ((g_get_real_time () - time)/1000.0);
  g_print ("AVX 256 TIME %f %f\n", diff2, dist);

  time = g_get_real_time ();
  for (iter = 0; iter < max_iter; iter++) {
    dist = optic_tensor_dot (self, other);
  }
  diff2 = ((g_get_real_time () - time)/1000.0);
  g_print ("AVX 256 TIME DOT %f %f\n", diff2, dist);

  for (iter = 0; iter < max_iter; iter++) {
    optic_threadpool_push_work (threadpool, (gpointer)tensor_list);
  }

  g_object_unref (self);
  g_object_unref (other);
  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);
  return 0;
}
