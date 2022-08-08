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

// These structures store Vehicle Model state. Vehicles of the same model
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

struct ModelState {
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

    // Model Results
    shared_ptr<ModelState> modelState;

    // State
    OperationState operationState;
    double currentOperationTime;

    // Per Vehicle totals
    // Times are in simulated minutes;
    double totalTimeEnRoute;
    double totalTimeCharging;
    double totalTimeWaiting;

public:
    // Only need one constructor - we're not doing any copying
    Vehicle(shared_ptr<ModelState> results);

    // Prevent unneeded defaults
    Vehicle(const Vehicle&) = delete;
    Vehicle& operator=(const Vehicle&) = delete;

    ~Vehicle();

    // Iterate by delta-time, units are simulated minutes
    void iterate(double deltaT);

    // Print out various results;
    void printResult() const;

    // Accessor for operation state which should be of internal
    // interest only - so just used for tests
    OperationState getOpState() const;

    // From IChargeable
    void setCharging() override;
    bool isCharging () const override;
    bool isWaitingForQueue() const override;
    void setInWaitQueue() override;

private:


};


// I like the idea of having a type per vehicle model which allows me to define a set of statics for each model
// to store model parameters and collect model runtime state. The problem is that this makes the code less flexible
// because the Models would need to be defined and initialized at compile time and adding a new model would require
// a recompile.  In any case, it would look something like this.
//template<ModelID TID>
//class Model: public Vehicle {
//private:
//    ModelID typeId;
//
//public:
//    Model() :
//          typeId(TID),
//          Vehicle(...){}
//
//};
#endif //_VEHICLE_H