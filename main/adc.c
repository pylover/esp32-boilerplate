#include <uaio.h>
#include <ush.h>
#include <elog.h>

// #include "esp_adc/adc_cali.h"
// #include "esp_adc/adc_cali_scheme.h"

#include "adc.h"


// TODO: disable wifi
// TODO: configure adc1

// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/esp_err.html#c.ESP_OK
// #define ESP_ASSERT(c) if ((c != ESP_OK)) { UAIO_THROW(self) }


static int
_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten,
        adc_cali_handle_t *handleout) {
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    INFO("registering curve-fitting calibration scheme");
    adc_cali_curve_fitting_config_t calicfg = {
        .unit_id = unit,
        .chan = channel,
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_curve_fitting(&calicfg, &handle);

#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    INFO("registering line-fitting calibration scheme");
    adc_cali_line_fitting_config_t calicfg = {
        .unit_id = unit,
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_line_fitting(&calicfg, &handle);

#endif

    if (ret == ESP_OK) {
        INFO("calibration success");
        *handleout = handle;
        return 0;
    }

    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ERROR("eFuse not burnt, skip software calibration");
        return -1;
    }

    ERROR("invalid arg or no memory");
    return -1;
}


static void
_calibration_deinit(adc_cali_handle_t unit) {
    esp_err_t ret;
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    INFO("deregistering curve-fitting calibration scheme");
    ret = adc_cali_delete_scheme_curve_fitting(unit);

#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    INFO("deregistering line-fitting calibration scheme");
    ret = adc_cali_delete_scheme_line_fitting(unit);
#endif

    if (ret != ESP_OK) {
        ERROR("deregistering calibration scheme failed");
    }
}


struct adc *
adc_create(int unit, int atten, int chan) {
    struct adc *adc = malloc(sizeof(struct adc));
    if (adc == NULL) {
        return NULL;
    }

    /* create and allocate a unit */
    adc_oneshot_unit_init_cfg_t uicfg = {
        .unit_id = unit,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    if (adc_oneshot_new_unit(&uicfg, &adc->unit) != ESP_OK) {
        return NULL;
    }

    /* configure channel */
    adc_oneshot_chan_cfg_t chcfg = {
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if (adc_oneshot_config_channel(adc->unit, chan, &chcfg) != ESP_OK) {
        return NULL;
    }

    /* callibration */
    if (_calibration_init(unit, chan, atten, &adc->cali)) {
        return NULL;
    }

    adc->chan = chan;
    adc->atten = atten;
    return adc;
}


int
adc_destroy(struct adc *adc) {
    if (adc == NULL) {
        return -1;
    }

    _calibration_deinit(adc->cali);
    if (adc_oneshot_del_unit(adc->unit) != ESP_OK) {
        return -1;
    }

    return 0;
}


int
adc_read(struct adc *adc) {
    int raw;
    int v;

    if (adc_oneshot_read(adc->unit, adc->chan, &raw) != ESP_OK) {
        ERROR("adc oneshot read");
        return -1;
    }
    if (adc_cali_raw_to_voltage(adc->cali, raw, &v) != ESP_OK) {
        ERROR("adc cali raw to voltage");
        return -1;
    }

    return v;
}


ASYNC
adcA(struct uaio_task *self, struct ush_process *p) {
    struct adc *adc = (struct adc *)p->userptr;
    UAIO_BEGIN(self);

    /* create an adc instance */
    adc = adc_create(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_CHANNEL_1);
    if (adc == NULL) {
        UAIO_THROW(self);
    }
    p->userptr = adc;

    UAIO_SLEEP(self, 1000);
    printf("%d mV\n", adc_read(adc));

    UAIO_FINALLY(self);
    adc_destroy((struct adc*)p->userptr);
}
