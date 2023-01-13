#include <pthread.h>
#include "include/optic_threadpool.h"

G_DEFINE_TYPE (OpticThreadPool, optic_threadpool, OPTIC_TYPE_THREADPOOL);

#define OPTIC_THREADPOOL_LOCK (lock) pthread_mutex_lock (&lock)
#define OPTIC_THREADPOOL_UNLOCK (lock) pthread_mutex_unlock (&lock)

enum {
  PROP_0 = 0,
  PROP_THREADPOOL_TP_COUNT
};

void *loop (void *arg);

void*
loop (void *arg)
{
  OpticThreadPoolWork *work;
  OpticThreadPool *threadpool = (OpticThreadPool *)arg;
  while (1) {
    work = (OpticThreadPoolWork *)optic_queue_pop (&threadpool->queue);
    work->work (arg);
  }
  return NULL;
}

static void
optic_threadpool_dispose (GObject *object)
{
  G_OBJECT_CLASS (optic_threadpool_parent_class)->dispose (object);
}

static void
optic_threadpool_finalize (GObject *object)
{
  G_OBJECT_CLASS (optic_threadpool_parent_class)->finalize (object);
}

static void
optic_threadpool_set_property (GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec)
{
  guint i;
  OpticThreadPool *self = OPTIC_THREADPOOL (object);
  switch (property_id) {
    case PROP_THREADPOOL_TP_COUNT:
      self->thread_count = g_value_get_int (value);
      self->workers = (pthread_t *)g_malloc0 (sizeof(pthread_t) * self->thread_count);
      break;
    default:
      break;
  }
}

static void
optic_threadpool_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
}

static void
optic_threadpool_class_init (OpticThreadPoolClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = optic_threadpool_set_property;
  gobject_class->get_property = optic_threadpool_get_property;

  g_object_class_install_property (gobject_class, 
      PROP_THREADPOOL_TP_COUNT,
      g_param_spec_int ("num_threads", "Number of threads in threadpool",
        "Number of thread", 0, G_MAXINT, 10, G_PARAM_READWRITE));

  gobject_class->finalize = optic_threadpool_finalize;
  gobject_class->finalize = optic_threadpool_dispose;
}

static void
optic_threadpool_init (OpticThreadPool *instance)
{
  instance->workers = NULL;
  instance->thread_count = 0;
  pthread_cond_init (&instance->cond, NULL);
  pthread_mutex_init (&instance->tp_lock, NULL);
}

