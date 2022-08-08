#include "./ChargingStation.h"

// Constructor
ChargingStation::ChargingStation(int numberOfChargingSlots) : cNumberOfChargingSlots(numberOfChargingSlots){}

// Add an IChargeable to the waitQueue
void ChargingStation::addVehicle(shared_ptr<IChargeable> chargeable) {
    waitQueue.push(chargeable);
    chargeable->setInWaitQueue();
}

// Process the charge slots and wait queue
void ChargingStation::iterate() {
    // Check who's done charging and remove them
    for (std::set<shared_ptr<IChargeable>>::iterator it = chargers.begin(); it != chargers.end();) {
        if (!(*it)->isCharging()) {
            it = chargers.erase(it); // No longer charging so remove from charging station
        } else {
            ++it;
        }
    }

    // Add vehicles to the charger if possible
    while (!waitQueue.empty() && chargers.size() < cNumberOfChargingSlots) {
        shared_ptr<IChargeable> v = waitQueue.front();
        waitQueue.pop();
        chargers.insert(v);
        v->setCharging();
    }
}


