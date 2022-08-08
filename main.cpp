#include <iostream>
#include <vector>
#include <map>
#include <random>

#include "./Vehicle.h"
#include "./ChargingStation.h"

using std::vector;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;

// Vehicle model IDs
enum ModelID {
    alpha,
    beta,
    charlie,
    delta,
    echo,
    count // For tracking the number of models
};

// Convent typedef
typedef std::map<ModelID, shared_ptr<ModelState>> VehicleModelMap;

// Get system time as a double
double sys_time() {
    struct timespec time_spec;
    assert(clock_gettime(CLOCK_MONOTONIC_RAW, &time_spec) == 0);
    double sys_time = time_spec.tv_sec + (time_spec.tv_nsec * 1e-9);
    return sys_time;
}

// Function to initialize the various Vehicle "Models" in the simulation. In real life this could be data driven
// based on the contents of setup files or something.
void  initialize_models(VehicleModelMap& vehicleModels) {
    // Define the specific Models (this could potentially be made data-driven)
    // Alpha
    shared_ptr<ModelState> alphaState(new ModelState {
        {
            "Alpha",
            120.0,       // Speed (mph)
            320.0,       // Battery Capacity (kWh)
            0.6 * 60.0,  // Time to Charge (minutes)
            1.6,         // Energy use at Cruse (kwh/mile)
            4,           // Passenger Count
            0.25,        // Fault probability / hour
            (320.0 * 60.0) / (120.0 * 1.6) // Flight endurance (minutes), I could calculate this every time.
        },
        0, // Number in Fleet
        0, // Accumulated Fleet Flight Time
        0, // Accumulated Fleet Charging Time
        0, // Accumulated Fleet Wait Time
    });
    vehicleModels[ModelID::alpha] = alphaState;

    // Beta
    shared_ptr<ModelState> betaState(new ModelState {
        {
            "Beta",
            100.0,       // Speed (mph)
            100.0,       // Battery Capacity (kWh)
            0.2 * 60.0,  // Time to Charge (minutes)
            1.5,         // Energy use at Cruse (kwh/mile)
            5,           // Passenger Count
            0.1,         // Fault probability / hour
            (100.0 * 60.0) / (100.0 * 1.5) // Flight endurance (minutes), I could calculate this every time.
        },
        0, // Number in Fleet
        0, // Accumulated Fleet Flight Time
        0, // Accumulated Fleet Charging Time
        0, // Accumulated Fleet Wait Time
    });
    vehicleModels[ModelID::beta] = betaState;

    // Charlie
    shared_ptr<ModelState> charlieState(new ModelState {
        {
            "Charlie",
            160.0,       // Speed (mph)
            220.0,       // Battery Capacity (kWh)
            0.8 * 60.0,  // Time to Charge (minutes)
            2.2,         // Energy use at Cruse (kwh/mile)
            3,           // Passenger Count
            0.05,        // Fault probability / hour
            (220.0 * 60.0) / (160.0 * 2.2) // Flight endurance (minutes), I could calculate this every time.
        },
        0, // Number in Fleet
        0, // Accumulated Fleet Flight Time
        0, // Accumulated Fleet Charging Time
        0, // Accumulated Fleet Wait Time
    });
    vehicleModels[ModelID::charlie] = charlieState;

    // Delta
    shared_ptr<ModelState> deltaState(new ModelState {
        {
            "Delta",
            90.0,       // Speed (mph)
            120.0,       // Battery Capacity (kWh)
            0.62 * 60.0,  // Time to Charge (minutes)
            0.8,         // Energy use at Cruse (kwh/mile)
            2,           // Passenger Count
            0.22,        // Fault probability / hour
            (120.0 * 60.0) / (120.0 * 1.6) // Flight endurance (minutes), I could calculate this every time.
        },
        0, // Number in Fleet
        0, // Accumulated Fleet Flight Time
        0, // Accumulated Fleet Charging Time
        0, // Accumulated Fleet Wait Time
    });
    vehicleModels[ModelID::delta] = deltaState;

    // Echo
    shared_ptr<ModelState> echoState(new ModelState {
        {
            "Echo",
            30.0,        // Speed (mph)
            150.0,       // Battery Capacity (kWh)
            0.3 * 60.0,  // Time to Charge (minutes)
            5.8,         // Energy use at Cruse (kwh/mile)
            2,           // Passenger Count
            0.61,        // Fault probability / hour
            (150.0 * 60.0) / (30.0 * 5.8) // Flight endurance (minutes), I could calculate this every time.
        }, // Number in Fleet
        0, // Number in Fleet
        0, // Accumulated Fleet Flight Time
        0, // Accumulated Fleet Charging Time
        0, // Accumulated Fleet Wait Time
    });
    vehicleModels[ModelID::echo] = echoState;
}

// Runs the actual simulation
// In this incarnation the simulation is a single threaded loop through a vector
// of randomly select vehicle models.
//
// Params:
//   useRealTime - If true, use the system clock and each system second is 1 sim minutes.
//                 If false, each pass through the main loop increments world time by 0.6 seconds.
//   runTime     - Number of world time minutes to run for
void run_simulation(bool useRealTime, double runTime) {
    // We're going to want some randomness
    std::random_device rd;
    std::default_random_engine rng(rd());

    // Create a map from Model ModelID to ModelState records
    VehicleModelMap modelStates;
    initialize_models(modelStates);

    // Generate the Fleet
    // Create a random assortment of vehicles and store in a vector
    std::uniform_int_distribution<unsigned int> dist(0, ModelID::count-1);
    vector<shared_ptr<Vehicle>> vehicles;
    for (unsigned int vehicleCount=0; vehicleCount < 20;) {
        ModelID nextModel = static_cast<ModelID>(dist(rng));

        // If there's an entry for the model in the model map add a new vehicle to the fleet
        if (modelStates.count(nextModel) != 0) { // Just making sure there's a entry for this model
            shared_ptr<ModelState> model = modelStates[nextModel];
            vehicles.push_back(shared_ptr<Vehicle>(new Vehicle(model)));
            ++(model->fleetCount);
            ++vehicleCount;
        }
    }

    // Generate the charging station
    // Assumption: There is only one charging location that all vehicles share after every trip.
    ChargingStation chargingStation(3);

    // Simulation loop:
    double increment = 0.01; // The value only maters if we're not using the system clock
    double currentTime = 0.0; // Start currentTime delta
    double now = sys_time(); // Only used when using the real time clock;
    double startTime = now;

    unsigned long totalIterations = 0; // Just here for debugging
    // Main simulation loop, we're done when it is over.
    while (currentTime < runTime) {
        if (useRealTime) {
            // We're using the system clock to calculate the next increment
            double lastTime = now;
            now = sys_time();
            increment = now - lastTime;
        }

        // Shuffle the vector of vehicle so we're not always incrementing them in the same order.
        std::shuffle(vehicles.begin(), vehicles.end(), rng);

        // Iterate through vehicles.
        // This could potentially change the vehicle's state from
        // "en_route" -> "waiting_for_charge_queue" -> "in_charge_queue",
        // or "charging" -> "en_route".
        for(shared_ptr<Vehicle> &v : vehicles) {
            v->iterate(increment);
            // Vehicles don't know about the charging station and the charging station doesn't know
            // about vehicles it's not yet managing (just exited "en_route" state).
            if (v->isWaitingForQueue()) {
                // Tell the charging station about the vehicle - which changes vehicle state from
                // "waiting_for_charge_queue" to "in_charge_queue"
                chargingStation.addVehicle(v);
            }
        }

        // Iterate the chargingStation which will fill charging slots with queued vehicles if there are
        // any empty slots and queued vehicles.
        //
        // This could potentially change some vehicle states from
        // "in_charge_queue" to "charging"
        chargingStation.iterate();

        // Increment the currentTime for the next iteration.
        currentTime += increment;

        // For debugging
        ++totalIterations;
    }
    // Print some run data
    double totalRunTime = sys_time() - startTime;
    cout << "Simulation finished, total-run-time(seconds)/total-iterations: " << totalRunTime << "/" << totalIterations << endl << endl;

    // Done with the simulation, now print out results.
    // Print vehicle stats
    cout << "Per Vehicle Stats:" << endl;
    for (shared_ptr<Vehicle> v: vehicles) {
        v->printResult();
    }

    // Print model stats
    cout << endl << "Per Model Stats:" << endl;
    for(auto model: modelStates) {
        model.second->prinResult();
    }

}


int main() {
    double runTime = 60.0 * 3.0;
    bool useSysClock = true;
    run_simulation(useSysClock, runTime);
    return 0;
}