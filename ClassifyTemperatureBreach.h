#pragma once

#include "typewise-alert.h"
#include "getCoolingLimits.h"
#include "inferBreach.h"

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);
