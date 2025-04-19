#ifndef ADC_H_
#define ADC_H_


#include "esp_adc/adc_oneshot.h"

#include <uaio.h>
#include <ush.h>


struct adc {
    adc_oneshot_unit_handle_t unit;
    adc_cali_handle_t cali;
    int atten;
    int chan;
};


struct adc *
adc_create(int unit, int atten, int chan);


int
adc_destroy(struct adc *adc);


int
adc_read(struct adc *adc);


ASYNC
adcA(struct uaio_task *self, struct ush_process *p);


#endif  // ADC_H_
