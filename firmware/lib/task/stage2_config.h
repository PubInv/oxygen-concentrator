/*
  stage2_config -- configuration specifically for the Stage2 heaters of the high-oxygen experiment

  Copyright 2023, Robert L. Read

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

#ifndef STAGE2_CONFIG_H
#define STAGE2_CONFIG_H

#include <machine_core_defs.h>


struct Stage2StatusReport {
  MachineState ms_int1;
  MachineState ms_ext1;
  MachineState ms_ext2;
  float target_int1_temp_C;
  float target_ext1_temp_C;
  float target_ext2_temp_C;
  float int1_temp_C;
  float ext1_temp_C;
  float ext2_temp_C;
  float heater_duty_cycle_int1;
  float heater_duty_cycle_ext1;
  float heater_duty_cycle_ext2;
};


#endif