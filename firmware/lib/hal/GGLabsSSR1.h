// Copyright (C) 2021
// Robert Read.

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


// This is an interface for the GGLabs SSR1 Solid-state Relay.
// This is our attempt to control 110VAC, in particular to control a heater.
// This is a very easy board to use; it has two channels and lets us
// control them directly from a GPIO pin

#ifndef GGLABSSSR1
#define GGLABSSSR1

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#include <model.h>


class GGLabsSSR1 {
public:
  GGLabsSSR1();
  const int NUM_CHANNELS = 1;
  // NOTE: GPIO PIN 53 on my DUE appears to be stuck HIGH!
  // This card is currently burned out
  int CHANNEL_0_PIN = 51;
  //  int CHANNEL_1_PIN = 51;
  int CHANNEL_1_PIN = 52;
  int channel_pins[2] = {CHANNEL_0_PIN,CHANNEL_1_PIN};
  void init();

  bool setHeater(int heaterNum, bool onIfTrue);
  virtual ~GGLabsSSR1() {};
};


#endif
