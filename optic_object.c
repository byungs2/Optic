#include "include/optic_object.h"

typedef struct _OpticObjectPrivate OpticObjectPrivate;

struct _OpticObjectPrivate {
  guint id;
};

G_DEFINE_TYPE_WITH_PRIVATE (OpticObject, optic_object, G_TYPE_OBJECT);

static void 
optic_object_dispose (GObject *object)
{
  G_OBJECT_CLASS (optic_object_parent_class)->dispose (object); 
}

static void
optic_object_finalize (GObject *object)
{
  G_OBJECT_CLASS (optic_object_parent_class)->finalize (object);
}

static void
optic_object_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
}

static void
optic_object_set_property (GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec)
{
}

static void
optic_object_class_init (OpticObjectClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->get_property = optic_object_get_property;
  object_class->set_property = optic_object_set_property;
  object_class->finalize = optic_object_finalize;
  object_class->dispose = optic_object_dispose;
  klass->update_state = optic_object_update_state_default_func;
}

static void
optic_object_init (OpticObject *instance)
{

}

gboolean 
optic_object_update_state_default_func (OpticObject *self)
{
  return TRUE;
}
