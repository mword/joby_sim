#ifndef _VEHICLE_H
#define _VEHICLE_H

#include "./IChargeable.h"

using std::shared_ptr;

// Operation States
enum OperationState {
    en_route,
    waiting_for_charge_queue,
    in_charge_queue,
    charging,
};

// These structures store Vehicle Model date. Vehicles of the same model
// share a single Model Record.
// I did this (as opposed to subclassing a vehicle for each particular model) so class changes wouldn't be
// necessary to add or remove Models from the flying population.

struct ModelParameters {
    const std::string label;
    const double cruiseSpeed;           // mph
    const double batteryCapacity;       // kWh
    const double timeToCharge;          // minutes
    const double energyUseAtCruise;     // kWh/mile
    const unsigned int passengerCount;  // n
    const double MaxFaultsPerHour;      // faults/hour (I'm assume this means per flight hour?)
    const double endurance;             // minutes/trip
};

struct ModelData {
    const ModelParameters params;
    unsigned int fleetCount;
    double totalFlightTime;
    double totalChargingTime;
    double totalWaitingTime;

    void prinResult();
};

// Class to track & iterate vehicle state
// Subclass IChargeable so chargers & vehicles don't have to know about each other. Chargers only know about
// the IChargeable interface calls.
class Vehicle : public IChargeable {

private:

    // Shared model parameters and totals - this structure is shared
    // by all vehicles of the same model
    shared_ptr<ModelData> modelState;

    // Current operation mode
    OperationState operationState;
    // Duration of current operation mode
    double currentOperationTime;

    // Per Vehicle totals
    // Times are in simulated minutes;
    double totalTimeEnRoute;
    double totalTimeCharging;
    double totalTimeWaiting;

public:
    // Only need one constructor - we're not doing any copying
    // Model data is a structure shared amongst vehicle of a common type.
    Vehicle(shared_ptr<ModelData> modelData);

    // Prevent unneeded defaults
    Vehicle(const Vehicle&) = delete;
    Vehicle& operator=(const Vehicle&) = delete;

    ~Vehicle();

    // Iterate by delta-time, units are simulated minutes
    void iterate(double deltaT);

    // Print out various results;
    void printResult() const;

    // Accessor for operation state - this isn't used except testing
    // as operation state is a internal detail.
    OperationState getOpState() const;

    // From IChargeable for ChargingStation interaction
    void setCharging() override;
    bool isCharging () const override;
    bool isWaitingForQueue() const override;
    void setInWaitQueue() override;

};

#endif //_VEHICLE_H