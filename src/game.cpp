#include <gameslib/game.hpp>
#include <pigpio.h>
#include <chrono>

namespace gameslib 
{

  Game::Game(size_t displayBus, const std::string& joystickDevicePath)
   : display(displayBus), joystickDevice(joystickDevicePath), joystickThread(&Game::startPollingJoystick, this), joystick()
  {
    if (!gpioInitialise())
      throw std::runtime_error("Failed to initialise GPIO");
    if (!joystickDevice.isFound())
      throw std::runtime_error("Could not open joystick device");
  }

  Game::~Game()
  {
    joystickThread.join();
    gpioTerminate();
  }

  void Game::loop()
  {
    // TODO maybe move out into base class init()
    running = true;
    frame = 0u;
    deltaTime = DeltaTime();
    init();
    while (running)
    {
      applyJoystickEvents();
      deltaTime.update();
      update(deltaTime);
      render();
      display.render();
      frame++;
    }
  }

  void Game::run(Game* game)
  {
    game->loop();
  }

  void Game::stop()
  {
    running = false;
  }

  const std::atomic<bool>& Game::isRunning() const
  {
    return running;
  }

  void Game::startPollingJoystick(Game* game)
  {
    game->pollJoystickEvents();
  }

  void Game::pollJoystickEvents()
  {
    while (running)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      JoystickEvent joystickEvent;
      if (joystickDevice.sample(&joystickEvent))
      {
        std::lock_guard<std::mutex> lock(joystickEventsMutex);
        joystickEvents.push(joystickEvent);
      }
    }
  }

  void Game::applyJoystickEvents()
  {
    std::lock_guard<std::mutex> lock(joystickEventsMutex);
    while (!joystickEvents.empty())
    {
      JoystickEvent event = joystickEvents.front();
      joystickEvents.pop();
      if (event.isButton())
        joystick.applyButton(event.number, event.value);
      else
        joystick.applyAxis(event.number, event.value);
    }
  }

}