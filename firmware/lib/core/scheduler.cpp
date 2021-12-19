
/*
Public Invention's Ox Project is an open source hardware design for an oxygen
concentrator for use by field hospitals around the world. This team aims to
design an oxygen concentrator that can be manufactured locally while overcoming
challenges posed by human resources, hospital location (geographically),
infrastructure and logistics; in addition, this project attempts the minimum
documentation expected of their design for international approval whilst
tackling regulatory requirements for medical devices. Copyright (C) 2021
Robert Read, Ben Coombs, and Darío Hereñú.

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

#include "HAL/linux/linux.h"
#include "scheduler.h"
#include "util.h"
#include <iostream>

namespace OxCore {

bool Scheduler::AddTask(Task *task, TaskId id, TaskPriority priority) {
    if (WithinArrayBounds(_numberOfTasks, MAX_TASKS)) {
        TaskState state = task->Init(id, priority);
        if (state == TaskState::Undefined) {
            map.add(id, task);
            task->_id = id;
            return true;
        } else {
            std::cout << "Something went wrong!\n";
        }
    }
    // Out of bounds
    return false;
}

TaskState Scheduler::RunTaskById(uint32_t msNow, TaskId id) {
    std::cout << "Running task! id: " << id << std::endl;
    Task* tp = map.getValue(id);
    if (tp == nullptr) {
        return TaskState::Error;
    } else {
        _currentRunningTaskId = id;
        tp->Run(msNow);
        return TaskState::Running;
    }
}

TaskId Scheduler::GetRunningTaskId() const {
    return _currentRunningTaskId;
}

Task* Scheduler::GetTaskById(TaskId id) {
    Task* tp = map.getValue(id);
    return tp;
}

void Scheduler::RemoveTaskById(TaskId id) {    
    //_tasks[index] = nullptr;
}

void Scheduler::RemoveAllTasks() {
    //
}

}