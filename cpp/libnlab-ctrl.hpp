/*
 * controller-libs
 * Copyright (c) 2021 Wahtari GmbH
 *
 * All source code in this file is subject to the included LICENSE file.
 */

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

enum ErrCode {
    Generic  = 0,
    NotFound = 1
};

class Exception : public std::exception {
    public:
        Exception(const ErrCode code, const std::string msg);

        const char* what() const throw ();
        ErrCode     code() const;

    private:
        const ErrCode code_;
        const std::string msg_;
};

//#############//
//### Types ###//
//#############//

enum StatusLEDState {
    OFF      = 0,
    ON       = 1,
    Blinking = 2
};

struct StepMotor {
    std::string id;
    std::string name;
    int         step;
    int         minStep;
    int         maxStep;
};

struct LED {
    std::string id;
    std::string name;
    bool        on;
    int         brightness;
    bool        strobeOn;
    int         strobeDelay;
};

struct Switch {
    std::string id;
    std::string name;
    bool        on;
};

//##################//
//### Controller ###//
//##################//

struct ControllerOpts {
    std::string stateDir;
};

class Controller {
    public:
        typedef std::shared_ptr<Controller> Ptr;

        static Ptr open(const std::string& backendID, const std::string& devPath, const ControllerOpts& opts);

        virtual std::vector<StepMotor> getStepMotors() = 0;
        virtual StepMotor              getStepMotor(const std::string& id) = 0;
        virtual void                   setStepMotorRelPos(const std::string& id, int step) = 0;
        virtual void                   setStepMotorAbsPos(const std::string& id, int step) = 0;

        virtual void setStatusLED                (StatusLEDState state) = 0;
        virtual void setStatusLEDBlinkingDuration(long long int duration) = 0;

        virtual std::vector<LED> getLEDs() = 0;
        virtual LED              getLED(const std::string& id) = 0;
        virtual void             setLED(const std::string& id, bool on) = 0;
        virtual void             setLEDStrobe(const std::string& id, bool on) = 0;
        virtual void             setLEDBrightness(const std::string& id, int brightness) = 0;
        virtual void             setLEDStrobeDelay(const std::string& id, int delay) = 0;

        virtual std::vector<Switch> getSwitches() = 0;
        virtual Switch              getSwitch(const std::string& id) = 0;
        virtual void                setSwitch(const std::string& id, bool on) = 0;

        virtual float temperature() = 0;

        virtual void close() noexcept = 0;
};

}

#endif