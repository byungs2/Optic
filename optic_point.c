#include <math.h>
#include <malloc.h>
#include <immintrin.h>
#include "include/optic_point.h"

G_DEFINE_TYPE (OpticPoint, optic_point, OPTIC_TYPE_OBJECT)

enum {
  PROP_0 = 0,
  PROP_D_TYPE,
  PROP_DIM_NUM,
  PROP_DIM_POINT_VALS,
};

/* TODO need to free mallocs in here */
static void
optic_point_dispose (GObject *object)
{
  G_OBJECT_CLASS (optic_point_parent_class)->dispose (object);
}

static void
optic_point_finalize (GObject *object)
{
  G_OBJECT_CLASS (optic_point_parent_class)->finalize (object);
}

static void
optic_point_set_property (GObject *object, 
    guint property_id, 
    const GValue *value, 
    GParamSpec *psepc)
{
  gint i;
  gpointer seq;
  OpticPoint *self = OPTIC_POINT (object);
  switch (property_id) {
    case PROP_DIM_NUM:
      self->dim_size = g_value_get_int (value);
      g_free (self->dim);
      self->dim = (gfloat *)g_malloc0 (self->dim_size * sizeof(gfloat));
      break;
    case PROP_DIM_POINT_VALS:
      seq = g_value_get_pointer (value);
      for (i = 0; i < self->dim_size; i++) {
        self->dim[i] = ((gfloat *)seq)[i];
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
optic_point_get_property (GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec)
{
  OpticPoint *self = OPTIC_POINT (object);
  switch (property_id) {
    case PROP_DIM_NUM:
      g_value_set_int (value, self->dim_size);
      break;
    case PROP_DIM_POINT_VALS:
      g_value_set_pointer (value, self->dim);
      break;
    case PROP_D_TYPE:
      g_value_set_enum (value, self->dtype);
      break;
    default:
      break;
  }
}

static void
optic_point_class_init (OpticPointClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = optic_point_set_property;
  gobject_class->get_property = optic_point_get_property;

  g_object_class_install_property (gobject_class, PROP_DIM_NUM,
      g_param_spec_int ("dim", "Point dimentsion number",
        "Point dimension number", 0, G_MAXINT, 3, G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_DIM_POINT_VALS,
      g_param_spec_pointer ("dim-vals", "Value of point",
        "Value of point", G_PARAM_READWRITE));
  g_object_class_install_property (gobject_class, PROP_D_TYPE,
      g_param_spec_int ("dtype", "Type of data",
        "Type of data", 0, G_MAXINT, 0, G_PARAM_READWRITE));

  gobject_class->finalize = optic_point_finalize;
  gobject_class->dispose = optic_point_dispose;
}

static void
optic_point_init (OpticPoint *instance)
{
  instance->dim_size = 3;
  instance->dtype = G_TYPE_FLOAT;
  instance->dim = g_malloc0 (sizeof(gfloat) * instance->dim_size);
}

gfloat 
optic_point_distance (OpticPoint *self, OpticPoint *other)
{
  gint i, j, iter = { 0, };
  gfloat res = 0;
  gfloat *tmp_self, *tmp_other = { NULL, };
  __m256 self_point, other_point, dest, distance;

  g_assert (self->dim_size == other->dim_size 
      && self->dim != NULL && other->dim != NULL 
      );

  iter = (self->dim_size + 7)/8;

  tmp_self = (gfloat *)self->dim;
  tmp_other = (gfloat *)other->dim;
  distance = _mm256_set1_ps (0.0);

  for (i = 0; i < iter; i++) {
    self_point = _mm256_loadu_ps (tmp_self);
    other_point = _mm256_loadu_ps (tmp_other);
    dest = _mm256_sub_ps (self_point, other_point);
    dest = _mm256_mul_ps (dest, dest);
    distance = _mm256_add_ps (dest, distance);

    tmp_self += 8;
    tmp_other += 8;
  }
  for (j = 0; j < 8; j++) {
    res += ((gfloat *)&distance)[j];
  }
  
  return sqrt(res);
}

void 
optic_point_mul (OpticPoint *self, gfloat constant)
{
  gint i;
  for (i = 0; i < self->dim_size; i++) {
    self->dim[i] *= constant;
  }
}

void 
optic_point_div (OpticPoint *self, gfloat constant)
{
  gint i;
  g_assert (constant != 0);
  for (i = 0; i < self->dim_size; i++) {
    self->dim[i] /= constant;
  }
}

void 
optic_point_add (OpticPoint *self, gfloat constant)
{
  gint i;
  for (i = 0; i < self->dim_size; i++) {
    self->dim[i] += constant;
  }
}
