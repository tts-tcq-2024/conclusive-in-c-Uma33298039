#include "alertBasedOnTarget.h"

void alertBasedOnTarget(AlertTarget alertTarget, BreachType breachType) {
  switch (alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}
