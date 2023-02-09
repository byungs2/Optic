#include "include/optic_collision_object.h"
#include "include/optic_tensor.h"

enum _OpticCollisionObjectSignal {
  OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT = 0,
} OpticCollisionObjectSignal;

gboolean optic_collision_object_default_is_collision (OpticCollisionObject *self, OpticCollisionObject *other);

gboolean optic_collision_object_update_state_default_func (OpticCollisionObject *self);

typedef struct _OpticCollisionObjectPrivate OpticCollisionObjectPrivate;

struct _OpticCollisionObjectPrivate {
  OpticTensor *point;
  gint collision_count;
};

enum {
  PROP_0 = 0,
  PROP_COLLISION_OBJECT_POINT
};

G_DEFINE_TYPE_WITH_PRIVATE (OpticCollisionObject, optic_collision_object, G_TYPE_OBJECT)

static void 
optic_collision_object_dispose (GObject *object)
{
  G_OBJECT_CLASS (optic_collision_object_parent_class)->dispose (object); 
}

static void
optic_collision_object_finalize (GObject *object)
{
  G_OBJECT_CLASS (optic_collision_object_parent_class)->finalize (object);
}

static void
optic_collision_object_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
  OpticCollisionObject *self = OPTIC_COLLISION_OBJECT (object);
  OpticCollisionObjectPrivate *priv = optic_collision_object_get_instance_private (self);
  gpointer pointer = NULL;
  switch (property_id) {
    case PROP_COLLISION_OBJECT_POINT:
      g_object_get (priv->point, "data", pointer, NULL);
      g_value_set_pointer (value, pointer);
      break;
    default:
      break;
  }
}

static void
optic_collision_object_set_property (GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec)
{
  OpticCollisionObject *self = OPTIC_COLLISION_OBJECT (object);
  OpticCollisionObjectPrivate *priv = optic_collision_object_get_instance_private (self);
  switch (property_id) {
    case PROP_COLLISION_OBJECT_POINT:
      g_object_set (priv->point, "data", value, NULL);
      break;
    default:
      break;
  }
}

static void
optic_collision_object_class_init (OpticCollisionObjectClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  size_t default_signal_handler_offset = 0;

  gobject_class->set_property = optic_collision_object_set_property;
  gobject_class->get_property = optic_collision_object_get_property;

  klass->collision_signal_default_handler = optic_collision_object_default_signal_callback;
  klass->signals[OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT] = g_signal_new ("collision",
      G_TYPE_FROM_CLASS (klass),
      G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
      G_STRUCT_OFFSET (OpticCollisionObjectClass, collision_signal_default_handler),
      NULL,
      NULL,
      NULL,
      G_TYPE_NONE,
      0
      );
  klass->update_state = optic_collision_object_update_state_default_func;
  klass->is_collision = optic_collision_object_default_is_collision;
}

static void
optic_collision_object_init (OpticCollisionObject *instance)
{
  OpticCollisionObjectPrivate *priv = optic_collision_object_get_instance_private (instance);
  priv->collision_count = 0;
  priv->point = NULL;
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
  g_print ("collision %d %d\n", self_priv->collision_count, other_priv->collision_count);
}

gboolean
optic_collision_object_default_is_collision (OpticCollisionObject *self, OpticCollisionObject *other)
{
  gboolean res = 1;
  gfloat dest = 0;
  OpticCollisionObjectPrivate *self_priv = optic_collision_object_get_instance_private (self);
  OpticCollisionObjectPrivate *other_priv = optic_collision_object_get_instance_private (self);
  OpticCollisionObjectClass *klass = OPTIC_COLLISION_GET_CLASS (self);

  /* TODO criteria of collision */
  if (res) {
    g_print ("SIGNAL EMIT\n");
    g_signal_emit (self, klass->signals[OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT], NULL);
  }
  return res;
}

void 
optic_collision_object_is_collision (OpticCollisionObject *self, OpticCollisionObject *other)
{
  OpticCollisionObjectClass *klass = OPTIC_COLLISION_GET_CLASS (self);
  klass->is_collision (self, other);
}

gboolean
optic_collision_object_update_state_default_func (OpticCollisionObject *self)
{
  gboolean res = 1;
  return res;
}
