#ifndef __OPTIC_TENSOR_H__
#define __OPTIC_TENSOR_H__

#include "common.h"
#include "optic_object.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_TENSOR (optic_tensor_get_type ())
#define OPTIC_TENSOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_TENSOR, OpticTensor))
#define OPTIC_TENSOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_TENSOR, OpticTensorClass))
#define OPTIC_IS_TENSOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_TENSOR))
#define OPTIC_IS_TENSOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_TENSOR))

typedef struct _OpticTensorClass OpticTensorClass;
typedef struct _OpticTensor OpticTensor;
typedef struct _OpticTensorTemplate OpticTensorTemplate;

struct _OpticTensorTemplate {
  const gchar *shape;
  guint dtype;
};

struct _OpticTensorClass {
  OpticObjectClass parent_class;
};

struct _OpticTensor {
  OpticObject parent_instance;
};

GType optic_tensor_get_type (void);

/* cal euclidean distance */
gfloat optic_tensor_distance (OpticTensor *self, OpticTensor *other); 

OpticTensor *optic_tensor_get_farthest_point_in_direction (OpticTensor *self, OpticTensor *direction);

OpticTensor *optic_tensor_get_minkowski_difference (OpticTensor *self, OpticTensor *other, OpticTensor *direction);

OpticTensor *optic_tensor_subtract (OpticTensor *p1, OpticTensor *p2);

OpticTensor *optic_tensor_new_with_template (gpointer data, OpticTensorTemplate *tmpl);

void optic_tensor_negative (OpticTensor *self);

void optic_tensor_mul_scalar (OpticTensor *self, gfloat constant);

void optic_tensor_div_scalar (OpticTensor *self, gfloat constant);

void optic_tensor_add_scalar (OpticTensor *self, gfloat constant);

gfloat optic_tensor_dot (OpticTensor *self, OpticTensor *other);

void optic_tensor_cross (OpticTensor *self, OpticTensor *other, OpticTensor *dest);

void optic_tensor_outer (OpticTensor *self, OpticTensor *other, OpticTensor *dest);

G_END_DECLS
#endif
