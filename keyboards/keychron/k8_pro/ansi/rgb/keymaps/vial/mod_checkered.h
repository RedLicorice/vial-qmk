
#define RGB_MATRIX_EFFECT_MOD_CHECKERED
RGB_MATRIX_EFFECT(MOD_CHECKERED)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool MOD_CHECKERED(effect_params_t* params) {
    HSV hsv1  = rgb_matrix_config.hsv;
    RGB rgb1 = rgb_matrix_hsv_to_rgb(hsv1);
    HSV hsv2 = hsv1;
    hsv2.h += scale8(hsv2.h, rgb_matrix_config.speed);
    RGB rgb2 = rgb_matrix_hsv_to_rgb(hsv2);
    

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        if(i%2 == 0){
            rgb_matrix_set_color(i, rgb1.r, rgb1.g, rgb1.b);
        } else {
            rgb_matrix_set_color(i, rgb2.r, rgb2.g, rgb2.b);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS