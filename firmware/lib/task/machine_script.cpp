/*
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

#include <machine_script.h>
#include <debug.h>
#include <string.h>
#include <strcasestr.h>

using namespace OxCore;




int
parse_param(char *buffer, const char *value, char **rvalue) {
  char *item = strstr(buffer, value);
  if (!item) return 0;

  if (*rvalue) free(*rvalue);

  item += strlen(value);
  while (*item == ' ' || *item == '.') item++;
  char *end = strchr(item, '\n');
  *end = '\0';
  *rvalue = (char *)malloc(strlen(item) + 1);
  strcpy(*rvalue, item);
  *end = '\n';
  return 1;
}

int
parse_param(char *buffer, const char *value, int *rvalue) {
  char *item = strstr(buffer, value);
  if (!item) return 0;

  item += strlen(value);
  while (*item == ' ' || *item == '.') item++;
  char *end = strchr(item, '\n');
  *end = '\0';
  if (strncasecmp(item, "On", 2) == 0) *rvalue = 1;
  else if (strncasecmp(item, "Yes", 3) == 0) *rvalue = 1;
  else if (strncasecmp(item, "True", 4) == 0) *rvalue = 1;
  else if (strncasecmp(item, "Off", 3) == 0) *rvalue = 0;
  else if (strncasecmp(item, "No", 2) == 0) *rvalue = 0;
  else if (strncasecmp(item, "False", 5) == 0) *rvalue = 0;
  else *rvalue = atoi(item);
  *end = '\n';
  return 1;
}

int parse_param_state(char *buffer, const char *value, char **rvalue) {
  char str[strlen(value) + 9];
  sprintf(str, "[[ Name. %s", value);
  char *item = strcasestr(buffer, str);
  if (!item) return 0;

  if (*rvalue) free(*rvalue);

  item += strlen(str);
  while (*item == ' ' || *item == '\n') item++;

  char *end = strstr(item, "[[");
  if (end) *--end = '\0';
  else {
    end = strrchr(item, '\n');
    *end = '\0';
  }
  *rvalue = (char *) malloc(strlen(item) + 1);
  strcpy(*rvalue, item);
  if (end) *end = '\n';
  return 1;
}

int parse_state(char *state, struct phase_t *phase_list[]) {
  if (state == NULL || strlen(state) == 0) return 0;
  char *ptr  = NULL;

  int phasecnt = 0;
  for (ptr = state; *ptr; ptr++) if (*ptr == '[') phasecnt++;

  struct phase_t *pl = (struct phase_t *) malloc(sizeof (struct phase_t) * phasecnt);

  *phase_list = pl;

  char tempphase[strlen(state)];
  strcpy(tempphase, state);

  char *start = tempphase;
  int pcount = 0;

   while (ptr = strtok(start, "[")) {
     start = NULL;
     if (strlen(ptr) == 0) continue;

     char *tptr = NULL;
     if (tptr = strstr(ptr, "Phases.Duration")) {
       tptr += 15;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].duration = atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Fan.Speed")) {
       tptr += 16;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].fan_speed = atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Fan.Flow")) {
       tptr += 15;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].fan_flow = atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Preheat.Temp")) {
       tptr += 19;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].preheat_temp = atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Preheat.Current")) {
       tptr += 22;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].preheat_current = atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Preheat.Ramp")) {
       tptr += 19;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].preheat_ramp = atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Stack.Temp")) {
       tptr += 17;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].stack_temp = atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Stack.Current")) {
       tptr += 20;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].stack_current= atoi(tptr);
     }
     if (tptr = strstr(ptr, "Phases.Stack.Ramp")) {
       tptr += 17;
       while (*tptr == ' ' || *tptr == '.') tptr++;
       pl[pcount].stack_ramp = atoi(tptr);
     }
     pcount++;
   }
  return phasecnt;
}

void MachineScript::DeleteAllPhases() {
  numPhases = 0;
}
bool MachineScript::AppendPhase(MachinePhase p) {
  if (numPhases < (MAX_NUM_PHASES-1)) {
    //    phase[[numPhases++] = p;
    return true;
  } else {
    DebugLn<const char *>("MAXIMUM_NUM_PHASES Exceeded!");
    return false;
  }
}

// Beginning hacking of this...
MachineScript *MachineScript::parse_buffer_into_new_script(char *packetBuffer) {
  MachineScript *ms = new MachineScript();
  if (DEBUG_MS > 1) {
    DebugLn<const char *>("BUFFER BEGIN");
    DebugLn<const char *>(packetBuffer);
    DebugLn<const char *>("BUFFER END");
  }
  char *gName = NULL;
  char *gTimestamp = NULL;
  int gDryRun = 1;
  int gNonce = -1;
  int gMaxRampUp = 1;
  int gMaxRampDown = 2;
  char *gWarmUp = NULL;
  char *gCooldown = NULL;
  char *gEmShutdown = NULL;
  char *gOperation = NULL;
  int tempnonce;

  if (DEBUG_MS > 1) {
    Debug<const char *>("PARSING SCRIPT \n");
  }

  parse_param(packetBuffer, "Nonce", &tempnonce);
  if (tempnonce <= gNonce) exit(1);
  gNonce = tempnonce;

  parse_param(packetBuffer, "Name", &gName);
  parse_param(packetBuffer, "TimeStamp", &gTimestamp);
  parse_param(packetBuffer, "DryRun", &gDryRun);
  parse_param(packetBuffer, "MaxRampUp", &gMaxRampUp);
  parse_param(packetBuffer, "MaxRampDown", &gMaxRampDown);

  parse_param_state(packetBuffer, "Warmup", &gWarmUp);
  parse_param_state(packetBuffer, "Cooldown", &gCooldown);
  parse_param_state(packetBuffer, "EmergencyShutdown", &gEmShutdown);
  parse_param_state(packetBuffer, "Operation", &gOperation);

  if (DEBUG_MS > 1) {
    DebugLn<const char *>("Name is \n");
    DebugLn<const char *>(gName);
    DebugLn<const char *>("Timestamp is \n");
    DebugLn<const char *>(gTimestamp);
    DebugLn<const char *>("Dryrun is \n");
    DebugLn<const char *>( gDryRun ? "True" : "False");
    DebugLn<const char *>("Nonce is n");
    DebugLn<int>(gNonce);
    DebugLn<const char *>("Max UP is %d\n");
    DebugLn<int>(gMaxRampUp);
    DebugLn<const char *>("Max DOWN is n");
    DebugLn<int>(gMaxRampDown);
    DebugLn<const char *>("Warmup Script:\n");
    DebugLn<const char *>(gWarmUp);
    DebugLn<const char *>("\n----\n");
    DebugLn<const char *>("Cooldown Script:\n");
    DebugLn<const char *>("\n----\n");
    DebugLn<const char *>(gCooldown);
    DebugLn<const char *>("Emergency Script:\n");
    DebugLn<const char *>("\n----\n");
    DebugLn<const char *>(gEmShutdown);
    DebugLn<const char *>("Operation Script:\n");
    DebugLn<const char *>("\n----\n");
    DebugLn<const char *>(gOperation);
    DebugLn<const char *>("\n----\n");
  }

  if (DEBUG_MS > 0) {
    parse_phases_from_state_script(MachineState::Warmup,gWarmUp,ms);
    parse_phases_from_state_script(MachineState::Cooldown,gCooldown,ms);
    parse_phases_from_state_script(MachineState::EmergencyShutdown,gEmShutdown,ms);
    parse_phases_from_state_script(MachineState::NormalOperation,gOperation,ms);
  }
  return ms;
}

void parse_phases_from_state_script(MachineState m_state,char *script,MachineScript *ms) {
  struct phase_t *p;
  int c = parse_state(script, &p);

  Debug<const char *>("This state hadN phases: ");
  DebugLn<int>(c);
  for (int x = 0; x < c; x++) {
    Debug<const char *>("Phase :\n");
    DebugLn<int>(x);
    Debug<const char *>("  Duration \n");
    DebugLn<int>(p[x].duration);
    Debug<const char *>("  preheat temp \n");
    DebugLn<int>(p[x].preheat_temp);
    Debug<const char *>("  stack temp \n");
    DebugLn<int>(p[x].stack_temp);
    Debug<const char *>("  stack ramp \n");
    DebugLn<int>(p[x].stack_ramp);
  }
}
