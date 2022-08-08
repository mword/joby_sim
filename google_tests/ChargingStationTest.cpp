//
// Created by Mike Slo on 8/7/22.
//

//
// Created by Mike Slo on 8/7/22.
//
#include <gtest/gtest.h>

#include <Vehicle.h>
#include <ChargingStation.h>

// Test the addVehicle & iterate methods
TEST(ChargingStationTest, add_Vehicle_And_Iterate) {
    // The charging station to test
    ChargingStation chargingStation(1);

    // A vehicle to test
    auto testModel = shared_ptr<ModelData>(new ModelData({{"Test", 22.0, 33.0, 44.0, 55.0, 3, 0.66, 77.0}, 5, 7, 11, 13}));
    auto testVehicle = shared_ptr<Vehicle>(new Vehicle(testModel));
    EXPECT_EQ(testVehicle->getOpState(), OperationState::en_route);

    testVehicle->iterate(77.0);
    EXPECT_EQ(testVehicle->getOpState(), OperationState::waiting_for_charge_queue);

    chargingStation.addVehicle(testVehicle);
    EXPECT_EQ(testVehicle->getOpState(), OperationState::in_charge_queue);

    chargingStation.iterate();
    EXPECT_EQ(testVehicle->getOpState(), OperationState::charging);
}