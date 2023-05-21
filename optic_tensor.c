#include <math.h>
#include <immintrin.h>
#include "include/optic_tensor.h"

typedef struct _OpticTensorPrivate OpticTensorPrivate;

struct _OpticTensorPrivate {
  gfloat *tensor;
  guint64 *shape;
  guint64 length;
  guint dim_size;
  guint dtype;
};

static void optic_tensor_free (OpticTensor *self);

G_DEFINE_TYPE_WITH_PRIVATE (OpticTensor, optic_tensor, OPTIC_TYPE_OBJECT)

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
  OpticTensor *self = NULL;

  self = OPTIC_TENSOR (object);

  optic_tensor_free (self);
  G_OBJECT_CLASS (optic_tensor_parent_class)->finalize (object);
}

static void
optic_tensor_set_property (GObject *object, 
    guint property_id, 
    const GValue *value, 
    GParamSpec *pspec)
{
  switch (property_id) {
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
  OpticTensorPrivate *priv = optic_tensor_get_instance_private (self);
  switch (property_id) {
    case PROP_TENSOR_DIM:
      g_value_set_int (value, priv->dim_size);
      break;
    case PROP_TENSOR_DATA:
      g_value_set_pointer (value, priv->tensor);
      break;
    case PROP_TENSOR_SHAPE:
      g_value_set_pointer (value, priv->shape);
      break;
    case PROP_TENSOR_TYPE:
      g_value_set_int (value, priv->dtype);
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
  OpticTensorPrivate *priv = NULL;

  priv = optic_tensor_get_instance_private (instance);
  priv->dim_size = 0;
  priv->length = 0;
  priv->dtype = G_TYPE_FLOAT;
  priv->shape = NULL;
  priv->tensor = NULL;
}

/* TODO is it useful? */
gfloat 
optic_tensor_distance (OpticTensor *self, OpticTensor *other)
{
  gint i = 0, iter = 0, count = 0;
  gfloat res = 0.0, tmp = 0.0;
  gfloat *tmp_self = NULL, *tmp_other = NULL, *tmp_distance = NULL;
  __m256 self_tensor, other_tensor, dest, distance;

  OpticTensorPrivate *self_priv = NULL, *other_priv = NULL;

  self_priv = optic_tensor_get_instance_private (self);
  other_priv = optic_tensor_get_instance_private (other);

  /* TODO need to check shape too */
  g_assert (self_priv->dim_size == other_priv->dim_size 
      && self_priv->tensor != NULL && other_priv->tensor != NULL);

  iter = self_priv->length/8;

  tmp_self = (gfloat *)self_priv->tensor;
  tmp_other = (gfloat *)other_priv->tensor;
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

  i = self_priv->length%8;
  for (; i > 0; i--) {
    tmp = tmp_self[i-1] - tmp_other[i-1];
    tmp *= tmp;
    res += tmp;
  }
  
  return sqrt(res);
}

/* TODO */
OpticTensor *
optic_tensor_get_farthest_point_in_direction (OpticTensor *self, OpticTensor *direction)
{
  OpticTensor *res = NULL;

  return NULL;
}

OpticTensor *
optic_tensor_get_minkowski_difference (OpticTensor *self, OpticTensor *other, OpticTensor *direction)
{
  OpticTensor *p1 = NULL;
  OpticTensor *p2 = NULL;
  OpticTensor *p3 = NULL;

  p1 = optic_tensor_get_farthest_point_in_direction (self, direction);
  optic_tensor_negative (direction);
  p2 = optic_tensor_get_farthest_point_in_direction (self, direction);
  p3 = optic_tensor_subtract (p1, p2);

  return p3;
}

/* TODO */
gfloat
optic_tensor_dot (OpticTensor *self, OpticTensor *other)
{
  gint i = 0, iter = 0;
  gfloat res = 0.0;
  gfloat tmp = 0.0;
  gfloat *tmp_self = NULL, *tmp_other = NULL;
  __m256 self_tensor, other_tensor, dest, mul;

  OpticTensorPrivate *self_priv = NULL, *other_priv = NULL;

  self_priv = optic_tensor_get_instance_private (self);
  other_priv = optic_tensor_get_instance_private (other);

  /* TODO need to check shape too */
  g_assert (self_priv->dim_size == other_priv->dim_size 
      && self_priv->tensor != NULL && other_priv->tensor != NULL);

  iter = self_priv->length/8;

  tmp_self = (gfloat *)self_priv->tensor;
  tmp_other = (gfloat *)other_priv->tensor;
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

  i = self_priv->length%8;
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
optic_tensor_mul_scalar (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;
  OpticTensorPrivate *self_priv = NULL;

  self_priv = optic_tensor_get_instance_private (self);

  g_assert (self_priv->tensor != NULL);

  i = self_priv->length/8;

  tmp_self = (gfloat *)self_priv->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_mul_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }

  i = self_priv->length%8;
  for (; i > 0; i--) {
    tmp_self[i-1] *= constant;
  }
}

void 
optic_tensor_div_scalar (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;
  OpticTensorPrivate *self_priv = NULL;

  g_assert (constant != 0);

  self_priv = optic_tensor_get_instance_private (self);

  i = self_priv->length/8;

  tmp_self = (gfloat *)self_priv->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_div_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }

  i = self_priv->length%8;
  for (; i > 0; i--) {
    tmp_self[i-1] /= constant;
  }
}

void 
optic_tensor_add_scalar (OpticTensor *self, gfloat constant)
{
  gint i = 0;
  gfloat *tmp_self = NULL;
  __m256 self_tensor, dest, cnst;
  OpticTensorPrivate *self_priv = NULL;

  g_assert (constant != 0);

  self_priv = optic_tensor_get_instance_private (self);
  i = self_priv->length/8;

  tmp_self = (gfloat *)self_priv->tensor;
  cnst = _mm256_set1_ps (constant);

  for (; i > 0; i--) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    dest = _mm256_add_ps (self_tensor, cnst);
    _mm256_storeu_ps (tmp_self, dest);

    tmp_self += 8;
  }

  i = self_priv->length%8;
  for (; i > 0; i--) {
    tmp_self[i-1] += constant;
  }
}

/* TODO */
OpticTensor *
optic_tensor_subtract (OpticTensor *p1, OpticTensor *p2)
{
  OpticTensor *res = NULL;
  gint i = 0, iter = 0;
  gfloat tmp = 0.0;
  gfloat *tmp_self = NULL, *tmp_other = NULL;
  __m256 self_tensor, other_tensor, dest, mul;

  OpticTensorPrivate *p1_priv = NULL, *p2_priv = NULL;

  p1_priv = optic_tensor_get_instance_private (p1);
  p2_priv = optic_tensor_get_instance_private (p2);

  /* TODO need to check shape too */
  g_assert (p1_priv->dim_size == p2_priv->dim_size 
      && p1_priv->tensor != NULL && p2_priv->tensor != NULL);

  iter = p1_priv->length/8;

  tmp_self = (gfloat *)p1_priv->tensor;
  tmp_other = (gfloat *)p2_priv->tensor;

  for (i = 0; i < iter; i++) {
    self_tensor = _mm256_loadu_ps (tmp_self);
    other_tensor = _mm256_loadu_ps (tmp_other);
    dest = _mm256_sub_ps (self_tensor, other_tensor);

    tmp_self += 8;
    tmp_other += 8;
  }

  return res;
}

/* TODO */
void
optic_tensor_negative (OpticTensor *self)
{
  OpticTensor *res = NULL;
}

OpticTensor *
optic_tensor_new_with_template (gpointer data, OpticTensorTemplate *tmpl)
{
  OpticTensor *new_tensor = NULL;
  OpticTensorPrivate *priv = NULL;
  gchar **shape = NULL;
  gint i = 0, tmp = 1;

  new_tensor = g_object_new (OPTIC_TYPE_TENSOR, NULL);
  priv = optic_tensor_get_instance_private (new_tensor);

  shape = g_strsplit (tmpl->shape, ":", -1);
  priv->dim_size = g_strv_length (shape);
  priv->dtype = tmpl->dtype;

  priv->shape = (guint64 *)g_malloc0 (sizeof (guint64) * priv->dim_size);

  for (i = 0; i < priv->dim_size; i++) {
    priv->shape[i] = atoi (shape[i]);
    tmp *= priv->shape[i];
  }

  priv->length = tmp;

  if (data != NULL) {
    priv->tensor = g_memdup (data, sizeof (gfloat) * priv->length);
  } else {
    priv->tensor = (gfloat *)g_malloc0 (sizeof (gfloat) * priv->length);
  }

  g_strfreev (shape);
  return new_tensor;
}

void
optic_tensor_free (OpticTensor *self)
{
  OpticTensorPrivate *priv = NULL;

  priv = optic_tensor_get_instance_private (self);
  g_free (priv->shape);
  g_free (priv->tensor);
}
