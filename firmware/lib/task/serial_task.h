#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#ifdef ARDUINO
#include <Arduino.h>
#else // Native
#include <iostream>
#endif

#include <core.h>

#include <machine.h>

#define SERIAL_BAUD_RATE 19200
// #define SERIAL_BAUD_RATE 9600
#define SERIAL_DELAY 1000
#define SERIAL_TIMEOUT_MAX 10000

#define INPUT_BUFFER_SIZE 256


namespace OxApp
{
  class AbstractSerialTask : public OxCore::Task {
  private:
    bool one_char_command_found(int num_read, char buffer[], int k);

  public:
        bool initialization_success;
        bool new_from_UI;
    // A buffer is needed to cross boundaries that may occur
    // within a PIRCS command within the
    char input_buffer[INPUT_BUFFER_SIZE];
    bool _init() override;
    bool _run() override;

    virtual int clear_buffers(char buffer[]);
    virtual bool listen(char buffer[], int length);
  };

  class SerialTask : public AbstractSerialTask {
  public:
    bool _init() override;
    bool _run() override;
  };

  class Stage2SerialTask : public AbstractSerialTask {
  public:

    bool _init() override;
    bool _run() override;
  };

}

#endif
