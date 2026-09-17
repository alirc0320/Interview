#pragma once
#include <chrono>

enum class ThermalState
{
    Normal,
    Warning,
    Throttled,
    Shutdown,
};

struct Sample
{
    std::chrono::steady_clock::time_point timestamp;
    double temperature_c;
};

// TODO: implement. Use hysteresis so noisy readings don't rapidly
// toggle states; reject stale/invalid samples.
class ThermalController
{
  public:
    ThermalController(double warn_c, double throttle_c, double shutdown_c, double recover_c);

    ThermalState on_sample(const Sample& sample);
    ThermalState state() const;

  private:
    // TODO
};
