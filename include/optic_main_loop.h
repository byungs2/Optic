#ifndef __OPTIC_MAIN_LOOP_H__
#define __OPTIC_MAIN_LOOP_H__

#include "common.h"
#include "optic_object.h"

G_BEGIN_DECLS

#define OPTIC_TYPE_MAIN_LOOP (optic_main_loop_get_type ())
#define OPTIC_MAIN_LOOP(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),OPTIC_TYPE_MAIN_LOOP, OpticMainLoop))
#define OPTIC_MAIN_LOOP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), OPTIC_TYPE_MAIN_LOOP, OpticMainLoopClass))
#define OPTIC_IS_MAIN_LOOP(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),OPTIC_TYPE_MAIN_LOOP))
#define OPTIC_IS_MAIN_LOOP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), OPTIC_TYPE_MAIN_LOOP))

typedef struct _OpticMainLoopClass OpticMainLoopClass;
typedef struct _OpticMainLoop OpticMainLoop;

struct _OpticMainLoopClass {
  OpticObjectClass parent_class;
};

struct _OpticMainLoop {
  OpticObject parent_instance;
  GMainLoop *g_main_loop;
};

GType optic_main_loop_get_type (void);

G_END_DECLS
#endif
