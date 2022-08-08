#ifndef _I_CHARGEABLE_H
#define _I_CHARGEABLE_H

// Pure Interface for chargeable vehicles - this way charging stations
// Don't need to know anything about Vehicles
class IChargeable {
public:
    virtual ~IChargeable() = default;

    virtual void setCharging() = 0;
    virtual bool isCharging() const = 0;
    virtual void setInWaitQueue() = 0;
    virtual bool isWaitingForQueue() const = 0;
};

#endif //_I_CHARGEABLE_H