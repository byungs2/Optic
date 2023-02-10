#include "include/optic_collision_object.h"
#include "include/optic_tensor.h"

enum _OpticCollisionObjectSignal {
  OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT = 0,
} OpticCollisionObjectSignal;

gboolean optic_collision_object_default_is_collision (OpticCollisionObject *self, 
    OpticCollisionObject *other, gfloat threshold);

gboolean optic_collision_object_update_state_default_func (OpticCollisionObject *self);

typedef struct _OpticCollisionObjectPrivate OpticCollisionObjectPrivate;

struct _OpticCollisionObjectPrivate {
  OpticTensor *point;
  gint collision_count;
};

enum {
  PROP_0 = 0,
  PROP_COLLISION_OBJECT_POINT,
  PROP_COLLISION_OBJECT_COLLISION_COUNT
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
    case PROP_COLLISION_OBJECT_COLLISION_COUNT:
      g_value_set_int (value, priv->collision_count);
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
  gpointer pointer = NULL;
  gint64 shape[1] = { 3 }; /* TODO */
  switch (property_id) {
    case PROP_COLLISION_OBJECT_POINT:
      pointer = g_value_get_pointer (value);
      g_object_set (priv->point, 
          "dim", 1,
          "shape", shape,
          "dtype", G_TYPE_FLOAT,
          "data", pointer, NULL);
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

  g_object_class_install_property (gobject_class, 
      PROP_COLLISION_OBJECT_POINT,
      g_param_spec_pointer ("point", "Values of point",
        "Value of point", G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, 
      PROP_COLLISION_OBJECT_COLLISION_COUNT,
      g_param_spec_int ("collision-count", "Values of collision count",
        "Value of count", 0, G_MAXINT, 0, G_PARAM_READABLE));
}

static void
optic_collision_object_init (OpticCollisionObject *instance)
{
  OpticCollisionObjectPrivate *priv = optic_collision_object_get_instance_private (instance);
  priv->collision_count = 0;
  priv->point = g_object_new (OPTIC_TYPE_TENSOR, NULL);
}

void 
optic_collision_object_default_signal_callback (OpticCollisionObject *self, 
    gpointer user_data)
{
  OpticCollisionObjectPrivate *self_priv = optic_collision_object_get_instance_private (self);
  self_priv->collision_count++;
}

gboolean
optic_collision_object_default_is_collision (OpticCollisionObject *self, 
    OpticCollisionObject *other, gfloat threshold)
{
  gboolean res = 1;
  gfloat dest = 0;
  OpticCollisionObjectPrivate *self_priv = optic_collision_object_get_instance_private (self);
  OpticCollisionObjectPrivate *other_priv = optic_collision_object_get_instance_private (self);
  OpticCollisionObjectClass *klass = OPTIC_COLLISION_OBJECT_GET_CLASS (self);

  dest = optic_tensor_distance (self_priv->point, other_priv->point);

  /* TODO criteria of collision */
  if (dest <= threshold) {
    g_signal_emit (self, klass->signals[OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT], NULL);
    g_signal_emit (other, klass->signals[OPTIC_COLLISION_OBJECT_SIGNAL_DEFAULT], NULL);
    res = 1;
  }
  return res;
}

gboolean
optic_collision_object_update_state_default_func (OpticCollisionObject *self)
{
  gboolean res = 1;
  return res;
}
