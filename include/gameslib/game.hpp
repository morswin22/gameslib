#pragma once
#include <gameslib/deltatime.hpp>
#include <gameslib/display.hpp>
#include <gameslib/joystick.hpp>
#include <gameslib/button.hpp>
#include <joystick.hh>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>

namespace gameslib
{

  class Game
  {
  protected:
    std::atomic<bool> running;
    size_t frame;
    DeltaTime deltaTime;

    Display display;

    Joystick joystickDevice;
    std::queue<JoystickEvent> joystickEvents;
    std::mutex joystickEventsMutex;
    std::thread joystickThread;
    JoystickState joystick;

  public:
    Game(size_t displayBus, const std::string& joystickDevicePath);
    virtual ~Game();

    virtual void init() {};
    virtual void update(const DeltaTime& dt) {};
    virtual void render() {};

    static void run(Game* game);
    void loop();
    void stop();
    const std::atomic<bool>& isRunning() const;

    static void startPollingJoystick(Game* game);
    void pollJoystickEvents();
    void applyJoystickEvents();
  };

}