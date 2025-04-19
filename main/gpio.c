#include <stdio.h>
#include "driver/gpio.h"

#include <uaio.h>
#include <ush.h>
#include <elog.h>
#include <earg.h>

#include "gpio.h"


static enum earg_eatstatus
_eat(const struct earg_option *opt, const char *value,
        struct gpio_conf *cfg) {
    if (opt == NULL) {
        /* Positional */
        if (cfg->num == -1) {
            cfg->num = atoi(value);
        }
        else if (cfg->val == -1) {
            cfg->val = atoi(value);
        }
        else {
            return EARG_EAT_UNRECOGNIZED;
        }
        return EARG_EAT_OK;
    }

    return EARG_EAT_UNRECOGNIZED;
}


/* create and configure a earg structure */
static struct earg cli = {
    .eat = (earg_eater_t)_eat,
    .args = "num value",
};


ASYNC
gpioA(struct uaio_task *self, struct ush_process *p) {
    struct gpio_conf conf = {
        .num = -1,
        .val = -1,
    };

    UAIO_BEGIN(self);
    cli.userptr = &conf;
    enum earg_status status = earg_parse(&cli, p->argc,
            (const char **)p->argv, NULL);
    earg_dispose(&cli);

    if (status < EARG_OK) {
        UAIO_THROW(self);
    }
    if (status == EARG_OK_EXIT) {
        UAIO_RETURN(self);
    }

    gpio_set_direction(conf.num, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(conf.num, GPIO_FLOATING);
    gpio_set_level(conf.num, conf.val);
    UAIO_FINALLY(self);
}
