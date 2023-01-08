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

struct _OpticTensorClass {
  OpticObjectClass parent_class;
};

struct _OpticTensor {
  OpticObject parent_instance;
  gfloat *tensor; /* for now only takes float */
  guint *shape;
  guint dim_size;
  guint dtype;
};

GType optic_tensor_get_type (void);

/* cal euclidean distance */
gfloat optic_tensor_distance (OpticTensor *self, OpticTensor *other); 

void optic_tensor_mul (OpticTensor *self, gfloat constant);

void optic_tensor_div (OpticTensor *self, gfloat constant);

void optic_tensor_add (OpticTensor *self, gfloat constant);

G_END_DECLS
#endif
