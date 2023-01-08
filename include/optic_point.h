#ifndef __OPTIC_POINT_H__
#define __OPTIC_POINT_H__

#include "common.h"
#include "optic_object.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_POINT (optic_point_get_type ())
#define OPTIC_POINT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_POINT, OpticPoint))
#define OPTIC_POINT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_POINT, OpticPointClass))
#define OPTIC_IS_POINT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_POINT))
#define OPTIC_IS_POINT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_POINT))

typedef struct _OpticPointClass OpticPointClass;
typedef struct _OpticPoint OpticPoint;

struct _OpticPointClass {
  OpticObjectClass parent_class;
};

struct _OpticPoint {
  OpticObject parent_instance;
  gint dim_size;
  gint dtype;
  gfloat *dim; /* for now only takes float */
};

GType optic_point_get_type (void);

/* cal euclidean distance */
gfloat optic_point_distance_256 (OpticPoint *self, OpticPoint *other);

gfloat optic_point_distance (OpticPoint *self, OpticPoint *other); 

void optic_point_mul (OpticPoint *self, gfloat constant);

void optic_point_div (OpticPoint *self, gfloat constant);

void optic_point_add (OpticPoint *self, gfloat constant);

G_END_DECLS
#endif
