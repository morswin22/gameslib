#pragma once

#include <iostream>

namespace gameslib
{

  namespace JoystickAxis
  {
    enum AXIS
    {
      Lx = 0,
      Ly = 1,
      Lz = 2,
      Rx = 3,
      Ry = 4,
      Rz = 5,
      SelectX = 6,
      SelectY = 7
    };
  }

  namespace JoystickButton
  {
    enum BUTTON
    {
      X = 0,
      O = 1,
      Triangle = 2,
      Square = 3,
      L1 = 4,
      R1 = 5,
      L2 = 6,
      R2 = 7,
      Share = 8,
      Options = 9,
      PS = 10,
      ThumbL = 11,
      ThumbR = 12
    };
  }

  struct JoystickState
  {
    double axes[8]{ 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0 };
    bool buttons[13]{ false, false, false, false, false, false, false, false, false, false, false, false, false };

    void applyAxis(int axis, int value)
    {
      axes[axis] = static_cast<double>(value) / 32767.0;
    }

    void applyButton(int button, bool value)
    {
      buttons[button] = value;
    }
  };

}