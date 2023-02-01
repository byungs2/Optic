#ifndef __OPTIC_COLLISION_OBJECT_H__
#define __OPTIC_COLLISION_OBJECT_H__

#include "common.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_COLLISION_OBJECT (optic_collision_object_get_type ())
#define OPTIC_COLLISION_OBJECT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_COLLISION_OBJECT, OpticCollisionObject))
#define OPTIC_COLLISION_OBJECT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_COLLISION_OBJECT, OpticCollisionObjectClass))
#define OPTIC_IS_COLLISION_OBJECT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_COLLISION_OBJECT))
#define OPTIC_IS_COLLISION_OBJECT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_COLLISION_OBJECT))

#define OPTIC_COLLISION_OBJECT_MAX_SIGNAL_COUNT 12

typedef struct _OpticCollisionObjectClass OpticCollisionObjectClass;
typedef struct _OpticCollisionObject OpticCollisionObject;

struct _OpticCollisionObjectClass {
  GObjectClass parent_class;
  guint signals[OPTIC_COLLISION_OBJECT_MAX_SIGNAL_COUNT];
  void (*collision_signal_default_handler) (OpticCollisionObject *, gpointer);
  void (*is_collision) (OpticCollisionObject *, OpticCollisionObject *);
};

struct _OpticCollisionObject {
  GObject parent_instance;
};

enum _OpticCollisionObjectSignal {
  OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT = 0,
} OpticCollisionObjectSignal;

GType optic_collision_object_get_type (void);

void optic_collision_object_default_signal_callback (OpticCollisionObject *self,
    gpointer user_data);

void optic_collision_object_is_collision (OpticCollisionObject *self, OpticCollisionObject *other);

G_END_DECLS
#endif
