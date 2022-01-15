#pragma once
#include <chrono>

namespace gameslib
{

  struct DeltaTime
  {
    std::chrono::high_resolution_clock::time_point last;
    std::chrono::high_resolution_clock::time_point current;
    std::chrono::duration<double> delta;

    DeltaTime()
      : last(std::chrono::high_resolution_clock::now()), current(std::chrono::high_resolution_clock::now()), delta(std::chrono::duration<double>::zero())
    {}

    void update()
    {
      last = current;
      current = std::chrono::high_resolution_clock::now();
      delta = current - last;
    }

    double getSeconds() const
    {
      return delta.count();
    }

    double getMilliseconds() const
    {
      return delta.count() * 1000.0;
    }

    double getMicroseconds() const
    {
      return delta.count() * 1000000.0;
    }

    double getNanoseconds() const
    {
      return delta.count() * 1000000000.0;
    }

    double getMinutes() const
    {
      return delta.count() / 60.0;
    }
  };

}