#include <math.h>
#include <malloc.h>
#include <immintrin.h>
#include "include/optic_tensor.h"

G_DEFINE_TYPE (OpticTensor, optic_tensor, OPTIC_TYPE_OBJECT)

enum {
  PROP_0 = 0,
  PROP_D_TYPE,
  PROP_DIM_NUM,
  PROP_DIM_TENSOR_VALS,
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
    GParamSpec *psepc)
{
  gint i;
  gpointer seq;
  OpticTensor *self = OPTIC_TENSOR (object);
  switch (property_id) {
    case PROP_DIM_NUM:
      self->dim_size = g_value_get_int (value);
      g_free (self->tensor);
      self->tensor = (gfloat *)g_malloc0 (self->dim_size * sizeof(gfloat));
      break;
    case PROP_DIM_TENSOR_VALS:
      seq = g_value_get_pointer (value);
      for (i = 0; i < self->dim_size; i++) {
        self->tensor[i] = ((gfloat *)seq)[i];
      }
      break;
    case PROP_D_TYPE:
      self->dtype = g_value_get_enum (value);
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
    case PROP_DIM_NUM:
      g_value_set_int (value, self->dim_size);
      break;
    case PROP_DIM_TENSOR_VALS:
      g_value_set_pointer (value, self->tensor);
      break;
    case PROP_D_TYPE:
      g_value_set_enum (value, self->dtype);
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

  g_object_class_install_property (gobject_class, PROP_DIM_NUM,
      g_param_spec_int ("dim", "Tensor dimentsion number",
        "Tensor dimension number", 0, G_MAXINT, 3, G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_DIM_TENSOR_VALS,
      g_param_spec_pointer ("dim-vals", "Value of tensor",
        "Value of tensor", G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_D_TYPE,
      g_param_spec_int ("dtype", "Type of data",
        "Type of data", 0, G_MAXINT, 0, G_PARAM_READWRITE));

  gobject_class->finalize = optic_tensor_finalize;
  gobject_class->dispose = optic_tensor_dispose;
}

static void
optic_tensor_init (OpticTensor *instance)
{
  instance->dim_size = 3;
  instance->dtype = G_TYPE_FLOAT;
  instance->tensor = (gfloat *) g_malloc0 (sizeof(gfloat) * instance->dim_size);
}

gfloat 
optic_tensor_distance (OpticTensor *self, OpticTensor *other)
{
  gint i, iter = { 0, };
  gfloat res = 0;
  gfloat *tmp_self, *tmp_other = { NULL, };
  __m256 self_tensor, other_tensor, dest, distance;

  g_assert (self->dim_size == other->dim_size 
      && self->tensor != NULL && other->tensor != NULL);

  iter = (self->dim_size + 7)/8;

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
  for (i = 0; i < 8; i++) {
    res += ((gfloat *)&distance)[i];
  }
  
  return sqrt(res);
}

void
optic_tensor_dot (OpticTensor *self, OpticTensor *other)
{

}

void
optic_tensor_cross (OpticTensor *self, OpticTensor *other)
{

}

void
optic_tensor_outer (OpticTensor *self, OpticTensor *other)
{

}

void 
optic_tensor_mul (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;

  g_assert (self->tensor != NULL);

  i = (self->dim_size + 7)/8;

  tmp_self = (gfloat *)self->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_mul_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }
}

void 
optic_tensor_div (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;

  g_assert (constant != 0);

  i = (self->dim_size + 7)/8;

  tmp_self = (gfloat *)self->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_div_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }
}

void 
optic_tensor_add (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;

  g_assert (constant != 0);

  i = (self->dim_size + 7)/8;

  tmp_self = (gfloat *)self->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_add_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }
}
