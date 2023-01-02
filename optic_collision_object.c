#include <stddef.h>
#include "include/optic_collision_object.h"

/* must change to private define */
G_DEFINE_TYPE (OpticCollisionObject, optic_collision_object, G_TYPE_OBJECT)

static void
optic_collision_object_class_init (OpticCollisionObjectClass *klass)
{
  size_t default_signal_handler_offset = offsetof (OpticCollisionObjectClass, 
      collision_signal_default_handler);
  klass->collision_signal_default_handler = optic_collision_object_default_signal_callback;
  klass->signals[OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT] = g_signal_new ("collision",
      G_TYPE_FROM_CLASS (klass),
      G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
      default_signal_handler_offset, 
      NULL,
      NULL,
      NULL,
      G_TYPE_NONE,
      1,
      G_TYPE_POINTER
      );
}

static void
optic_collision_object_init (OpticCollisionObject *instance)
{
  instance->collision_count = 0;
}

void 
optic_collision_object_default_signal_callback (OpticCollisionObject *self, 
    gpointer user_data)
{
  OpticCollisionObject *other = (OpticCollisionObject *)user_data;
  self->collision_count++;
  other->collision_count++;
}
