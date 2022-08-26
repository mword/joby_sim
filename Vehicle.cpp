#include <iostream>

#include "./Vehicle.h"

using std::cout;
using std::endl;

Vehicle::Vehicle(shared_ptr<ModelData> modelData) :
        modelState(modelData),
        operationState(en_route),
        currentOperationTime(0.0),
        totalTimeEnRoute(0.0),
        totalTimeCharging(0.0),
        totalTimeWaiting(0.0)
{
};

Vehicle::~Vehicle() {
    // Nothing to do
};

// Implementation of ICharging interface
void Vehicle::setCharging() {
    operationState = OperationState::charging;
    currentOperationTime = 0;
}

bool Vehicle::isCharging () const {
    return operationState == OperationState::charging;
}

bool Vehicle::isWaitingForQueue() const {
    return operationState == OperationState::waiting_for_charge_queue;
}

void Vehicle::setInWaitQueue() {
    operationState = OperationState::in_charge_queue;
}

OperationState Vehicle::getOpState() const {
    return operationState;
}

// Iterate by delta-time in seconds (simulated minutes)
void Vehicle::iterate(double deltaT) {
    double newOperationTime = currentOperationTime + deltaT;
    switch (operationState) {
        case en_route: {
            if (newOperationTime >= modelState->params.endurance) {
                operationState = OperationState::waiting_for_charge_queue; //This is an intermediate state
                // Reset operation time - there may be an overrun
                currentOperationTime = 0;
                if (newOperationTime > modelState->params.endurance) {
                    currentOperationTime = newOperationTime - modelState->params.endurance;
                }
            } else {
                currentOperationTime = newOperationTime;
                totalTimeEnRoute += deltaT;   // Total for vehicle (sim minutes)
                modelState->totalFlightTime += deltaT; // Total for model (sim minutes)
            }
            break;
        };
        case waiting_for_charge_queue: {
            // This state should never be set at the beginning of an iteration as the
            // the vehicle should be moved to the wait queue before the iteration is over.
            assert(true);
            break;
        };
        case in_charge_queue: {
            // This could be a bit off (one deltaT) because the following charging station iterate()
            // might start charging this vehicle
            totalTimeWaiting += deltaT;  // Total for vehicle (sim minutes)
            modelState->totalWaitingTime += deltaT; // Total for model (sim minutes)
            break;
        };
        case charging: {
            if (newOperationTime >= modelState->params.timeToCharge) {
                operationState = OperationState::en_route;
                // Reset operation time - there may be an overrun
                currentOperationTime = 0;
                if (newOperationTime > modelState->params.timeToCharge) {
                    currentOperationTime = newOperationTime - modelState->params.timeToCharge;
                }
            } else {
                currentOperationTime = newOperationTime;
                totalTimeCharging += deltaT;   // Total for vehicle (sim minutes)
                modelState->totalChargingTime += deltaT; // Total for model (sim minutes)
            }
            break;
        };

    };
}

void Vehicle::printResult() const {
    cout << "Result: " << endl;
    cout << "  Model                      : " << modelState->params.label << endl;
    cout << "  Endurance                  : " << modelState->params.endurance << endl;
    cout << "  Total Time In Flight (mins): " << totalTimeEnRoute << endl;
    cout << "  Total Time Charging (mins) : " << totalTimeCharging << endl;
    cout << "  Total Time Waiting (mins)  : " << totalTimeWaiting << endl;
    cout << "  Total Passenger Miles      : " << (totalTimeEnRoute / 60.0) * modelState->params.cruiseSpeed * modelState->params.passengerCount << endl;
}

void ModelData::prinResult() {
    double totalPassengerMiles = params.cruiseSpeed * (totalFlightTime/60.0) *params.passengerCount;
    cout << params.label << " Results: " << endl;
    cout << "   Number of Vehicle            : " << fleetCount << endl;
    cout << "   Average Time in Flight (mins): " << totalFlightTime/fleetCount << endl;
    cout << "   Average Time Waiting (mins)  : " << totalWaitingTime/fleetCount << endl;
    cout << "   Average Time Charging (mins) : " << totalChargingTime/fleetCount << endl;
    cout << "   Max number of faults         : " << params.MaxFaultsPerHour * (totalFlightTime/60.0) << endl;
    cout << "   Total Passenger Miles        : " << totalPassengerMiles << endl;
    cout << "   Average Passenger Miles      : " << totalPassengerMiles/fleetCount << endl;
}
