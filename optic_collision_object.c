#include <stddef.h>
#include "include/optic_collision_object.h"

typedef struct _OpticCollisionObjectPrivate OpticCollisionObjectPrivate;

struct _OpticCollisionObjectPrivate {
  gint collision_count;
};

/* must change to private define */
G_DEFINE_TYPE_WITH_PRIVATE (OpticCollisionObject, optic_collision_object, G_TYPE_OBJECT)

static void
optic_collision_object_class_init (OpticCollisionObjectClass *klass)
{
  size_t default_signal_handler_offset = offsetof (OpticCollisionObjectClass, 
      collision_signal_default_handler);
  klass->collision_signal_default_handler = optic_collision_object_default_signal_callback;
  klass->update_state = optic_collision_object_update_state_default_func;
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
  OpticCollisionObjectPrivate *priv = optic_collision_object_get_instance_private (instance);
  priv->collision_count = 0;
}

void 
optic_collision_object_default_signal_callback (OpticCollisionObject *self, 
    gpointer user_data)
{
  OpticCollisionObject *other = (OpticCollisionObject *)user_data;
  OpticCollisionObjectPrivate *other_priv = optic_collision_object_get_instance_private (other);
  OpticCollisionObjectPrivate *self_priv = optic_collision_object_get_instance_private (self);
  self_priv->collision_count++;
  other_priv->collision_count++;
}

gboolean
optic_collision_object_update_state_default_func (OpticCollisionObject *self)
{
  return 1;
}
