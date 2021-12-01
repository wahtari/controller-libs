/*
 * controller-libs
 * Copyright (c) 2021 Wahtari GmbH
 *
 * All source code in this file is subject to the included LICENSE file.
 */

/// \file
/// \brief Contains the complete C++ API.
#ifndef NLAB_CTRL_LIB_HPP
#define NLAB_CTRL_LIB_HPP

#include <string>
#include <exception>
#include <memory>
#include <vector>

#include <libnlab-ctrl.h>

namespace nlab::ctrl {

//#################//
//### Exception ###//
//#################//

/// \brief An exception used to signal the failure of an operation.
///
/// Every method of the Controller that is not marked with the ***noexcept*** keyword
/// can throw this exception if an error occurs.
class Exception : public std::exception {
public:
    /// \brief Status code of an Exception.
    enum ErrCode {
        Generic  = 0, ///< Some unknown error occurred.
        NotFound = 1 ///< Some resource could not be found.
    };

    /// \brief The constructor of an Exception.
    ///
    /// \param[in]  code  The ErrCode of this Exception.
    /// \param[in]  msg   The human-readable message of this Exception.
    Exception(const ErrCode code, const std::string msg);

    /// \brief Returns the human-readable description of the Exception.
    const char* what() const throw ();
    /// \brief Returns the ErrCode of the Exception.
    ErrCode     code() const;

private:
    const ErrCode code_;
    const std::string msg_;
};

//#############//
//### Types ###//
//#############//

/// \brief Contains information about a controller.
///
/// Contains all information about a controller necessary to identify and open it.
struct Info {
    std::string backendID; ///< The id of the backend compatible with this controller.
    std::string id;        ///< The unique id of the controller itself.
    std::string devPath;   ///< The device path on the host system of the controller.
};

/// \brief The state of the status led.
enum StatusLEDState {
    OFF      = 0, ///< The status led is switched off.
    ON       = 1, ///< The status led is switched on.
    Blinking = 2  ///< The status led is switched on and blinks.
};

/// \brief Represents a single led of the controller.
struct StepMotor {
    std::string id;      ///< A unique identifier.
    std::string name;    ///< A human-readable identifier.
    int         step;    ///< The current step position of the step motor.
    int         minStep; ///< The minimum step position of the step motor.
    int         maxStep; ///< The maximum step position of the step motor.
};

/// \brief Represents a single led of the controller.
struct LED {
    std::string id;          ///< A unique identifier.
    std::string name;        ///< A human-readable identifier.
    bool        on;          ///< The current state of the led.
    int         brightness;  ///< The current brightness of the led.
    bool        strobeOn;    ///< A flag whether strobe is active.
    int         strobeDelay; ///< The current delay of the strobe in milliseconds.
};

/// \brief Represents a single switch of the controller.
struct Switch {
    std::string id;   ///< A unique identifier.
    std::string name; ///< A human-readable identifier.
    bool        on;   ///< The current state of the switch.
};

/// \brief The direction of a gpio pin.
enum GPIOPinDirection {
    IN  = 0, ///< Input pin (read-only).
    OUT = 1, ///< Output pin (writable).
    IO  = 2  ///< Input/Output pin (read-write).
};

/// \brief Represents a single gpio pin of the controller.
struct GPIOPin {
    std::string      id;        ///< A unique identifier.
    std::string      name;      ///< A human-readable identifier.
    GPIOPinDirection direction; ///< The direction of the gpio pin.
    bool             on;        ///< The current state of the gpio pin.
};

//##################//
//### Controller ###//
//##################//

/// \brief Options for a controller.
///
/// Contains options that modify the behaviour of a controller. \n
/// For every member that is not set a sensible default value is used. This means that an empty struct represents default options.
struct ControllerOpts {
    /// \brief Directory to store controller state.
    ///
    /// The controller saves changes to some of its values to persistent storage. \n
    /// This allows a controller to load its previous state when opened again.
    std::string stateDir;
};

/// \brief The main class representing one controller.
///
/// Offers static methods to query available controllers and open them.
/// All the methods from the C API have their C++ equivalent here.
class Controller {
public:
    /// \brief A type definition for a shared instance of a Controller.
    typedef std::shared_ptr<Controller> Ptr;

    /// \brief Lists available controllers.
    ///
    /// Returns a list of controllers that are found on the system.
    /// These can then be used to open a controller with Controller::open.
    ///
    /// \return A list of Info structs.
    /// \throws Exception 
    static std::vector<Info> list();

    /// \brief Static factory method that opens a controller for interaction.
    ///
    /// Opens a controller using the provided parameters and options. \n
    /// It is not strictly necessary to close the controller, as it gets
    /// automatically closed in its destructor.
    ///
    /// \param[in]  backendID  The id of the backend to use.
    /// \param[in]  devPath    The device path on the host.
    /// \param[in]  opts       Optional parameters of the controller.
    ///
    /// \return A Ptr to a Controller ready to use.
    /// \throws Exception 
    static Ptr open(const std::string& backendID, const std::string& devPath, const ControllerOpts& opts);

    /// \brief Retrieves all step motors of the controller.
    ///
    /// \return A list of StepMotors.
    /// \throws Exception 
    virtual std::vector<StepMotor> getStepMotors() = 0;

    /// \brief Retrieves a single step motor of the controller by its id.
    ///
    /// \param[in]  id  The id of the step motor.
    ///
    /// \return The StepMotor struct.
    /// \throws Exception 
    virtual StepMotor getStepMotor(const std::string& id) = 0;

    /// \brief Sets the relative position of a step motor.
    ///
    /// Moves the step motor by the given step from its current position.
    ///
    /// \param[in]  id    The id of the step motor.
    /// \param[in]  step  Number of steps the step motor should move, bound by StepMotor::minStep and StepMotor::maxStep.
    ///
    /// \throws Exception 
    virtual void setStepMotorRelPos(const std::string& id, int step) = 0;

    /// \brief Sets the absolute position of a step motor.
    ///
    /// Moves the step motor to the given step position. \n
    /// Notice: This is currently not supported by most controllers.
    ///
    /// \param[in]  id    The id of the step motor.
    /// \param[in]  step  position the step motor should move to, bound by StepMotor::minStep and StepMotor::maxStep.
    ///
    /// \throws Exception 
    virtual void setStepMotorAbsPos(const std::string& id, int step) = 0;

    /// \brief Sets the state of the status led.
    ///
    /// \param[in]  state  The new state of the status led.
    ///
    /// \throws Exception 
    virtual void setStatusLED(StatusLEDState state) = 0;

    /// \brief Sets the interval at which the status led blinks.
    ///
    /// This has only an effect, if the status led's state is StatusLEDState::Blinking.
    ///
    /// \param[in]  duration  The duration in nanoseconds at which the status led should blink.
    ///
    /// \throws Exception 
    virtual void setStatusLEDBlinkingDuration(long long int duration) = 0;

    /// \brief Retrieves all leds of the controller.
    ///
    /// \return A list of leds.
    /// \throws Exception 
    virtual std::vector<LED> getLEDs() = 0;

    /// \brief Retrieves a single led of the controller by its id.
    ///
    /// \param[in]  id  The id of the led.
    ///
    /// \return The LED struct.
    /// \throws Exception 
    virtual LED getLED(const std::string& id) = 0;

    /// \brief Sets the state of the led.
    ///
    /// \param[in]  id  The id of the led.
    /// \param[in]  on  The new state of the led.
    ///
    /// \throws Exception 
    virtual void setLED(const std::string& id, bool on) = 0;

    /// \brief Sets the strobe state of the led.
    ///
    /// \param[in]  id  The id of the led.
    /// \param[in]  on  The new strobe state of the led.
    ///
    /// \throws Exception 
    virtual void setLEDStrobe(const std::string& id, bool on) = 0;

    /// \brief Sets the brightness of the led.
    ///
    /// \param[in]  id          The id of the led.
    /// \param[in]  brightness  The brightness the led should be set to. Valid values are between 0-100 (inclusive).
    ///
    /// \throws Exception 
    virtual void setLEDBrightness(const std::string& id, int brightness) = 0;

    /// \brief Sets the interval at which the led blinks.
    ///
    /// \param[in]  id     The id of the led.
    /// \param[in]  delay  The delay in nanoseconds at which the led should blink. Valid values are between 10ms and 1s (inclusive).
    ///
    /// \throws Exception 
    virtual void setLEDStrobeDelay(const std::string& id, int delay) = 0;

    /// \brief Retrieves all switches of the controller.
    ///
    /// \return A list of switches.
    /// \throws Exception 
    virtual std::vector<Switch> getSwitches() = 0;

    /// \brief Retrieves a single switch of the controller by its id.
    ///
    /// \param[in]  id  The id of the switch.
    ///
    /// \return A switch struct.
    /// \throws Exception
    virtual Switch getSwitch(const std::string& id) = 0;

    /// \brief Sets the state of the switch.
    ///
    /// \param[in]  id  The id of the switch.
    /// \param[in]  on  The new state of the switch.
    ///
    /// \throws Exception
    virtual void setSwitch(const std::string& id, bool on) = 0;

    /// \brief Enables the gpio pins of the controller for usage.
    ///
    /// When enabled, the GPIOPin::on state of gpio pins is actually filled with the real value.
    /// In addition, setGPIOPin() can only be used when gpio pins are enabled.
    /// While disabled, you can still query pins using the getter functions, but their on field is always false.
    /// As soon as you no longer need gpio pins, you should call disableGPIOPins().
    /// The operation fails, if the gpio pins are already enabled.
    ///
    /// \throws Exception
    virtual void enableGPIOPins() = 0;

    /// \brief Disables the gpio pins of the controller for usage.
    ///
    /// When disabled, the GPIOPin::on state of gpio pins is always false..
    /// In addition, setGPIOPin() can not be used when gpio pins are disabled.
    /// The operation fails, if the gpio pins are already disabled.
    ///
    /// \throws Exception
    virtual void disableGPIOPins() = 0;

    /// \brief Returns whether gpio pins are currently enabled or disabled.
    ///
    /// \return True, if the gpio pins are enabled, else false.
    virtual bool gpioPinsEnabled() noexcept = 0;

    /// \brief Retrieves all gpio pins of the controller.
    ///
    /// \return A list of GPIOPin structs.
    /// \throws Exception
    virtual std::vector<GPIOPin> getGPIOPins() = 0;

    /// \brief Retrieves a single GPIOPin of the controller by its id.
    ///
    /// \param[in]  id  The id of the GPIOPin.
    ///
    /// \return A GPIOPin struct.
    /// \throws Exception
    virtual GPIOPin getGPIOPin(const std::string& id) = 0;

    /// \brief Sets the state of the GPIOPin.
    ///
    /// \param[in]  id  The id of the GPIOPin.
    /// \param[in]  on  The new state of the gpio pin.
    ///
    /// \throws Exception
    virtual void setGPIOPin(const std::string& id, bool on) = 0;

    /// \brief Returns the temperature of the controller board's sensor in degree celcius.
    ///
    /// \return Temperature as float value in degree celcius.
    /// \throws Exception
    virtual float temperature() = 0;

    /// \brief Triggers a power reset for the whole system.
    ///
    /// This cuts off the power to the rest of the system and reboots it after 45 seconds.
    /// Use this with care, as is does an unclean shutdown of the whole system.
    /// Data might very well get lost or corrupted, as OS and applications have no time to react.
    virtual void powerReset() = 0;

    /// \brief Closes this controller and frees its resources.
    ///
    /// The controller is no longer usable, after this function returns.
    /// Should be called as soon as the controller is no longer needed.
    virtual void close() noexcept = 0;
};

}

#endif