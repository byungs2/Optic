#include "include/optic_queue.h"

G_DEFINE_TYPE (OpticQueue, optic_queue, OPTIC_TYPE_OBJECT)

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
};

static void
optic_queue_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
}

static void
optic_queue_class_init (OpticQueueClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = optic_queue_set_property;
  gobject_class->get_property = optic_queue_get_property;

  gobject_class->finalize = optic_queue_finalize;
  gobject_class->dispose = optic_queue_dispose;
}

static void
optic_queue_init (OpticQueue *instance)
{
  instance->head = 0;
  instance->tail = 0;
}

gboolean 
optic_queue_push (OpticQueue *queue, gpointer data) 
{
  g_assert ((queue->head - queue->tail) != 1);
  queue->queue[queue->tail] = data;
  queue->tail = (queue->tail + 1)%MAX_QUEUE_SIZE;
  return 1;
}

gpointer
optic_queue_pop (OpticQueue *queue)
{
  gpointer data = NULL;
  if (queue->head == queue->tail) {
    return NULL;
  }
  data = queue->queue[queue->head];
  queue->queue[queue->head] = NULL;
  queue->head = (queue->head + 1)%MAX_QUEUE_SIZE;
  return data;
}

