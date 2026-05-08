#pragma once
#include "models.h"

ScenarioData makeScenarioA(); // stock sufficient: CONFIRMED / RELEASED
ScenarioData makeScenarioB(); // stock shortage:   PRODUCING
ScenarioData makeScenarioC(); // order rejected:   REJECTED
ScenarioData makeScenarioD(); // mixed states (dashboard/monitoring view)
