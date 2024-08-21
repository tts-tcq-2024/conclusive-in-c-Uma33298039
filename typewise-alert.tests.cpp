#include <gtest/gtest.h>
#include "typewise-alert.h"

// Test inferBreach
TEST(TypewiseAlertTest, InferBreachTooLow) {
    EXPECT_EQ(inferBreach(10, 20, 30), TOO_LOW);
}

TEST(TypewiseAlertTest, InferBreachTooHigh) {
    EXPECT_EQ(inferBreach(40, 20, 30), TOO_HIGH);
}

TEST(TypewiseAlertTest, InferBreachNormal) {
    EXPECT_EQ(inferBreach(25, 20, 30), NORMAL);
}

// Test getCoolingLimits
TEST(TypewiseAlertTest, GetCoolingLimitsPassiveCooling) {
    int lowerLimit, upperLimit;
    getCoolingLimits(PASSIVE_COOLING, &lowerLimit, &upperLimit);
    EXPECT_EQ(lowerLimit, 0);
    EXPECT_EQ(upperLimit, 35);
}

TEST(TypewiseAlertTest, GetCoolingLimitsHiActiveCooling) {
    int lowerLimit, upperLimit;
    getCoolingLimits(HI_ACTIVE_COOLING, &lowerLimit, &upperLimit);
    EXPECT_EQ(lowerLimit, 0);
    EXPECT_EQ(upperLimit, 45);
}

TEST(TypewiseAlertTest, GetCoolingLimitsMedActiveCooling) {
    int lowerLimit, upperLimit;
    getCoolingLimits(MED_ACTIVE_COOLING, &lowerLimit, &upperLimit);
    EXPECT_EQ(lowerLimit, 0);
    EXPECT_EQ(upperLimit, 40);
}

// Test classifyTemperatureBreach
TEST(TypewiseAlertTest, ClassifyTemperatureBreachTooLow) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -5), TOO_LOW);
}

TEST(TypewiseAlertTest, ClassifyTemperatureBreachTooHigh) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 40), TOO_HIGH);
}

TEST(TypewiseAlertTest, ClassifyTemperatureBreachNormal) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 25), NORMAL);
}

TEST(TypewiseAlertTest, ClassifyTemperatureBreachHiActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
}

TEST(TypewiseAlertTest, ClassifyTemperatureBreachMedActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
}

// Mocked print function for testing sendToController and sendToEmail
class PrintMock : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect stdout to a stringstream
        oldCoutBuf = std::cout.rdbuf();
        std::cout.rdbuf(ss.rdbuf());
    }

    void TearDown() override {
        // Reset stdout
        std::cout.rdbuf(oldCoutBuf);
    }

    std::stringstream ss;
    std::streambuf* oldCoutBuf;
};

TEST_F(PrintMock, SendToControllerTest) {
    sendToController(TOO_LOW);
    EXPECT_EQ(ss.str(), "feed : 1\n");
}

TEST_F(PrintMock, SendToEmailTooLowTest) {
    sendToEmail(TOO_LOW);
    std::string expected = "To: a.b@c.com\nHi, the temperature is too low\n";
    EXPECT_EQ(ss.str(), expected);
}

TEST_F(PrintMock, SendToEmailTooHighTest) {
    sendToEmail(TOO_HIGH);
    std::string expected = "To: a.b@c.com\nHi, the temperature is too high\n";
    EXPECT_EQ(ss.str(), expected);
}

// Test checkAndAlert
TEST_F(PrintMock, CheckAndAlertToControllerNormal) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_CONTROLLER, batteryChar, 25);
    EXPECT_EQ(ss.str(), "feed : 0\n");
}

TEST_F(PrintMock, CheckAndAlertToEmailTooLow) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_EMAIL, batteryChar, -5);
    std::string expected = "To: a.b@c.com\nHi, the temperature is too low\n";
    EXPECT_EQ(ss.str(), expected);
}

TEST_F(PrintMock, CheckAndAlertToEmailTooHigh) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    checkAndAlert(TO_EMAIL, batteryChar, 40);
    std::string expected = "To: a.b@c.com\nHi, the temperature is too high\n";
    EXPECT_EQ(ss.str(), expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
