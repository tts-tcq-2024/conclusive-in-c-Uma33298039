#include "generateEmailMessage.h"

const char* generateEmailMessage(BreachType breachType) {
  switch (breachType) {
    case TOO_LOW:
      return "Hi, the temperature is too low";
    case TOO_HIGH:
      return "Hi, the temperature is too high";
    default:
      return NULL; // No message for NORMAL
  }
}
