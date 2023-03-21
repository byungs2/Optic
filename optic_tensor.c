#include <math.h>
#include <immintrin.h>
#include "include/optic_tensor.h"

G_DEFINE_TYPE (OpticTensor, optic_tensor, OPTIC_TYPE_OBJECT)

enum {
  PROP_0 = 0,
  PROP_TENSOR_DIM,
  PROP_TENSOR_SHAPE,
  PROP_TENSOR_TYPE,
  PROP_TENSOR_DATA
};

/* TODO need to free mallocs in here */
static void
optic_tensor_dispose (GObject *object)
{
  G_OBJECT_CLASS (optic_tensor_parent_class)->dispose (object);
}

static void
optic_tensor_finalize (GObject *object)
{
  G_OBJECT_CLASS (optic_tensor_parent_class)->finalize (object);
}

static void
optic_tensor_set_property (GObject *object, 
    guint property_id, 
    const GValue *value, 
    GParamSpec *pspec)
{
  gint i;
  gpointer pointer;
  OpticTensor *self = OPTIC_TENSOR (object);
  switch (property_id) {
    case PROP_TENSOR_DIM:
      self->dim_size = g_value_get_int (value);
      if (self->shape != NULL) {
        g_free (self->shape);
        self->shape = NULL;
      }
      self->shape = (guint64 *)g_malloc0 (sizeof(guint64) * self->dim_size);
      break;
    case PROP_TENSOR_DATA:
      pointer = g_value_get_pointer (value);
      if (self->tensor != NULL) {
        g_free (self->tensor);
        self->tensor = NULL;
      }
      /* TODO maximum single malloc size is INT 
       * need to make fragment of tensor array
       * as its shape
       * */
      self->tensor = (gfloat *)g_malloc0 (sizeof(gfloat) * self->length);
      if (pointer != NULL) {
        for (i = 0; i < self->length; i++) {
          self->tensor[i] = ((gfloat *)pointer)[i];
        }
      }
      break;
    case PROP_TENSOR_SHAPE:
      pointer = g_value_get_pointer (value);
      self->length = 1;
      for (i = 0; i < self->dim_size; i++) {
        self->shape[i] = ((guint64 *)pointer)[i];
        self->length *= self->shape[i];
      }
      break;
    case PROP_TENSOR_TYPE:
      self->dtype = g_value_get_int (value);
      break;
    default:
      break;
  }
};

static void
optic_tensor_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
  OpticTensor *self = OPTIC_TENSOR (object);
  switch (property_id) {
    case PROP_TENSOR_DIM:
      g_value_set_int (value, self->dim_size);
      break;
    case PROP_TENSOR_DATA:
      g_value_set_pointer (value, self->tensor);
      break;
    case PROP_TENSOR_SHAPE:
      g_value_set_pointer (value, self->shape);
      break;
    case PROP_TENSOR_TYPE:
      g_value_set_int (value, self->dtype);
      break;
    default:
      break;
  }
}

static void
optic_tensor_class_init (OpticTensorClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = optic_tensor_set_property;
  gobject_class->get_property = optic_tensor_get_property;

  g_object_class_install_property (gobject_class, PROP_TENSOR_DIM,
      g_param_spec_int ("dim", "Tensor dimentsion number",
        "Tensor dimension number", 0, G_MAXINT, 3, G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_TENSOR_DATA,
      g_param_spec_pointer ("data", "Value of tensor",
        "Value of tensor", G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_TENSOR_TYPE,
      g_param_spec_int ("dtype", "Type of data",
        "Type of data", 0, G_MAXINT, 0, G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_TENSOR_SHAPE,
      g_param_spec_pointer ("shape", "Shape of tensor",
        "Shape of data", G_PARAM_READWRITE));

  gobject_class->finalize = optic_tensor_finalize;
  gobject_class->dispose = optic_tensor_dispose;
}

static void
optic_tensor_init (OpticTensor *instance)
{
  instance->dim_size = 0;
  instance->length = 0;
  instance->dtype = G_TYPE_FLOAT;
  instance->shape = NULL;
  instance->tensor = NULL;
}

/* TODO is it useful? */
gfloat 
optic_tensor_distance (OpticTensor *self, OpticTensor *other)
{
  gint i = 0, iter = 0;
  gfloat res = 0.0, tmp = 0.0;
  gfloat *tmp_self = NULL, *tmp_other = NULL;
  __m256 self_tensor, other_tensor, dest, distance;

  /* TODO need to check shape too */
  g_assert (self->dim_size == other->dim_size 
      && self->tensor != NULL && other->tensor != NULL);

  iter = self->length/8;

  tmp_self = (gfloat *)self->tensor;
  tmp_other = (gfloat *)other->tensor;
  distance = _mm256_set1_ps (0.0);

  for (i = 0; i < iter; i++) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    other_tensor = _mm256_loadu_ps (tmp_other);
    dest = _mm256_sub_ps (self_tensor, other_tensor);
    dest = _mm256_mul_ps (dest, dest);
    distance = _mm256_add_ps (dest, distance);

    tmp_self += 8;
    tmp_other += 8;
  }
  for (i = 0; iter > 0 && i < 8; i++) {
    res += ((gfloat *)&distance)[i];
  }

  i = self->length%8;
  for (; i > 0; i--) {
    tmp = tmp_self[i-1] - tmp_other[i-1];
    tmp *= tmp;
    res += tmp;
  }
  
  return sqrt(res);
}

OpticTensor *
optic_tensor_get_minkowski_difference (OpticTensor *self, OpticTensor *other)
{
  OpticTensor *minkowski_difference = NULL;
  return minkowski_difference;
}

gfloat
optic_tensor_dot (OpticTensor *self, OpticTensor *other)
{
  gint i = 0, iter = 0;
  gfloat res = 0.0;
  gfloat tmp = 0.0;
  gfloat *tmp_self = NULL, *tmp_other = NULL;
  __m256 self_tensor, other_tensor, dest, mul;

  /* TODO need to check shape too */
  g_assert (self->dim_size == other->dim_size 
      && self->tensor != NULL && other->tensor != NULL);

  iter = self->length/8;

  tmp_self = (gfloat *)self->tensor;
  tmp_other = (gfloat *)other->tensor;
  dest = _mm256_set1_ps (0.0);

  for (i = 0; i < iter; i++) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    other_tensor = _mm256_loadu_ps (tmp_other);
    mul = _mm256_mul_ps (self_tensor, other_tensor);
    dest = _mm256_add_ps (dest, mul);

    tmp_self += 8;
    tmp_other += 8;
  }
  for (i = 0; iter > 0 && i < 8; i++) {
    res += ((gfloat *)&dest)[i];
  }

  i = self->length%8;
  for (; i > 0; i--) {
    tmp = tmp_self[i-1] * tmp_other[i-1];
    res += tmp;
  }

  return res;
}

void
optic_tensor_cross (OpticTensor *self, OpticTensor *other, OpticTensor *dest)
{

}

void
optic_tensor_outer (OpticTensor *self, OpticTensor *other, OpticTensor *dest)
{

}

void 
optic_tensor_mul (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;

  g_assert (self->tensor != NULL);

  i = self->length/8;

  tmp_self = (gfloat *)self->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_mul_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }

  i = self->length%8;
  for (; i > 0; i--) {
    tmp_self[i-1] *= constant;
  }
}

void 
optic_tensor_div (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;

  g_assert (constant != 0);

  i = self->length/8;

  tmp_self = (gfloat *)self->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_div_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }

  i = self->length%8;
  for (; i > 0; i--) {
    tmp_self[i-1] /= constant;
  }
}

void 
optic_tensor_add (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;

  g_assert (constant != 0);

  i = self->length/8;

  tmp_self = (gfloat *)self->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_add_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }

  i = self->length%8;
  for (; i > 0; i--) {
    tmp_self[i-1] += constant;
  }
}
