/*
 * controller-libs
 * Copyright (c) 2021 Wahtari GmbH
 *
 * All source code in this file is subject to the included LICENSE file.
 */

/// \file
/// \brief Contains the complete C API.
#ifndef NLAB_CTRL_LIB_H
#define NLAB_CTRL_LIB_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Same as const char, needed to use const char with cgo.
typedef const char const_char;

//#############//
//### Error ###//
//#############//

/// \brief Status code of an nlab_ctrl_error.
typedef enum {
    NLAB_CTRL_OK, ///< Operation was successful.
    NLAB_CTRL_ERR, ///< Some unknown error occurred.
    NLAB_CTRL_ERR_NOT_FOUND ///< Some resource could not be found.
} nlab_ctrl_error_code;

/// \brief Contains the result of an operation.
///
/// Every API function that has the potential to fail takes this error as argument. \n
/// A call was only successful, if the code is set to ::NLAB_CTRL_OK.
///
/// Errors should be created with nlab_ctrl_error_new() and eventually freed with nlab_ctrl_error_free(). \n
/// See also: nlab_ctrl_error_set(), nlab_ctrl_error_clear().
typedef struct {
    /// \brief Indicates the result of an operation.
    nlab_ctrl_error_code code;

    /// \brief Human-readable description of the error, if code is not ::NLAB_CTRL_OK.
    char* msg;
} nlab_ctrl_error;

/// \brief Creates a new error.
///
/// Creates a new error that is ready to use. \n
/// Must be eventually freed with nlab_ctrl_error_free().
nlab_ctrl_error* nlab_ctrl_error_new();

/// \brief Set error properties.
void nlab_ctrl_error_set(nlab_ctrl_error* err, nlab_ctrl_error_code code, char* msg);

/// \brief Prints the error.
///
/// Prints a formatted string to stdout with a trailing newline.
void nlab_ctrl_error_print(nlab_ctrl_error* err);

/// \brief Clears the error.
///
/// Sets the code to ::NLAB_CTRL_OK and frees msg, if set.
void nlab_ctrl_error_clear(nlab_ctrl_error* err);

/// \brief Frees the error.
///
/// Must be called eventually for every error created with nlab_ctrl_error_new(). \n
/// Do not use err further afterwards.
void nlab_ctrl_error_free(nlab_ctrl_error* err);

//############//
//### Ctrl ###//
//############//

/// \brief Contains information about a controller.
///
/// Contains all information about a controller necessary to identify and open it.
typedef struct {
    /// \brief The id of the backend compatible with this controller.
    char* backend_id;

    /// \brief The unique id of the controller itself.
    char* id;

    /// \brief The device path on the host system of the controller.
    char* dev_path;
} nlab_ctrl_info;

/// \brief A list of nlab_ctrl_info.
///
/// Defines a list of nlab_ctrl_info structs. \n
/// Instead of manually interacting via C pointer arithmetic with it, use the provided convenience functions: \n
/// nlab_ctrl_info_list_size(), nlab_ctrl_info_list_at_index(), nlab_ctrl_info_list_free().
typedef nlab_ctrl_info** nlab_ctrl_info_list;

/// \brief Returns the number of elements in the list.
int nlab_ctrl_info_list_size(nlab_ctrl_info_list infl);

/// \brief Retrieves element at the given index.
///
/// Safely accesses the nlab_ctrl_info at the specified index of the given nlab_ctrl_info_list.
///
/// \return A pointer to the nlab_ctrl_info, or NULL, if the index was out of bounds.
nlab_ctrl_info* nlab_ctrl_info_list_at_index(nlab_ctrl_info_list infl, int index);

/// \brief Frees the list.
///
/// Calls nlab_ctrl_info_free() on every element of the list, and in the end frees the list itself.
void nlab_ctrl_info_list_free(nlab_ctrl_info_list infl);

/// \brief Frees the nlab_ctrl_info.
void nlab_ctrl_info_free(nlab_ctrl_info* inf);

/// \brief Options for a controller.
///
/// Contains options that modify the behaviour of a controller. \n
/// For every member that is not set a sensible default value is used. This means that an empty struct represents default options.
typedef struct {
    /// \brief Directory to store controller state.
    ///
    /// The controller saves changes to some of its values to persistent storage. \n
    /// This allows a controller to load its previous state when opened again.
    const char* state_dir;
} nlab_ctrl_opts;

/// \brief The main controller type.
///
/// The main type representing a successfully opened controller. \n
/// Check out nlab_ctrl_open() for more info.
typedef struct {
    void* go_ptr;
} nlab_ctrl;

//#################//
//### StepMotor ###//
//#################//

/// \brief Represents a single step motor of the controller.
typedef struct {
    /// \brief A unique identifier.
    char* id;

    /// \brief A human-readable identifier.
    char* name;

    /// \brief The current step position of the step motor.
    int step;

    /// \brief The minimum step position of the step motor.
    int min_step;

    /// \brief The maximum step position of the step motor.
    int max_step;
} nlab_ctrl_step_motor;

/// \brief A list of nlab_ctrl_step_motor.
///
/// Defines a list of nlab_ctrl_step_motor structs. \n
/// Instead of manually interacting via C pointer arithmetic with it, use the provided convenience functions: \n
/// nlab_ctrl_step_motors_size(), nlab_ctrl_step_motors_at_index(), nlab_ctrl_step_motors_free().
typedef nlab_ctrl_step_motor** nlab_ctrl_step_motors;

/// \brief Returns the number of elements in the list.
int nlab_ctrl_step_motors_size(nlab_ctrl_step_motors sms);

/// \brief Retrieves element at the given index.
///
/// Safely accesses the nlab_ctrl_step_motor at the specified index of the given nlab_ctrl_step_motors.
///
/// \return A pointer to the nlab_ctrl_step_motor, or NULL, if the index was out of bounds.
nlab_ctrl_step_motor* nlab_ctrl_step_motors_at_index(nlab_ctrl_step_motors sms, int index);

/// \brief Frees the list.
///
/// Calls nlab_ctrl_step_motor_free() on every element of the list, and in the end frees the list itself.
void nlab_ctrl_step_motors_free(nlab_ctrl_step_motors sms);

/// \brief Frees the nlab_ctrl_step_motor.
void nlab_ctrl_step_motor_free(nlab_ctrl_step_motor* sm);

//###########//
//### LED ###//
//###########//

/// \brief The state of the status led.
typedef enum {
    NLAB_CTRL_STATUS_LED_OFF, ///< The status led is switched off.
    NLAB_CTRL_STATUS_LED_ON,  ///< The status led is switched on.
    NLAB_CTRL_STATUS_LED_BLINKING  ///< The status led is switched on and blinks.
} nlab_ctrl_status_led_state;

/// \brief Represents a single led of the controller.
typedef struct {
    /// \brief A unique identifier.
    char* id;

    /// \brief A human-readable identifier.
    char* name;

    /// \brief The current state of the led.
    bool  on;

    /// \brief The current brightness of the led.
    int   brightness;

    /// \brief A flag whether strobe is active.
    bool  strobe_on;

    /// \brief The current delay of the strobe in milliseconds.
    int   strobe_delay;
} nlab_ctrl_led;

/// \brief A list of nlab_ctrl_led.
///
/// Defines a list of nlab_ctrl_led structs. \n
/// Instead of manually interacting via C pointer arithmetic with it, use the provided convenience functions: \n
/// nlab_ctrl_leds_size(), nlab_ctrl_leds_at_index(), nlab_ctrl_leds_free().
typedef nlab_ctrl_led** nlab_ctrl_leds;

/// \brief Returns the number of elements in the list.
int nlab_ctrl_leds_size(nlab_ctrl_leds leds);

/// \brief Retrieves element at the given index.
///
/// Safely accesses the nlab_ctrl_led at the specified index of the given nlab_ctrl_leds.
///
/// \return A pointer to the nlab_ctrl_led, or NULL, if the index was out of bounds.
nlab_ctrl_led* nlab_ctrl_leds_at_index(nlab_ctrl_leds leds, int index);

/// \brief Frees the list.
///
/// Calls nlab_ctrl_led_free() on every element of the list, and in the end frees the list itself.
void nlab_ctrl_leds_free(nlab_ctrl_leds leds);

/// \brief Frees the nlab_ctrl_led.
void nlab_ctrl_led_free(nlab_ctrl_led* led);

//##############//
//### Switch ###//
//##############//

/// \brief Represents a single switch of the controller.
typedef struct {
    /// \brief A unique identifier.
    char* id;

    /// \brief A human-readable identifier.
    char* name;

    /// \brief The current state of the switch.
    bool  on;
} nlab_ctrl_switch;

/// \brief A list of nlab_ctrl_switch.
///
/// Defines a list of nlab_ctrl_switch structs. \n
/// Instead of manually interacting via C pointer arithmetic with it, use the provided convenience functions: \n
/// nlab_ctrl_switches_size(), nlab_ctrl_switches_at_index(), nlab_ctrl_switches_free().
typedef nlab_ctrl_switch** nlab_ctrl_switches;

/// \brief Returns the number of elements in the list.
int nlab_ctrl_switches_size(nlab_ctrl_switches sws);

/// \brief Retrieves element at the given index.
///
/// Safely accesses the nlab_ctrl_switch at the specified index of the given nlab_ctrl_switches.
///
/// \return A pointer to the nlab_ctrl_switch, or NULL, if the index was out of bounds.
nlab_ctrl_switch* nlab_ctrl_switches_at_index(nlab_ctrl_switches sws, int index);

/// \brief Frees the list.
///
/// Calls nlab_ctrl_switch_free() on every element of the list, and in the end frees the list itself.
void nlab_ctrl_switches_free(nlab_ctrl_switches sws);

/// \brief Frees the nlab_ctrl_switch.
void nlab_ctrl_switch_free(nlab_ctrl_switch* sw);

//################//
//### GPIO Pin ###//
//################//

/// \brief The direction of a gpio pin.
typedef enum {
    NLAB_CTRL_GPIO_PIN_DIRECTION_IN, ///< Input pin (read-only).
    NLAB_CTRL_GPIO_PIN_DIRECTION_OUT, ///< Output pin (writable).
    NLAB_CTRL_GPIO_PIN_DIRECTION_IO ///< Input/Output pin (read-write).
} nlab_ctrl_gpio_pin_direction;

/// \brief Represents a single gpio pin of the controller.
typedef struct {
    /// \brief A unique identifier.
    char* id;

    /// \brief A human-readable identifier.
    char* name;

    /// \brief The direction of the gpio pin.
    nlab_ctrl_gpio_pin_direction direction;

    // The current state of the gpio pin.
    bool on;
} nlab_ctrl_gpio_pin;

/// \brief A list of nlab_ctrl_gpio_pin.
///
/// Defines a list of nlab_ctrl_gpio_pin structs. \n
/// Instead of manually interacting via C pointer arithmetic with it, use the provided convenience functions: \n
/// nlab_ctrl_gpio_pins_size(), nlab_ctrl_gpio_pins_at_index(), nlab_ctrl_gpio_pins_free().
typedef nlab_ctrl_gpio_pin** nlab_ctrl_gpio_pins;

/// \brief Returns the number of elements in the list.
int nlab_ctrl_gpio_pins_size(nlab_ctrl_gpio_pins gps);

/// \brief Retrieves element at the given index.
///
/// Safely accesses the nlab_ctrl_gpio_pin at the specified index of the given nlab_ctrl_gpio_pins.
///
/// \return A pointer to the nlab_ctrl_gpio_pin, or NULL, if the index was out of bounds.
nlab_ctrl_gpio_pin* nlab_ctrl_gpio_pins_at_index(nlab_ctrl_gpio_pins gps, int index);

/// \brief Frees the list.
///
/// Calls nlab_ctrl_gpio_pin_free() on every element of the list, and in the end frees the list itself.
void nlab_ctrl_gpio_pins_free(nlab_ctrl_gpio_pins gps);

/// \brief Frees the nlab_ctrl_gpio_pin.
void nlab_ctrl_gpio_pin_free(nlab_ctrl_gpio_pin* gp);

#ifdef __cplusplus
}
#endif

#endif
/* Code generated by cmd/cgo; DO NOT EDIT. */

/* package nlab/controller-libs */


#line 1 "cgo-builtin-export-prolog"

#include <stddef.h> /* for ptrdiff_t below */

#ifndef GO_CGO_EXPORT_PROLOGUE_H
#define GO_CGO_EXPORT_PROLOGUE_H

#ifndef GO_CGO_GOSTRING_TYPEDEF
typedef struct { const char *p; ptrdiff_t n; } _GoString_;
#endif

#endif

/* Start of preamble from import "C" comments.  */


#line 8 "lib.go"

#line 1 "cgo-generated-wrapper"


/* End of preamble from import "C" comments.  */


/* Start of boilerplate cgo prologue.  */
#line 1 "cgo-gcc-export-header-prolog"

#ifndef GO_CGO_PROLOGUE_H
#define GO_CGO_PROLOGUE_H

typedef signed char GoInt8;
typedef unsigned char GoUint8;
typedef short GoInt16;
typedef unsigned short GoUint16;
typedef int GoInt32;
typedef unsigned int GoUint32;
typedef long long GoInt64;
typedef unsigned long long GoUint64;
typedef GoInt64 GoInt;
typedef GoUint64 GoUint;
typedef __SIZE_TYPE__ GoUintptr;
typedef float GoFloat32;
typedef double GoFloat64;
typedef float _Complex GoComplex64;
typedef double _Complex GoComplex128;

/*
  static assertion to make sure the file is being used on architecture
  at least with matching size of GoInt.
*/
typedef char _check_for_64_bit_pointer_matching_GoInt[sizeof(void*)==64/8 ? 1:-1];

#ifndef GO_CGO_GOSTRING_TYPEDEF
typedef _GoString_ GoString;
#endif
typedef void *GoMap;
typedef void *GoChan;
typedef struct { void *t; void *v; } GoInterface;
typedef struct { void *data; GoInt len; GoInt cap; } GoSlice;

#endif

/* End of boilerplate cgo prologue.  */

#ifdef __cplusplus
extern "C" {
#endif


/// \brief Lists available controllers.
///
/// Returns a list of controllers that are found on the system.
/// These can then be used to open a controller with nlab_ctrl_open.
///
/// \return
extern nlab_ctrl_info_list nlab_ctrl_list(nlab_ctrl_error* ctrl_err);

/// \brief Opens a controller for interaction.
///
/// Opens a controller using the provided parameters and options. \n
/// If this operation succeeds, the returned controller can be used in all other API methods. \n
/// When the controller is no longer needed, nlab_ctrl_close() must be called.
///
/// \param[in]     backend_id   The id of the backend.
/// \param[in]     dev_path     The device path of the controller.
/// \param[in]     opts         Optional parameters of the controller.
/// \param[in,out] ctrl_err     Used to communicate the result of the operation.
///
/// \return A nlab_ctrl ready to use, or NULL, when nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl* nlab_ctrl_open(const_char* backend_id, const_char* dev_path, nlab_ctrl_opts opts, nlab_ctrl_error* ctrl_err);

/// \brief Retrieves all step motors of the controller.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A list of step motors, or NULL, then either no step motor is available or nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_step_motors nlab_ctrl_get_step_motors(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Retrieves a single step motor of the controller by its id.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the step motor.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A pointer to the step motor, or NULL, when nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_step_motor* nlab_ctrl_get_step_motor(nlab_ctrl* ctrl, const_char* id, nlab_ctrl_error* ctrl_err);

/// \brief Sets the relative position of a step motor.
///
/// Moves the step motor by the given step from its current position.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the step motor.
/// \param[in]     step       Number of steps the step motor should move, bound by nlab_ctrl_step_motor::min_step
///                           and nlab_ctrl_step_motor::max_step.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_step_motor_rel_pos(nlab_ctrl* ctrl, const_char* id, int step, nlab_ctrl_error* ctrl_err);

/// \brief Sets the absolute position of a step motor.
///
/// Moves the step motor to the given step position. \n
/// Notice: This is currently not supported by most controllers.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the step motor.
/// \param[in]     step       position the step motor should move to, bound by nlab_ctrl_step_motor::min_step
///                           and nlab_ctrl_step_motor::max_step.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_step_motor_abs_pos(nlab_ctrl* ctrl, const_char* id, int step, nlab_ctrl_error* ctrl_err);

/// \brief Sets the state of the status led.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     state      The new state of the status led.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_status_led(nlab_ctrl* ctrl, nlab_ctrl_status_led_state state, nlab_ctrl_error* ctrl_err);

/// \brief Sets the interval at which the status led blinks.
///
/// This has only an effect, if the status led's state is ::NLAB_CTRL_STATUS_LED_BLINKING.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     duration   The duration in nanoseconds at which the status led should blink.
///                           Valid values are between 50ms and 5s (inclusive).
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_status_led_blinking_duration(nlab_ctrl* ctrl, long long int duration, nlab_ctrl_error* ctrl_err);

/// \brief Retrieves all leds of the controller.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A list of leds, or NULL, then either no led is available or nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_leds nlab_ctrl_get_leds(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Retrieves a single led of the controller by its id.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the led.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A pointer to the led, or NULL, when nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_led* nlab_ctrl_get_led(nlab_ctrl* ctrl, const_char* id, nlab_ctrl_error* ctrl_err);

/// \brief Sets the state of the led.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the led.
/// \param[in]     on         The new state of the led.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_led(nlab_ctrl* ctrl, const_char* id, _Bool on, nlab_ctrl_error* ctrl_err);

/// \brief Sets the strobe state of the led.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the led.
/// \param[in]     on         The new strobe state of the led.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_led_strobe(nlab_ctrl* ctrl, const_char* id, _Bool on, nlab_ctrl_error* ctrl_err);

/// \brief Sets the brightness of the led.
///
/// \param[in]     ctrl        The opened controller.
/// \param[in]     id          The id of the led.
/// \param[in]     brightness  The brightness the led should be set to.
///                            Valid values are between 0-100 (inclusive).
/// \param[in,out] ctrl_err    Used to communicate the result of the operation.
extern void nlab_ctrl_set_led_brightness(nlab_ctrl* ctrl, const_char* id, int brightness, nlab_ctrl_error* ctrl_err);

/// \brief Sets the interval at which the led blinks.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the led.
/// \param[in]     delay      The delay in nanoseconds at which the led should blink.
///                           Valid values are between 10ms and 1s (inclusive).
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_led_strobe_delay(nlab_ctrl* ctrl, const_char* id, int delay, nlab_ctrl_error* ctrl_err);

/// \brief Retrieves all switches of the controller.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A list of switches, or NULL, then either no switch is available or nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_switches nlab_ctrl_get_switches(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Retrieves a single switch of the controller by its id.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the switch.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A pointer to the switch, or NULL, when nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_switch* nlab_ctrl_get_switch(nlab_ctrl* ctrl, const_char* id, nlab_ctrl_error* ctrl_err);

/// \brief Sets the state of the switch.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the switch.
/// \param[in]     on         The new state of the switch.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_switch(nlab_ctrl* ctrl, const_char* id, _Bool on, nlab_ctrl_error* ctrl_err);

/// \brief Enables the gpio pins of the controller for usage.
///
/// When enabled, the nlab_ctrl_gpio_pin::on state of gpio pins is actually filled with the real value.
/// In addition, nlab_ctrl_set_gpio_pin can only be used when gpio pins are enabled.
/// While disabled, you can still query pins using the getter functions, but their on field is always false.
/// As soon as you no longer need gpio pins, you should call nlab_ctrl_disable_gpio_pins.
/// The operation fails, if the gpio pins are already enabled.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_enable_gpio_pins(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Disables the gpio pins of the controller for usage.
///
/// When disabled, the nlab_ctrl_gpio_pin::on state of gpio pins is always false..
/// In addition, nlab_ctrl_set_gpio_pin can not be used when gpio pins are disabled.
/// The operation fails, if the gpio pins are already disabled.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_disable_gpio_pins(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Returns whether gpio pins are currently enabled or disabled.
///
/// \param[in] ctrl  The opened controller.
///
/// \return True, if the gpio pins are enabled, else false.
extern _Bool nlab_ctrl_gpio_pins_enabled(nlab_ctrl* ctrl);

/// \brief Retrieves all gpio pins of the controller.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A list of gpio pins, or NULL, then either no gpio pin is available or nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_gpio_pins nlab_ctrl_get_gpio_pins(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Retrieves a single gpio pin of the controller by its id.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the gpio pin.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return A pointer to the gpio pin, or NULL, when nlab_ctrl_error::code is not ::NLAB_CTRL_OK.
extern nlab_ctrl_gpio_pin* nlab_ctrl_get_gpio_pin(nlab_ctrl* ctrl, const_char* id, nlab_ctrl_error* ctrl_err);

/// \brief Sets the state of the gpio pin.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in]     id         The id of the gpio pin.
/// \param[in]     on         The new state of the gpio pin.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
extern void nlab_ctrl_set_gpio_pin(nlab_ctrl* ctrl, const_char* id, _Bool on, nlab_ctrl_error* ctrl_err);

/// \brief Returns the temperature of the controller board's sensor in degree celcius.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation.
///
/// \return Temperature as float value in degree celcius.
extern float nlab_ctrl_temperature(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Triggers a power reset for the whole system.
///
/// This cuts off the power to the rest of the system and reboots it after 45 seconds.
/// Use this with care, as is does an unclean shutdown of the whole system.
/// Data might very well get lost or corrupted, as OS and applications have no time to react.
///
/// \param[in]     ctrl       The opened controller.
/// \param[in,out] ctrl_err   Used to communicate the result of the operation. If this operation succeeds,
///                           your program will not be around to actually this value, of course.
extern void nlab_ctrl_power_reset(nlab_ctrl* ctrl, nlab_ctrl_error* ctrl_err);

/// \brief Closes this controller and frees its resources.
///
/// The controller is no longer usable, after this function returns.
/// Should be called as soon as the controller is no longer needed.
///
/// \param[in] ctrl  The opened controller.
extern void nlab_ctrl_close(nlab_ctrl* ctrl);

#ifdef __cplusplus
}
#endif
