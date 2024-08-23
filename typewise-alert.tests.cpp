#include <gtest/gtest.h>
#include "breach_detector.h"
#include "alert_checker.h"
#include "message_sender_to_controller.h"
#include "message_sender_to_email.h"

TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    EXPECT_EQ(infer_breach(12, 20, 30), TOO_LOW);
    EXPECT_EQ(infer_breach(25, 20, 30), NORMAL);
    EXPECT_EQ(infer_breach(35, 20, 30), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachForCoolingTypes) {
    EXPECT_EQ(classify_temperature_breach(PASSIVE_COOLING, 36), TOO_HIGH);
    EXPECT_EQ(classify_temperature_breach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
    EXPECT_EQ(classify_temperature_breach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
    EXPECT_EQ(classify_temperature_breach(PASSIVE_COOLING, 20), NORMAL);
}

TEST(TypeWiseAlertTestSuite, SendsAlertToController) {
    testing::internal::CaptureStdout();
    send_to_controller(TOO_HIGH);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");
}

TEST(TypeWiseAlertTestSuite, SendsAlertToEmail) {
    testing::internal::CaptureStdout();
    send_to_email(TOO_LOW);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");

    testing::internal::CaptureStdout();
    send_to_email(TOO_HIGH);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToController) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    
    testing::internal::CaptureStdout();
    check_and_alert(TO_CONTROLLER, batteryChar, 36);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmail) {
    BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandY"};    
    testing::internal::CaptureStdout();
    check_and_alert(TO_EMAIL, batteryChar, 46);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}
