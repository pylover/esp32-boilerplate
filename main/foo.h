#ifndef FOO_H_
#define FOO_H_


#include <uaio.h>


typedef struct foo {
    int interval;
} foo_t;


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY foo
#include "uaio_generic.h"


ASYNC
fooA(struct uaio_task *self, struct foo *f);


#endif  // FOO_H_
