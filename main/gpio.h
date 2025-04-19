#ifndef GPIO_H_
#define GPIO_H_


#include <uaio.h>
#include <ush.h>


struct gpio_conf {
    int num;
    int val;
};


ASYNC
gpioA(struct uaio_task *self, struct ush_process *p);


#endif  // GPIO_H_
