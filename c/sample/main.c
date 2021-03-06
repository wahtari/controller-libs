/*
 * controller-libs
 * Copyright (c) 2021 Wahtari GmbH
 *
 * All source code in this file is subject to the included LICENSE file.
 */

#include <libnlab-ctrl.h>

void print_step_motor(nlab_ctrl_step_motor* sm) {
    printf("{id: %s, name: %s, step: %d, min_step: %d, max_step: %d}", sm->id, sm->name, sm->step, sm->min_step, sm->max_step);
}

void print_led(nlab_ctrl_led* led) {
    printf("{id: %s, name: %s, on: %d, brightness: %d, strobe_on: %d, strobe_delay: %d}", led->id, led->name, led->on, led->brightness, led->strobe_on, led->strobe_delay);
}

void print_switch(nlab_ctrl_switch* sw) {
    printf("{id: %s, name: %s, on: %d}", sw->id, sw->name, sw->on);
}

void print_gpio_pin(nlab_ctrl_gpio_pin* gp) {
    printf("{id: %s, name: %s, on: %d, direction: %d}", gp->id, gp->name, gp->on, gp->direction);
}

void show_and_modify_single_step_motor(nlab_ctrl* ctrl, char* id, nlab_ctrl_error* err) {
    nlab_ctrl_step_motor* sm = nlab_ctrl_get_step_motor(ctrl, id, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("\nfirst step motor: ");
    print_step_motor(sm);
    printf("\n");

    // Set its values.
    int v = sm->step + (sm->step < 5 ? 1 : -1);
    nlab_ctrl_set_step_motor_rel_pos(ctrl, sm->id, v, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("increased rel pos by %d\n\n", v);

end:
    if (sm != NULL) {
        nlab_ctrl_step_motor_free(sm);
    }
}

void show_and_modify_single_led(nlab_ctrl* ctrl, char* id, nlab_ctrl_error* err) {
    nlab_ctrl_led* led = nlab_ctrl_get_led(ctrl, id, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("\nfirst led: ");
    print_led(led);
    printf("\n");

    // Set its values.
    nlab_ctrl_set_led(ctrl, led->id, !led->on, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("set led to %d\n", !led->on);

    nlab_ctrl_set_led_strobe(ctrl, led->id, !led->strobe_on, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("set led strobe to %d\n", !led->strobe_on);

    int v = led->brightness + (led->brightness < 50 ? 5 : -5);
    nlab_ctrl_set_led_brightness(ctrl, led->id, v, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("set led brightness to %d\n", v);

    v = led->strobe_delay + (led->strobe_delay < 200 ? 20 : -20);
    nlab_ctrl_set_led_strobe_delay(ctrl, led->id, v, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("set led strobe delay to %d\n\n", v);

end:
    if (led != NULL) {
        nlab_ctrl_led_free(led);
    }
}

void show_and_modify_single_switch(nlab_ctrl* ctrl, char* id, nlab_ctrl_error* err) {
    nlab_ctrl_switch* sw = nlab_ctrl_get_switch(ctrl, id, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("\nfirst switch: ");
    print_switch(sw);
    printf("\n");

    // Set its values.
    nlab_ctrl_set_switch(ctrl, sw->id, !sw->on, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("set switch to %d\n\n", !sw->on);

end:
    if (sw != NULL) {
        nlab_ctrl_switch_free(sw);
    }
}

void show_and_modify_single_gpio_pin(nlab_ctrl* ctrl, char* id, nlab_ctrl_error* err) {
    nlab_ctrl_gpio_pin* gp = nlab_ctrl_get_gpio_pin(ctrl, id, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("\nfirst gpio pin: ");
    print_gpio_pin(gp);
    printf("\n");

    // Set its values.
    nlab_ctrl_set_gpio_pin(ctrl, gp->id, !gp->on, err);
    if (err->code != NLAB_CTRL_OK) {
        goto end;
    }
    printf("set gpio pin to %d\n\n", !gp->on);

end:
    if (gp != NULL) {
        nlab_ctrl_gpio_pin_free(gp);
    }
}

int main() {
    // Program return code.
    int ret = 0;

    // The following types are automatically freed, if they are defined, 
    // at the end of this function.
    nlab_ctrl_error* err = nlab_ctrl_error_new();
    nlab_ctrl_info_list infl;
    nlab_ctrl* ctrl;
    nlab_ctrl_step_motors sms;
    nlab_ctrl_leds leds;
    nlab_ctrl_switches sws;
    nlab_ctrl_gpio_pins gps;

    // Get a the list of available controllers.
    // Note that dummy controllers are not part included in that.
    infl = nlab_ctrl_list(err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }
    printf("found %d real controllers, but using dummy now\n", nlab_ctrl_info_list_size(infl));

    // Open the controller.
    nlab_ctrl_opts opts = { "/tmp/nlab-ctrl-state" };
    ctrl = nlab_ctrl_open("dummy", "dummy", opts, err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }
    printf("opened dummy controller, saving state in '%s'\n", opts.state_dir);

    // Activate the status LED, while sample is running.
    nlab_ctrl_set_status_led(ctrl, NLAB_CTRL_STATUS_LED_ON, err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }

    // Retrieve all its step motors.
    sms = nlab_ctrl_get_step_motors(ctrl, err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }

    // Print them.
    int len = nlab_ctrl_step_motors_size(sms);
    nlab_ctrl_step_motor* sm;
    printf("found %d step motor(s):\n", len);
    for (int i = 0; i < len; ++i) {
        sm = nlab_ctrl_step_motors_at_index(sms, i);
        printf(" - ");
        print_step_motor(sm);
        printf("\n");
    }

    // Get a single step motor.
    if (len > 0) {
        show_and_modify_single_step_motor(ctrl, nlab_ctrl_step_motors_at_index(sms, 0)->id, err);
        if (err->code != NLAB_CTRL_OK) {
            goto error;
        }    
    }

    // Retrieve all its leds.
    leds = nlab_ctrl_get_leds(ctrl, err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }

    // Print them.
    len = nlab_ctrl_leds_size(leds);
    nlab_ctrl_led* led;
    printf("found %d led(s):\n", len);
    for (int i = 0; i < len; ++i) {
        led = nlab_ctrl_leds_at_index(leds, i);
        printf(" - ");
        print_led(led);
        printf("\n");
    }

    // Get a single led.
    if (len > 0) {
        show_and_modify_single_led(ctrl, nlab_ctrl_leds_at_index(leds, 0)->id, err);
        if (err->code != NLAB_CTRL_OK) {
            goto error;
        }    
    }

    // Retrieve all its switches.
    sws = nlab_ctrl_get_switches(ctrl, err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }

    // Print them.
    len = nlab_ctrl_switches_size(sws);
    nlab_ctrl_switch* sw;
    printf("found %d switch(es):\n", len);
    for (int i = 0; i < len; ++i) {
        sw = nlab_ctrl_switches_at_index(sws, i);
        printf(" - ");
        print_switch(sw);
        printf("\n");
    }

    // Get a single switch.
    if (len > 0) {
        show_and_modify_single_switch(ctrl, nlab_ctrl_switches_at_index(sws, 0)->id, err);
        if (err->code != NLAB_CTRL_OK) {
            goto error;
        }    
    }

    // Retrieve all its gpio pins.
    gps = nlab_ctrl_get_gpio_pins(ctrl, err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }

    // Print them.
    len = nlab_ctrl_gpio_pins_size(gps);
    nlab_ctrl_gpio_pin* gp;
    printf("found %d gpio_pin(s):\n", len);
    for (int i = 0; i < len; ++i) {
        gp = nlab_ctrl_gpio_pins_at_index(gps, i);
        printf(" - ");
        print_gpio_pin(gp);
        printf("\n");
    }

    // Get a single gpio pin.
    if (len > 0) {
        show_and_modify_single_gpio_pin(ctrl, nlab_ctrl_gpio_pins_at_index(gps, 0)->id, err);
        if (err->code != NLAB_CTRL_OK) {
            goto error;
        }    
    }

    // Retrieve the temperature.
    float temp = nlab_ctrl_temperature(ctrl, err);
    if (err->code != NLAB_CTRL_OK) {
        goto error;
    }
    printf("temperature: %f??C\n", temp);

    // Show how to detect a not found error.
    nlab_ctrl_get_step_motor(ctrl, "doesnotexist", err);
    if (err->code == NLAB_CTRL_ERR_NOT_FOUND) {
        printf("correctly handled error for non-existing step motor\n");
    } else {
        goto error;
    }

    goto success;
    
error:
    nlab_ctrl_error_print(err);
    ret = err->code;
success:
    // Deactivate the status led.
    nlab_ctrl_set_status_led(ctrl, NLAB_CTRL_STATUS_LED_OFF, err);
    if (err->code != NLAB_CTRL_OK) {
        nlab_ctrl_error_print(err);
    }

    // Free all resources.
    nlab_ctrl_error_free(err);
    if (infl != NULL) {
        nlab_ctrl_info_list_free(infl);
    }
    if (sms != NULL) {
        nlab_ctrl_step_motors_free(sms);
    }
    if (leds != NULL) {
        nlab_ctrl_leds_free(leds);
    }
    if (sws != NULL) {
        nlab_ctrl_switches_free(sws);
    }
    if (gps != NULL) {
        nlab_ctrl_gpio_pins_free(gps);
    }
    if (ctrl != NULL) {
        nlab_ctrl_close(ctrl);
    }
    return ret;
}

