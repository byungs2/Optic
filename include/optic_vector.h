#ifndef __OPTIC_VECTOR_H__
#define __OPTIC_VECTOR_H__

#include "common.h"
#include "optic_object.h"
#include "optic_vector.h"
#include "optic_point.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_VECTOR (optic_vector_get_type ())
#define OPTIC_VECTOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_VECTOR, OpticVector))
#define OPTIC_VECTOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_VECTOR, OpticVectorClass))
#define OPTIC_IS_VECTOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_VECTOR))
#define OPTIC_IS_VECTOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_VECTOR))

typedef struct _OpticVectorClass OpticVectorClass;
typedef struct _OpticVector OpticVector;

struct _OpticVectorClass {
  OpticObjectClass parent_class;
};

struct _OpticVector {
  OpticObject parent_instacne;
  OpticPoint *direction;
  gdouble length;
};

GType optic_vector_get_type (void);

G_END_DECLS
#endif
