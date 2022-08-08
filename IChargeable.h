#ifndef _I_CHARGEABLE_H
#define _I_CHARGEABLE_H

// Pure Interface for chargeable vehicles - using this interface
// keeps charging stations from having to know anything about Vehicles
class IChargeable {
public:
    virtual ~IChargeable() = default;

    // Handle setting state to "charging"
    virtual void setCharging() = 0;

    // Return "true" if in "charging" state
    virtual bool isCharging() const = 0;

    // Handle setting state to "in_wait_queue"
    virtual void setInWaitQueue() = 0;

    // Return "true" if in "not en_route & not yet in charging queue" state (i.e. newly arrived)
    virtual bool isWaitingForQueue() const = 0;
};

#endif //_I_CHARGEABLE_H