#pragma once
#include <pigpio.h>

namespace gameslib
{

  class Button
  {
  private:
    int pin;
  public:
    Button(int pin)
      : pin(pin)
    {
      gpioSetMode(pin, PI_INPUT);
      gpioSetPullUpDown(pin, PI_PUD_DOWN);
    }

    ~Button()
    {
      gpioSetPullUpDown(pin, PI_PUD_OFF);
    }

    bool pressed()
    {
      return gpioRead(pin) == PI_HIGH; 
    }
  };

}