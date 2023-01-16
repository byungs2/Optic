#include "include/optic_threadpool.h"

#define OPTIC_THREADPOOL_LOCK(lock) pthread_mutex_lock (&lock)
#define OPTIC_THREADPOOL_UNLOCK(lock) pthread_mutex_unlock (&lock)
#define OPTIC_THREADPOOL_WAIT(cond, lock) pthread_cond_wait (&cond, &lock)
#define OPTIC_THREADPOOL_WAKE(cond) pthread_cond_signal (&cond)

G_DEFINE_TYPE (OpticThreadPool, optic_threadpool, OPTIC_TYPE_OBJECT);

void *optic_threadpool_default_loop (gpointer arg);

enum {
  PROP_0 = 0,
  PROP_THREADPOOL_NUM_THREAD,
  PROP_THREADPOOL_LOOP_FUNCPTR
};

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
    case PROP_THREADPOOL_NUM_THREAD:
      self->thread_count = g_value_get_int (value); 
      break;
    case PROP_THREADPOOL_LOOP_FUNCPTR:
      self->loop  = g_value_get_pointer (value);
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
  guint i;
  OpticThreadPool *self = OPTIC_THREADPOOL (object);
  switch (property_id) {
    case PROP_THREADPOOL_NUM_THREAD:
      g_value_set_int (self->thread_count, value);
      break;
    case PROP_THREADPOOL_LOOP_FUNCPTR:
      g_value_set_pointer (value, self->loop);
      break;
    default:
      break;
  }
}

static void
optic_threadpool_class_init (OpticThreadPoolClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = optic_threadpool_set_property;
  gobject_class->get_property = optic_threadpool_get_property;

  g_object_class_install_property (gobject_class, 
      PROP_THREADPOOL_NUM_THREAD,
      g_param_spec_int ("num_thread", "Number of threads in threadpool",
        "Number of thread", 0, G_MAXINT, 10, G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, 
      PROP_THREADPOOL_LOOP_FUNCPTR,
      g_param_spec_pointer ("loop_func", "loop func pointer",
        "Func ptr", G_PARAM_READWRITE));

  gobject_class->finalize = optic_threadpool_finalize;
  gobject_class->finalize = optic_threadpool_dispose;
}

static void
optic_threadpool_init (OpticThreadPool *instance)
{
  instance->workers = NULL;
  instance->thread_count = 0;
  instance->loop = optic_threadpool_default_loop;
  instance->queue = g_object_new (OPTIC_TYPE_QUEUE, NULL);
  pthread_cond_init (&instance->cond, NULL);
  pthread_mutex_init (&instance->tp_lock, NULL);
}

void
optic_threadpool_hire_workers (OpticThreadPool *self)
{
  guint i;
  self->workers = (pthread_t *)g_malloc0 (sizeof(pthread_t) * self->thread_count);
  for (i = 0; i < self->thread_count; i++) {
      pthread_create (&self->workers[i], NULL, self->loop, (gpointer)self);
  }
}

void
optic_threadpool_fire_workers (OpticThreadPool *self)
{
  for (; self->thread_count > 0; self->thread_count--) {
    pthread_exit (self->workers);
    self->workers++;
  }
  self->workers = NULL;
}

gboolean
optic_threadpool_push_work (OpticThreadPool *self, OpticThreadPoolWork *work)
{
  gboolean result = 0;
  result = optic_queue_push (self->queue, (gpointer)work);
  if (result) {
    OPTIC_THREADPOOL_WAKE (self->cond);
  } else {
    /* TODO block thread properly  */
    while (1) {
      if (optic_queue_is_empty (self->queue)) {
        optic_queue_push (self->queue, (gpointer)work);
        OPTIC_THREADPOOL_WAKE (self->cond);
        break;
      }
    }
  }
  return result;
}

void*
optic_threadpool_default_loop (gpointer arg)
{
  OpticThreadPoolWork *threadpool_work;
  OpticThreadPool *threadpool = (OpticThreadPool *)arg;
  while (1) {
    threadpool_work = (OpticThreadPoolWork *)optic_queue_pop (threadpool->queue);
    if (threadpool_work == NULL) {
      OPTIC_THREADPOOL_LOCK (threadpool->tp_lock);
      OPTIC_THREADPOOL_WAIT(threadpool->cond, threadpool->tp_lock);
      OPTIC_THREADPOOL_UNLOCK (threadpool->tp_lock);
    } else {
      threadpool_work->work (threadpool_work->data);
    }
  }
  return NULL;
}
