#ifndef __OPTIC_THREADPOOL_H__
#define __OPTIC_THREADPOOL_H__

#include "common.h"
#include "optic_object.h"
#include "optic_queue.h"
#include <pthread.h>

G_BEGIN_DECLS

#define OPTIC_TYPE_THREADPOOL (optic_threadpool_get_type ())
#define OPTIC_THREADPOOL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_THREADPOOL, OpticThreadPool))
#define OPTIC_THREADPOOL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_THREADPOOL, OpticThreadPoolClass))
#define OPTIC_IS_THREADPOOL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_THREADPOOL))
#define OPTIC_IS_THREADPOOL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_THREADPOOL))

typedef struct _OpticThreadPoolClass OpticThreadPoolClass;
typedef struct _OpticThreadPool OpticThreadPool;
typedef struct _OpticThreadPoolWork OpticThreadPoolWork;
typedef void (*FUNCPTR) (gpointer);

struct _OpticThreadPoolClass {
  OpticObjectClass parent_class;
};

struct _OpticThreadPool {
  OpticObject parent_instance;
  OpticQueue queue;
  pthread_t *workers;
  pthread_mutex_t tp_lock;
  pthread_cond_t cond;
  guint thread_count;
};

struct _OpticThreadPoolWork {
  FUNCPTR work;
  gpointer arg;
};

GType optic_threadpool_get_type (void);

void optic_threadpool_push_work (OpticThreadPool *self, OpticThreadPoolWork *work);

G_END_DECLS
#endif
