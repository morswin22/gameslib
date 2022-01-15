#pragma once
#include <u8g2port.h>
#include <string>

namespace gameslib
{
  #define WIDTH 128.0
  #define HEIGHT 64.0

  class Display
  {
  private:
    size_t i2cBus;
    size_t i2cAddress;
    u8g2_t u8g2;
    
  public:
    Display(size_t bus);
    ~Display();

    void clear();
    void draw(double x, double y, bool value = true);
    void drawCircle(double x, double y, double radius, bool filled = true);
    void drawText(double x, double y, const std::string& text);
    void drawTextCenterX(double y, const std::string& text);

    void render();
  };

}