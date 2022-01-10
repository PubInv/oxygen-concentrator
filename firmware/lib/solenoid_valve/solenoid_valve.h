// Public Invention's Ox Project is an open source hardware design for an oxygen
// concentrator for use by field hospitals around the world. This team aims to
// design an oxygen concentrator that can be manufactured locally while overcoming
// challenges posed by human resources, hospital location (geographically),
// infrastructure and logistics; in addition, this project attempts the minimum
// documentation expected of their design for international approval whilst
// tackling regulatory requirements for medical devices. Copyright (C) 2021
// Robert Read, Ben Coombs, and Darío Hereñú.

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

#ifndef VALVE_H
#define VALVE_H

#include <core/types.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
//#include <cstdint>
#endif

namespace OxPSA {

    enum class ValveStatus {
        OK,
        MISSED,
        ERROR
    };

    struct ValveState {
        OxCore::u8 name;
        OxCore::u8 pin;
        OxCore::u32 onTime; //ms the valve will be on
        OxCore::u32 offTime; //ms the valve will be off
        OxCore::u32 msLast;
        ValveStatus status;
        bool isOn;
    };

    class Valve {
        private:
            ValveState state;
        public:
            Valve(OxCore::u8 name, OxCore::u8 pin, OxCore::u32 onTime, OxCore::u32 offTime){
                state.name = name;
                state.pin = pin;
                state.onTime = onTime;
                state.offTime = offTime;
                state.msLast = 0;
                state.status = ValveStatus::OK;
                state.isOn = false;
            }
            bool update(OxCore::u32 msNow);
            ValveStatus getValveStatus();
            bool changeTiming(OxCore::u32 onTime, OxCore::u32 offTime);
            bool forceValveTrigger();
    };

}

#endif
