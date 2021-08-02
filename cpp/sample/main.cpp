/*
 * controller-libs
 * Copyright (c) 2021 Wahtari GmbH
 *
 * All source code in this file is subject to the included LICENSE file.
 */

#include <string>
#include <iostream>

#include <libnlab-ctrl.hpp>

using namespace std;
using namespace nlab::ctrl;

void printStepMotor(StepMotor sm) {
    printf("{id: %s, name: %s, step: %d, minStep: %d, maxStep: %d}", sm.id.c_str(), sm.name.c_str(), sm.step, sm.minStep, sm.maxStep);
}

void printLED(LED led) {
    printf("{id: %s, name: %s, on: %d, brightness: %d, strobeOn: %d, strobeDelay: %d}", led.id.c_str(), led.name.c_str(), led.on, led.brightness, led.strobeOn, led.strobeDelay);
}

void printSwitch(Switch sw) {
    printf("{id: %s, name: %s, on: %d}", sw.id.c_str(), sw.name.c_str(), sw.on);
}

void printGPIOPin(GPIOPin gp) {
    printf("{id: %s, name: %s, on: %d, direction: %d}", gp.id.c_str(), gp.name.c_str(), gp.on, gp.direction);
}

void showAndModifySingleStepMotor(Controller::Ptr ctrl, const string& id) {
    StepMotor sm = ctrl->getStepMotor(id);
    cout << endl << "first step motor: ";
    printStepMotor(sm);
    cout << endl;
    
    int v = sm.step + (sm.step < 5 ? 1 : -1);
    ctrl->setStepMotorRelPos(id, v);
    cout << "set rel pos to " << to_string(v) << endl << endl;
}

void showAndModifySingleLED(Controller::Ptr ctrl, const string& id) {
    LED led = ctrl->getLED(id);
    cout << endl << "first led: ";
    printLED(led);
    cout << endl;

    ctrl->setLED(id, !led.on);
    cout << "set led to " << to_string(!led.on) << endl;

    ctrl->setLEDStrobe(id, !led.strobeOn);
    cout << "set led strobe to " << to_string(!led.strobeOn) << endl;

    int v = led.brightness + (led.brightness < 50 ? 5 : -5);
    ctrl->setLEDBrightness(id, v);
    cout << "set led brightness to " << to_string(v) << endl;

    v = led.strobeDelay + (led.strobeDelay < 200 ? 20 : -20);
    ctrl->setLEDStrobeDelay(id, v);
    cout << "set led strobe delay to " << to_string(v) << endl << endl;
}

void showAndModifySingleSwitch(Controller::Ptr ctrl, const string& id) {
    Switch sw = ctrl->getSwitch(id);
    cout << endl << "first switch: ";
    printSwitch(sw);
    cout << endl;

    ctrl->setSwitch(id, !sw.on);
    cout << "set switch to " << to_string(!sw.on) << endl << endl;
}

void showAndModifySingleGPIOPin(Controller::Ptr ctrl, const string& id) {
    GPIOPin gp = ctrl->getGPIOPin(id);
    cout << endl << "first gpioPin: ";
    printGPIOPin(gp);
    cout << endl;

    ctrl->setGPIOPin(id, !gp.on);
    cout << "set gpioPin to " << to_string(!gp.on) << endl << endl;
}

int main() {
    Controller::Ptr ctrl;
    try {
        // Get a the list of available controllers.
        // Note that dummy controllers are not part included in that.
        vector<Info> infoList = Controller::list();
        cout << "found " << to_string(infoList.size()) << " real controller(s), but using dummy now" << endl;

        // Open the controller.
        ctrl = Controller::open("dummy", "dummy", {.stateDir = "/tmp/nlab-ctrl-state"});

        // Activate the status LED, while sample is running.
        ctrl->setStatusLED(ON);

        // Retrieve all its step motors.
        vector<StepMotor> stepMotors = ctrl->getStepMotors();
        cout << "found " << to_string(stepMotors.size()) << " step motor(s):" << endl;
        for (const auto& sm : stepMotors) {
            cout << " - ";
            printStepMotor(sm);
            cout << endl;
        }

        // Get a single step motor and show all its functions.
        if (stepMotors.size() > 0) {
            showAndModifySingleStepMotor(ctrl, stepMotors[0].id);
        }

        // Retrieve all its leds.
        vector<LED> leds = ctrl->getLEDs();
        cout << "found " << to_string(leds.size()) << " led(s):" << endl;
        for (const auto& led : leds) {
            cout << " - ";
            printLED(led);
            cout << endl;
        }

        // Get a single led and show all its functions.
        if (leds.size() > 0) {
            showAndModifySingleLED(ctrl, leds[0].id);
        }

        // Retrieve all its switches.
        vector<Switch> switches = ctrl->getSwitches();
        cout << "found " << to_string(switches.size()) << " switch(es):" << endl;
        for (const auto& sw : switches) {
            cout << " - ";
            printSwitch(sw);
            cout << endl;
        }

        // Get a single switch and show all its functions.
        if (switches.size() > 0) {
            showAndModifySingleSwitch(ctrl, switches[0].id);
        }

        // Retrieve all its gpio pins.
        vector<GPIOPin> gpioPins = ctrl->getGPIOPins();
        cout << "found " << to_string(gpioPins.size()) << " gpioPin(s):" << endl;
        for (const auto& gp : gpioPins) {
            cout << " - ";
            printGPIOPin(gp);
            cout << endl;
        }

        // Get a single gpioPin and show all its functions.
        if (gpioPins.size() > 0) {
            showAndModifySingleGPIOPin(ctrl, gpioPins[0].id);
        }

        // Retrieve the temperature.
        cout << "temperature: " << to_string(ctrl->temperature()) << "°C" << endl;

        // Show how to catch a not found exception.
        try {
            StepMotor sm = ctrl->getStepMotor("doesnotexist");
        } catch (Exception& e) {
            if (e.code() == NotFound) {
                cout << "correctly catched NotFound exception for non-existing step motor" << endl;
            } else {
                // Something else happened, let it bubble up.
                throw e;
            }
        }
    } catch (Exception& e) {
        cout << "exception! code: " << to_string(e.code()) << ", message: " << e.what() << endl;
        return 1;
    }

    if (ctrl != nullptr) {
        try {
            ctrl->setStatusLED(OFF);
        } catch (Exception& e) {
            cout << "exception status led off! code: " << to_string(e.code()) << ", message: " << e.what() << endl;
            return 2;
        }
    }
    
    return 0;
}
