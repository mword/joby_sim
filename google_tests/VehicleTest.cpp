//
// Created by Mike Slo on 8/7/22.
//
#include <gtest/gtest.h>
#include <Vehicle.h>

// Test the iterate method
TEST(VehicleTest, iterate) {
    auto testModel = shared_ptr<ModelData>(new ModelData({{"Test", 22.0, 33.0, 44.0, 55.0, 3, 0.66, 77.0}, 5, 7, 11, 13}));
    Vehicle testVehicle(testModel);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::en_route);

    testVehicle.iterate(1.0);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::en_route);

    testVehicle.iterate(75.9);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::en_route);

    testVehicle.iterate(0.1);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::waiting_for_charge_queue);

    testVehicle.setCharging();
    EXPECT_EQ(testVehicle.getOpState(), OperationState::charging);

    testVehicle.iterate(43.9);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::charging);

    testVehicle.iterate(0.1);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::en_route);
}

// Test the setWaitQueue method
TEST(VehicleTest, setWaitQueue) {
    auto testModel = shared_ptr<ModelData>(
            new ModelData({{"Test", 22.0, 33.0, 44.0, 55.0, 3, 0.66, 77.0}, 5, 7, 11, 13}));
    Vehicle testVehicle(testModel);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::en_route);

    testVehicle.setInWaitQueue();
    EXPECT_EQ(testVehicle.getOpState(), OperationState::in_charge_queue);
}

// Test the setCharging & isCharging methods
TEST(VehicleTest, setCharging) {
    auto testModel = shared_ptr<ModelData>(
            new ModelData({{"Test", 22.0, 33.0, 44.0, 55.0, 3, 0.66, 77.0}, 5, 7, 11, 13}));
    Vehicle testVehicle(testModel);
    EXPECT_EQ(testVehicle.getOpState(), OperationState::en_route);

    testVehicle.setCharging();
    EXPECT_EQ(testVehicle.getOpState(), OperationState::charging);
    EXPECT_TRUE(testVehicle.isCharging());
}