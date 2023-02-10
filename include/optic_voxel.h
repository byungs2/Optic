#ifndef __OPTIC_VOXEL_H__
#define __OPTIC_VOXEL_H__

#include "common.h"
#include "optic_collision_object.h"
#include "optic_tensor.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_VOXEL (optic_voxel_get_type ())
#define OPTIC_VOXEL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_VOXEL, OpticVoxel))
#define OPTIC_VOXEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_VOXEL, OpticVoxelClass))
#define OPTIC_IS_VOXEL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_VOXEL))
#define OPTIC_IS_VOXEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_VOXEL))

typedef struct _OpticVoxelClass OpticVoxelClass;
typedef struct _OpticVoxel OpticVoxel;

struct _OpticVoxelClass {
  OpticCollisionObjectClass parent_class;
};

struct _OpticVoxel {
  OpticCollisionObject parent_instance;
};

GType optic_voxel_get_type (void);

gboolean optic_voxel_is_collision (GObject *self, GObject *other, gfloat threshold);

G_END_DECLS
#endif
