#include <gameslib/display.hpp>
#include <cmath>

namespace gameslib 
{

  Display::Display(size_t bus)
    : i2cBus(bus), i2cAddress(0x3c * 2)
  {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
      &u8g2,
      U8G2_R0,
      u8x8_byte_arm_linux_hw_i2c,
      u8x8_arm_linux_gpio_and_delay);

    init_i2c_hw(&u8g2, i2cBus);
    u8g2_SetI2CAddress(&u8g2, i2cAddress);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_extended);
    u8g2_ClearBuffer(&u8g2);
  }

  Display::~Display()
  {
    u8g2_SetPowerSave(&u8g2, 1);
    done_i2c();
    done_user_data(&u8g2);
  }

  void Display::clear()
  {
    u8g2_ClearBuffer(&u8g2);
  }

  void Display::render()
  {
    u8g2_SendBuffer(&u8g2);
  }

  void Display::draw(double x, double y, bool value)
  {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
      return;
    u8g2_SetDrawColor(&u8g2, value ? 1 : 0);
    u8g2_DrawPixel(&u8g2, static_cast<size_t>(std::round(x)), static_cast<size_t>(std::round(y)));
  }

  void Display::drawCircle(double x, double y, double radius, bool value)
  {
    if (x+radius < 0 || x-radius >= WIDTH || y+radius < 0 || y-radius >= HEIGHT)
      return;
    if (value)
      u8g2_DrawDisc(&u8g2, static_cast<size_t>(std::round(x)), static_cast<size_t>(std::round(y)), static_cast<size_t>(std::round(radius)), U8G2_DRAW_ALL);
    else
      u8g2_DrawCircle(&u8g2, static_cast<size_t>(std::round(x)), static_cast<size_t>(std::round(y)), static_cast<size_t>(std::round(radius)), U8G2_DRAW_ALL);
  }

  void Display::drawText(double x, double y, const std::string& text)
  {
    u8g2_DrawStr(&u8g2, static_cast<size_t>(std::round(x)), static_cast<size_t>(std::round(y)), text.c_str());
  }

  void Display::drawTextCenterX(double y, const std::string& text)
  {
    size_t width = u8g2_GetUTF8Width(&u8g2, text.c_str());
    u8g2_DrawUTF8(&u8g2, (WIDTH-width)/2, static_cast<size_t>(std::round(y)), text.c_str());
  }

}