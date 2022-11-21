#ifdef ENABLE_RGB_MATRIX_ALPHAS_MODS

/*
LED_MATRIX_EFFECT(ALPHAS_MODS)
#    ifdef LED_MATRIX_CUSTOM_EFFECT_IMPLS

// alphas = val1, mods = val2
bool ALPHAS_MODS(effect_params_t* params) {
    LED_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t val1 = led_matrix_eeconfig.val;
    uint8_t val2 = val1 + led_matrix_eeconfig.speed;

    for (uint8_t i = led_min; i < led_max; i++) {
        LED_MATRIX_TEST_LED_FLAGS();
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
            led_matrix_set_value(i, val2);
        } else {
            led_matrix_set_value(i, val1);
        }
    }
    return led_matrix_check_finished_leds(led_max);
}

#    endif // LED_MATRIX_CUSTOM_EFFECT_IMPLS
*/
/* 
    Sorry, this is not alphas mods.. copy of solid reactive nexus, with some mods :)
*/
#define RGB_MATRIX_EFFECT_ALPHAS_MODS
#define RGB_MATRIX_EFFECT_ALPHAS_INVS
RGB_MATRIX_EFFECT(ALPHAS_MODS)

#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// static HSV ALPHAS_MODS_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick, uint8_t time) {
//     uint16_t effect = tick - dist;
//     if (effect > 255) effect = 255;
//     if (dist > 42) effect = 255;
//     if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;

//     hsv.v = qadd8(hsv.v, 255 - effect);
//     hsv.h = time;
//     return hsv;
// }

// alphas = color1, mods = color2
bool ALPHAS_MODS(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    
    HSV hsv1  = rgb_matrix_config.hsv;
    RGB rgb1 = rgb_matrix_hsv_to_rgb(hsv1);
    // hsv.h += rgb_matrix_config.speed;
    // RGB rgb2 = rgb_matrix_hsv_to_rgb(hsv);

    uint8_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 2);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        if(i==0){
            rgb_matrix_set_color(i, rgb1.r, rgb1.g, rgb1.b);
            continue;
        }

        uint8_t start = qsub8(g_last_hit_tracker.count, 1);
        HSV hsv = rgb_matrix_config.hsv;
        hsv.v   = 0;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));

            uint16_t effect = tick - dist;
            if (effect > 255) effect = 255;
            if (dist > 73) effect = 255;
            if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;

            hsv.v = qadd8(hsv.v, 255 - effect);
            if(dist > 50 && dist < 73) {
                //hsv.v = hsv1.v;
                hsv.h = hsv1.h;
            }
            else {
                hsv.h = time;
            }
        }
        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_ALPHAS_MODS
