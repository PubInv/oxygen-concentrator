// Copyright (C) 2021 Robert Read, Ben Coombs.

// This program includes free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

// See the GNU Affero General Public License for more details.
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// This is a driver for "dual mount" of this fan (which is itself
// two counterrotating fans:
// https://www.delta-fan.com/Download/Spec/GFC0412DS-TP01.pdf


#ifndef DELTAFANS_H
#define DELTAFANS_H

#include <core.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
//#include <cstdint>
#include <math.h>
#endif


// NOTE: AT present, this is really a transistory in front
// of a twelve-volt fan. So we will control it with a PWM signal.
// Different Fans may require different drivers in the future.
// In particular, we hope to use a 4-wire fan with a tachometer.

// NOTE: PWM control of these fans works ONLY when they are powered
// at full power. If you use PWM contol of the main power, the
// PWM of contol of the individual fans does not work!
// void tachISR();

#define NUMBER_OF_FANS 4

// because these are interrupts, they cannot be members,
// and making them static is complicated...

//namespace tach_data {
  void refresh_tach_data(uint8_t i);
  void tachISR(uint8_t i);
  void tachISR0();
  void tachISR1();
  void tachISR2();
  void tachISR3();
//}


class DeltaFans {
public:
  const char * name;
  uint8_t id;
  uint8_t pin;
  float _pwm_ratio[NUMBER_OF_FANS] = {0.0,0.0,0.0,0.0};

  int DEBUG_FAN = 1;
  int MOTOR_OUT_PIN = 8;
  unsigned int PWM_PIN[NUMBER_OF_FANS];
  unsigned int TACH_PIN[NUMBER_OF_FANS];

  int SPEED_MAX = 100;// in %
  int SPEED_MIN = 0;//in %

public:
  void _init();
  unsigned long _calcRPM(uint8_t i);
  DeltaFans() {
    _init();
  };
  DeltaFans(const char * name, uint8_t id, uint8_t pin, float pwm_ratio){
    _init();
    this->name = name;
    this->id = id;
    this->pin = pin;
  };
  ~DeltaFans(){};

  void printRPMS();
  void motorControl(int s);

  void PWMMotorControl(int s, int m);
  void update(float pwm_ratio);
};


#endif
