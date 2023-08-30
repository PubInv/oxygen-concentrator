/*
 Copyright (C) 2022 Robert L. Read

 This program includes free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

See the GNU Affero General Public License for more details.
You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef COG_TASK_H
#define COG_TASK_H

#ifdef ARDUINO
#include <Arduino.h>
#endif
#include <core.h>
#include "../collections/array.h"
#include "heater.h"
#include "stack.h"
#include "abstract_ps.h"
#include "mostplus_flow.h"
#include <machine.h>

#include <abstract_temperature.h>

#include <OnePinHeater.h>
#include <MAX31850.h>

#include <temp_refresh_task.h>


#include "model.h"


namespace OxApp
{
    // Runs the Pressure Swing Adsorption cycle
    class CogTask : public OxCore::Task
    {
    public:
      TempRefreshTask* tempRefreshTask;
      int DEBUG_LEVEL = 0;
      // TODO: This should probably be done dynamically, not here...

      // There are really several senosrs, but they are indexed!
      const static int NUM_TEMPERATURE_SENSORS = 3;
      const static int NUM_TEMPERATURE_INDICES = 2;
      const static int NUM_FANS = 1;
      const static int NUM_STACKS = 1;

      // WARNING! This is a fragile; I believe a rate based algorithm is better.
      unsigned long begin_down_time = 0;

      //      void updateTemperatures();

      Temperature::AbstractTemperature* _temperatureSensors;

      AbstractPS* _stacks[NUM_STACKS];
      //      MostPlusFlow _flowsensor;
      //      SensirionFlow *_flowsensor;
    private:
      bool _init() override;
      bool _run() override;
      // This would go into the abstract class.

      MachineState _executeBasedOnState(MachineState ms);
      MachineState _updatePowerComponentsOperation(IdleOrOperateSubState i_or_o);
      MachineState _updatePowerComponentsOff();
      MachineState _updatePowerComponentsWarmup();
      MachineState _updatePowerComponentsIdle();
      MachineState _updatePowerComponentsCooldown();
      MachineState _updatePowerComponentsCritialFault();
      MachineState _updatePowerComponentsEmergencyShutdown();
      MachineState _updatePowerComponentsOffUserAck();
      void _updatePowerComponentsVoltage(float voltage);
      void _configTemperatureSensors();
      //      void _readTemperatureSensors();

      void _updateFanSpeed(float percentage);
      void _updateStackVoltage(float voltage);
      void _updateStackAmperage(float amperage);
      // We will use a model for mocking, which may grow into
      // something...
      Model model;

      // We have some mock heaters for simulation, but
      // we don't sue them in the Ribbon Fish configuration
#ifndef RIBBONFISH
      void RunForward(float t,Model& m);
#endif
    };
}

#endif
