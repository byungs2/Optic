#ifndef __OPTIC_QUEUE_H__
#define __OPTIC_QUEUE_H__

#include "common.h"
#include "optic_object.h"
#include <pthread.h>

G_BEGIN_DECLS

#define OPTIC_TYPE_QUEUE (optic_queue_get_type ())
#define OPTIC_QUEUE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_QUEUE, OpticQueue))
#define OPTIC_QUEUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_QUEUE, OpticQueueClass))
#define OPTIC_IS_QUEUE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_QUEUE))
#define OPTIC_IS_QUEUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_QUEUE))

#define DEFAULT_QUEUE_SIZE 255

typedef struct _OpticQueueClass OpticQueueClass;
typedef struct _OpticQueue OpticQueue;

struct _OpticQueueClass {
  OpticObjectClass parent_class;
};

struct _OpticQueue {
  OpticObject parent_instance;
  pthread_mutex_t lock;
  gpointer queue[DEFAULT_QUEUE_SIZE];
  guint queue_size;
  guint8 head, tail;
  guint8 leaky;
};

GType optic_queue_get_type (void);

gboolean optic_queue_push (OpticQueue *queue, gpointer data);

gpointer optic_queue_pop (OpticQueue *queue);

G_END_DECLS
#endif
