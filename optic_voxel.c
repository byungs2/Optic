#include "include/optic_voxel.h"

G_DEFINE_TYPE (OpticVoxel, optic_voxel, OPTIC_TYPE_COLLISION_OBJECT)

static void
optic_voxel_dispose (GObject *object)
{
  G_OBJECT_GET_CLASS (optic_voxel_parent_class)->dispose (object);
}

static void
optic_voxel_finalize (GObject *object)
{
  G_OBJECT_GET_CLASS (optic_voxel_parent_class)->finalize (object);
}

static void
optic_voxel_class_init (OpticVoxelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->dispose = optic_voxel_dispose;
  object_class->finalize = optic_voxel_finalize;
}

static void
optic_voxel_init (OpticVoxel *self)
{

}

gboolean 
optic_voxel_is_collision (GObject *self, 
    GObject *other, gfloat threshold)
{
  gboolean res = 1;
  OpticCollisionObjectClass *klass = OPTIC_COLLISION_OBJECT_GET_CLASS (self);
  res = klass->is_collision (OPTIC_COLLISION_OBJECT (self), 
      OPTIC_COLLISION_OBJECT (other), threshold);
  return res;
}
