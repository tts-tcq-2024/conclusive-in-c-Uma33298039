#include "gtest/gtest.h"
#include "AlertInEmail.h"
#include "TemperatureAlert_checker.h"
#include "breach_detector.h"
#include "alert_to_controller.h"
#include "cooling_config.h"
#include <string>
#include <sstream>

// Helper class to capture printf output
class PrintfCapture {
public:
    PrintfCapture() {
        old_stdout = std::cout.rdbuf(buffer.rdbuf());
    }

    ~PrintfCapture() {
        std::cout.rdbuf(old_stdout);
    }

    std::string str() const {
        return buffer.str();
    }

private:
    std::ostringstream buffer;
    std::streambuf* old_stdout;
};

// Test for send_to_email
TEST(AlertInEmail, TestSendToEmailLowBreach) {
    PrintfCapture capture;
    send_to_email(LOW_BREACH);
    std::string expected = "To: a.b@c.com\nHi, the temperature is too low\n";
    EXPECT_EQ(capture.str(), expected);
}

TEST(AlertInEmail, TestSendToEmailHighBreach) {
    PrintfCapture capture;
    send_to_email(HIGH_BREACH);
    std::string expected = "To: a.b@c.com\nHi, the temperature is too high\n";
    EXPECT_EQ(capture.str(), expected);
}

TEST(AlertInEmail, TestSendToEmailNormal) {
    PrintfCapture capture;
    send_to_email(NORMAL);
    std::string expected = "To: a.b@c.com\n";
    EXPECT_EQ(capture.str(), expected);
}

// Test for send_to_controller
TEST(AlertToController, TestSendToController) {
    PrintfCapture capture;
    send_to_controller(TOO_LOW);
    std::string expected = "feed : 1\n"; // Assuming breach_type TOO_LOW == 1
    EXPECT_EQ(capture.str(), expected);
}

// Test for breach_detector
TEST(BreachDetector, TestInferBreachTooLow) {
    EXPECT_EQ(infer_breach(10, 20, 30), TOO_LOW);
}

TEST(BreachDetector, TestInferBreachTooHigh) {
    EXPECT_EQ(infer_breach(40, 20, 30), TOO_HIGH);
}

TEST(BreachDetector, TestInferBreachNormal) {
    EXPECT_EQ(infer_breach(25, 20, 30), NORMAL);
}

TEST(BreachDetector, TestClassifyTemperatureBreach) {
    EXPECT_EQ(classify_temperature_breach(PASSIVE_COOLING, 50), TOO_HIGH);
    EXPECT_EQ(classify_temperature_breach(HI_ACTIVE_COOLING, -5), TOO_LOW);
    EXPECT_EQ(classify_temperature_breach(MED_ACTIVE_COOLING, 20), NORMAL);
}

// Test for check_and_alert
TEST(TemperatureAlertChecker, TestCheckAndAlertToController) {
    PrintfCapture capture;
    battery_character battery = {PASSIVE_COOLING, "Battery 1"};
    check_and_alert(TO_CONTROLLER, battery, 50);
    std::string expected = "feed : 2\n"; // Assuming breach_type TOO_HIGH == 2
    EXPECT_EQ(capture.str(), expected);
}

TEST(TemperatureAlertChecker, TestCheckAndAlertToEmail) {
    PrintfCapture capture;
    battery_character battery = {HI_ACTIVE_COOLING, "Battery 2"};
    check_and_alert(TO_EMAIL, battery, -5);
    std::string expected = "To: a.b@c.com\nHi, the temperature is too low\n";
    EXPECT_EQ(capture.str(), expected);
}

// Test for cooling_config
TEST(CoolingConfig, TestGetCoolingConfig) {
    cooling_config config = get_cooling_config(PASSIVE_COOLING);
    EXPECT_EQ(config.lower_limit, PASSIVE_COOLING_LOWER_LIMIT);
    EXPECT_EQ(config.upper_limit, PASSIVE_COOLING_UPPER_LIMIT);
    
    config = get_cooling_config(HI_ACTIVE_COOLING);
    EXPECT_EQ(config.lower_limit, HI_ACTIVE_COOLING_LOWER_LIMIT);
    EXPECT_EQ(config.upper_limit, HI_ACTIVE_COOLING_UPPER_LIMIT);
    
    config = get_cooling_config(MED_ACTIVE_COOLING);
    EXPECT_EQ(config.lower_limit, MED_ACTIVE_COOLING_LOWER_LIMIT);
    EXPECT_EQ(config.upper_limit, MED_ACTIVE_COOLING_UPPER_LIMIT);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
