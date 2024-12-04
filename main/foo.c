#include <uaio.h>
#include <elog.h>
#include "foo.h"


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY foo
#include "uaio_generic.c"


ASYNC
fooA(struct uaio_task *self, struct foo *f) {
    static int i;
    UAIO_BEGIN(self);

    for (i = 0;;i++) {
        INFO("Foo: #%d", i);
        UAIO_SLEEP(self, 1000000);
    }
    UAIO_FINALLY(self);
}
