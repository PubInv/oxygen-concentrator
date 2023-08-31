/*

  stage2SerialReport.cpp -- read temperatures sensors

  Copyright (C) 2023 Robert Read.

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

#include <stage2SerialReportTask.h>
#include <stage2_config.h>


bool Stage2SerialReportTask::_run()
{
  if (DEBUG_SERIAL_REPORT > 0) {
    OxCore::Debug<const char *>("Running Stage2SerialReport Task\n");
    delay(100);
  }
  MachineConfig *s2c = getConfig();
  s2c->outputStage2Report(s2c->s2sr);
}

bool Stage2SerialReportTask::_init()
{
    OxCore::Debug<const char *>("Stage2SerialReport Task init\n");
}
Stage2SerialReportTask::Stage2SerialReportTask() {
}