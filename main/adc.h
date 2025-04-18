#ifndef ADC_H_
#define ADC_H_


#include "esp_adc/adc_oneshot.h"

#include <uaio.h>
#include <ush.h>


struct adc {
    adc_oneshot_unit_handle_t unit;
    adc_cali_handle_t cali;
};


ASYNC
adcA(struct uaio_task *self, struct ush_process *p);


#endif  // ADC_H_
