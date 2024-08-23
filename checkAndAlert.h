#pragma once

#include "typewise-alert.h"
#include "classifyTemperatureBreach.h"
#include "alertBasedOnTarget.h"

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);
