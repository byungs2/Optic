#ifndef __OPTIC_OBJECT_H__
#define __OPTIC_OBJECT_H__

#include "common.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_OBJECT (optic_object_get_type ())
#define OPTIC_OBJECT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_OBJECT, OpticObject))
#define OPTIC_OBJECT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_OBJECT, OpticObjectClass))
#define OPTIC_IS_OBJECT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_OBJECT))
#define OPTIC_IS_OBJECT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_OBJECT))

#define OPTIC_OBJECT_MAX_SIGNAL_COUNT 12

typedef struct _OpticObjectClass OpticObjectClass;
typedef struct _OpticObject OpticObject;

struct _OpticObjectClass {
  GObjectClass parent_class;
  gboolean (*update_state) (OpticObject *);
};

struct _OpticObject {
  GObject parent_instance;
};

GType optic_object_get_type (void);

G_END_DECLS
#endif
