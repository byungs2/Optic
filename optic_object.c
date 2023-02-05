#include "include/optic_object.h"

typedef struct _OpticObjectPrivate OpticObjectPrivate;

struct _OpticObjectPrivate {

};

/* must change to with private define */
G_DEFINE_TYPE_WITH_PRIVATE (OpticObject, optic_object, G_TYPE_OBJECT);

static void
optic_object_class_init (OpticObjectClass *klass)
{
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
