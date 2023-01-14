#include "include/optic_queue.h"

G_DEFINE_TYPE (OpticQueue, optic_queue, OPTIC_TYPE_OBJECT)

#define OPTIC_QUEUE_LOCK(lock) pthread_mutex_lock (&lock)
#define OPTIC_QUEUE_UNLOCK(lock) pthread_mutex_unlock (&lock)

enum {
  PROP_0 = 0,
  PROP_QUEUE_LEAKY,
  PROP_QUEUE_SIZE
};

static void
optic_queue_dispose (GObject *object)
{
  G_OBJECT_CLASS (optic_queue_parent_class)->dispose (object);
}

static void
optic_queue_finalize (GObject *object)
{
  G_OBJECT_CLASS (optic_queue_parent_class)->finalize (object);
}

static void
optic_queue_set_property (GObject *object, 
    guint property_id, 
    const GValue *value, 
    GParamSpec *psepc)
{
  OpticQueue *self = OPTIC_QUEUE (object);
  switch (property_id) {
    case PROP_QUEUE_LEAKY:
      self->leaky = (guint8)g_value_get_uint (value);
      break;
    case PROP_QUEUE_SIZE:
      self->queue_size = g_value_get_uint (value);
    default:
      break;
  }
};

static void
optic_queue_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
  OpticQueue *self = OPTIC_QUEUE (object);
  switch (property_id) {
    case PROP_QUEUE_LEAKY:
      g_value_set_uint (value, (guint)self->leaky);
      break;
    case PROP_QUEUE_SIZE:
      g_value_set_uint (value, self->leaky);
      break;
    default:
      break;
  }
}

static void
optic_queue_class_init (OpticQueueClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = optic_queue_set_property;
  gobject_class->get_property = optic_queue_get_property;

  g_object_class_install_property (gobject_class, PROP_QUEUE_LEAKY,
      g_param_spec_uint ("leaky", "Leaky opt", "Leaky data queue", 0
        ,G_MAXUINT, 0, G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_QUEUE_LEAKY,
      g_param_spec_uint ("size", "queue max size", "size of queue", 0
        ,G_MAXUINT, DEFAULT_QUEUE_SIZE, G_PARAM_READWRITE));

  gobject_class->finalize = optic_queue_finalize;
  gobject_class->dispose = optic_queue_dispose;
}

static void
optic_queue_init (OpticQueue *instance)
{
  instance->head = 0;
  instance->tail = 0;
  instance->leaky = 0;
  pthread_mutex_init (&instance->lock, NULL);
}

gboolean 
optic_queue_push (OpticQueue *queue, gpointer data) 
{
  OPTIC_QUEUE_LOCK (queue->lock);
  if ((queue->head - queue->tail) == 1) {
    OPTIC_QUEUE_UNLOCK (queue->lock);
    return 0;
  }
  queue->queue[queue->tail] = data;
  queue->tail = (queue->tail + 1)%queue->queue_size;
  OPTIC_QUEUE_UNLOCK (queue->lock);
  return 1;
}

gpointer
optic_queue_pop (OpticQueue *queue)
{
  gpointer data = NULL;
  OPTIC_QUEUE_LOCK (queue->lock);
  if (queue->head == queue->tail) {
    OPTIC_QUEUE_UNLOCK (queue->lock);
    return NULL;
  }
  data = queue->queue[queue->head];
  queue->queue[queue->head] = NULL;
  queue->head = (queue->head + 1)%queue->queue_size;
  OPTIC_QUEUE_UNLOCK (queue->lock);
  return data;
}

