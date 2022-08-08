#ifndef _CHARGING_STATION_H
#define _CHARGING_STATION_H

#include <queue>
#include <set>
#include "./IChargeable.h"

using std::shared_ptr;

// Class to manage charging queue
class ChargingStation {
private:
    const int cNumberOfChargingSlots;

    std::queue<shared_ptr<IChargeable>> waitQueue;
    std::set<shared_ptr<IChargeable>> chargers;

public:
    // Only initialization state is the number of charging slots
    ChargingStation(int numberOfChargingSlots);

    // Disable unneeded defaults
    ChargingStation(double) = delete;
    ChargingStation(const ChargingStation&) = delete;
    ChargingStation& operator=(const ChargingStation&) = delete;

    // Add a vehicle to the wait queue
    void addVehicle(shared_ptr<IChargeable> chargeable);

    // Iterate manages the charging slots and wait queue
    void iterate();
};

#endif //_CHARGING_STATION_H