#include <gameslib/game.hpp>

#include <iostream>
#include <thread>
#include <list>
#include <random>
#include <cmath>

#define ZOOM 3
#define MAP_ZOOM std::pow(2, std::min(std::max(ZOOM, 0), 3))
#define MAP_WIDTH WIDTH / MAP_ZOOM
#define MAP_HEIGHT HEIGHT / MAP_ZOOM

#define LEFT_TOP_BUTTON 17
#define LEFT_BOTTOM_BUTTON 27
#define RIGHT_TOP_BUTTON 23
#define RIGHT_BOTTOM_BUTTON 24

#define EXIT_WITH_ENTER 0

using namespace gameslib;

std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_int_distribution<size_t> randomWidth(1, MAP_WIDTH - 1);
std::uniform_int_distribution<size_t> randomHeight(1, MAP_HEIGHT - 1);

struct Vector
{
  double x, y;
};

struct Snake
{
  Vector head;
  Vector direction;
  std::list<Vector> body;

  Vector createValidApple()
  {
    Vector apple{ randomWidth(generator), randomHeight(generator) };
    for (auto& part : body)
      if (part.x == apple.x && part.y == apple.y)
        return createValidApple();
    return apple;
  }
};

class SnakeGame : public Game
{
  Snake snake;
  Vector apple;
  Button exitButton;
  Button pauseButton;
  bool paused;
  Button resetButton;
  Button scoreButton;
  bool scoreShown;
  size_t highestScore;

public: 
  SnakeGame() 
    : Game(1u, "/dev/input/js0"), 
      exitButton(RIGHT_TOP_BUTTON), pauseButton(RIGHT_BOTTOM_BUTTON), paused(false),
      resetButton(LEFT_TOP_BUTTON), scoreButton(LEFT_BOTTOM_BUTTON), scoreShown(false)
  {}
  void init() override
  {
    snake.direction = {1.0, 0.0};
    snake.head = { MAP_WIDTH / 2, MAP_HEIGHT / 2 };
    snake.body.clear();
    snake.body.push_back({snake.head.x, snake.head.y});
    snake.body.push_back({snake.head.x - 1.0, snake.head.y});
    snake.body.push_back({snake.head.x - 2.0, snake.head.y});
    apple = snake.createValidApple();
  }
  void update(const DeltaTime& dt) override
  {
    if (exitButton.pressed())
      return stop();

    if (pauseButton.pressed())
    {
      paused = !paused;
      if (!paused && scoreShown)
        scoreShown = false;
    }

    if (scoreButton.pressed())
    {
      scoreShown = !scoreShown;
      if (scoreShown && !paused)
        paused = true;
    }

    if (resetButton.pressed())
      init();

    if (paused)
      return;

    if (joystick.axes[JoystickAxis::Lx] > 0.5 && snake.direction.x != -1)
      snake.direction = {1.0, 0.0};
    else if (joystick.axes[JoystickAxis::Lx] < -0.5 && snake.direction.x != 1)
      snake.direction = {-1.0, 0.0};
    if (joystick.axes[JoystickAxis::Ly] > 0.5 && snake.direction.y != -1)
      snake.direction = {0.0, 1.0};
    else if (joystick.axes[JoystickAxis::Ly] < -0.5 && snake.direction.y != 1)
      snake.direction = {0.0, -1.0};

    snake.head.x += snake.direction.x;
    snake.head.y += snake.direction.y;

    if (snake.head.x < 0)
      snake.head.x = MAP_WIDTH;
    else if (snake.head.x > MAP_WIDTH)
      snake.head.x = 0;
    if (snake.head.y < 0)
      snake.head.y = MAP_HEIGHT;
    else if (snake.head.y > MAP_HEIGHT)
      snake.head.y = 0;

    for (auto& part : snake.body)
      if (part.x == snake.head.x && part.y == snake.head.y)
        return init();

    snake.body.push_front({snake.head.x, snake.head.y});
    if (snake.head.x == apple.x && snake.head.y == apple.y)
    {
      apple = snake.createValidApple();
      if (snake.body.size() > highestScore)
        highestScore = snake.body.size();
    }
    else
      snake.body.pop_back();
  }
  void render() override
  {
    display.clear();

    if (scoreShown)
    {
      display.drawTextCenterX((HEIGHT-10) / 2, "Score: " + std::to_string(snake.body.size()));
      display.drawTextCenterX((HEIGHT-10) / 2 + 20, "Highest: " + std::to_string(highestScore));
      return;
    }

    display.drawCircle(apple.x * MAP_ZOOM, apple.y * MAP_ZOOM, MAP_ZOOM / 2);
    for (auto& piece : snake.body)
      display.drawCircle(piece.x * MAP_ZOOM, piece.y * MAP_ZOOM, MAP_ZOOM / 2);
  }
};

int main(void)
{
  std::cout << "Hello, world!" << std::endl;
  SnakeGame game;

#if EXIT_WITH_ENTER
  std::thread loop(&SnakeGame::run, &game);

  std::cout << "Press Enter to exit ";
  std::cin.get();
  game.stop();

  loop.join();
#else
  game.loop();
#endif

  std::cout << "Goodbye, world!" << std::endl;
}
