#ifndef __OPTIC_CLOCK_H__
#define __OPTIC_CLOCK_H__

#include "common.h"
#include "optic_object.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_CLOCK (optic_clock_get_type ())
#define OPTIC_CLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_CLOCK, OpticClock))
#define OPTIC_CLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_CLOCK, OpticClockClass))
#define OPTIC_IS_CLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_CLOCK))
#define OPTIC_IS_CLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_CLOCK))

typedef struct _OpticClockClass OpticClockClass;
typedef struct _OpticClock OpticClock;

struct _OpticClockClass {
  OpticObjectClass parent_class;
};

struct _OpticClock {
  OpticObject parent_instance;
};

GType optic_clock_get_type (void);

G_END_DECLS
#endif
