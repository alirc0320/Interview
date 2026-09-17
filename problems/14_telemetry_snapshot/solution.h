#pragma once
#include <cstdint>

struct Telemetry
{
    std::uint32_t temperature_c;
    std::uint32_t power_mw;
    std::uint64_t uptime_ms;
    // TODO: add more fields as needed
};

// TODO: implement. A firmware thread updates fields; a host thread
// must never observe a torn/mixed snapshot, without holding a
// long-duration global lock. Consider a sequence-counter or
// double-buffering approach.
class TelemetryStore
{
  public:
    void update(const Telemetry& t);
    Telemetry snapshot() const;

  private:
    // TODO
};
