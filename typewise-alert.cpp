#include "typewise-alert.h"
#include <stdio.h>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if (value < lowerLimit) {
    return TOO_LOW;
  }
  if (value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

void getCoolingLimits(CoolingType coolingType, int* lowerLimit, int* upperLimit) {
  switch(coolingType) {
    case PASSIVE_COOLING:
      *lowerLimit = 0;
      *upperLimit = 35;
      break;
    case HI_ACTIVE_COOLING:
      *lowerLimit = 0;
      *upperLimit = 45;
      break;
    case MED_ACTIVE_COOLING:
      *lowerLimit = 0;
      *upperLimit = 40;
      break;
  }
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  int lowerLimit = 0;
  int upperLimit = 0;
  getCoolingLimits(coolingType, &lowerLimit, &upperLimit);
  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

void sendAlert(AlertTarget alertTarget, BreachType breachType) {
  if (alertTarget == TO_CONTROLLER) {
    sendToController(breachType);
  } else if (alertTarget == TO_EMAIL) {
    sendToEmail(breachType);
  }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  sendAlert(alertTarget, breachType);
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  if (breachType == TOO_LOW) {
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is too low\n");
  } else if (breachType == TOO_HIGH) {
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is too high\n");
  }
}
