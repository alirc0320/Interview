#pragma once

enum class BootState
{
    Off,
    Powering,
    Resetting,
    Initializing,
    Ready,
    Failed,
};

// TODO: implement. Reject invalid transitions; any failure must
// enter a safe state (Failed). Add timeouts, retry limits, and a
// stored failure reason as needed.
class BootStateMachine
{
  public:
    BootState state() const;
    bool transition_to(BootState next);

  private:
    // TODO
};
