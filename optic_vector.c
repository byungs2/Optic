#include <math.h>
#include "include/optic_vector.h"

G_DEFINE_TYPE (OpticVector, optic_vector, OPTIC_TYPE_OBJECT)

static void
optic_vector_dispose (GObject *object)
{
  G_OBJECT_CLASS (optic_vector_parent_class)->dispose (object);
}

static void
optic_vector_finalize (GObject *object)
{
  G_OBJECT_CLASS (optic_vector_parent_class)->finalize (object);
}

static void
optic_vector_set_property (GObject *object, 
    guint property_id, 
    const GValue *value, 
    GParamSpec *psepc)
{
  int i;
  OpticVector *self = OPTIC_VECTOR (object);
};

static void
optic_vector_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
  OpticVector *self = OPTIC_VECTOR (object);
}

static void
optic_vector_class_init (OpticVectorClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = optic_vector_set_property;
  gobject_class->get_property = optic_vector_get_property;

  /*
  g_object_class_install_property (gobject_class, PROP_D_TYPE,
      g_param_spec_int ("dtype", "Type of data",
        "Type of data", 0, G_MAXINT, 0, G_PARAM_READWRITE));
  */

  gobject_class->finalize = optic_vector_finalize;
  gobject_class->dispose = optic_vector_dispose;
}

static void
optic_vector_init (OpticVector *instance)
{
}
