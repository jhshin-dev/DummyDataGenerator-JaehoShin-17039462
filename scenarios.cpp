#include "scenarios.h"

ScenarioData makeScenarioA() {
    ScenarioData d;
    d.scenarioName = "confirmed";

    d.samples = {
        { "SMPL-001", "NPN Transistor A", 50 },
        { "SMPL-002", "PMOS FET B",       30 },
    };

    d.orders = {
        { "ORD-001", "CUST-001", "SMPL-001", 10, OrderStatus::CONFIRMED },
        { "ORD-002", "CUST-002", "SMPL-002",  5, OrderStatus::RELEASED  },
    };

    d.productionLines = {
        { "LINE-001", "", LineStatus::IDLE },
        { "LINE-002", "", LineStatus::IDLE },
    };

    return d;
}

ScenarioData makeScenarioB() {
    ScenarioData d;
    d.scenarioName = "producing";

    d.samples = {
        { "SMPL-001", "NPN Transistor A", 3 },
    };

    d.orders = {
        { "ORD-001", "CUST-001", "SMPL-001", 20, OrderStatus::PRODUCING },
    };

    d.productionLines = {
        { "LINE-001", "ORD-001", LineStatus::RUNNING },
    };

    return d;
}

ScenarioData makeScenarioC() {
    ScenarioData d;
    d.scenarioName = "rejected";

    d.samples = {
        { "SMPL-001", "NPN Transistor A", 50 },
    };

    d.orders = {
        { "ORD-001", "CUST-003", "SMPL-001", 5, OrderStatus::REJECTED },
    };

    d.productionLines = {
        { "LINE-001", "", LineStatus::IDLE },
    };

    return d;
}

ScenarioData makeScenarioD() {
    ScenarioData d;
    d.scenarioName = "mixed";

    d.samples = {
        { "SMPL-001", "NPN Transistor A",  30 },
        { "SMPL-002", "PMOS FET B",         5 },
        { "SMPL-003", "Zener Diode C",    100 },
    };

    d.orders = {
        { "ORD-001", "CUST-001", "SMPL-001", 10, OrderStatus::RELEASED  },
        { "ORD-002", "CUST-002", "SMPL-002", 20, OrderStatus::PRODUCING },
        { "ORD-003", "CUST-003", "SMPL-003",  5, OrderStatus::CONFIRMED },
        { "ORD-004", "CUST-001", "SMPL-001", 15, OrderStatus::RESERVED  },
        { "ORD-005", "CUST-004", "SMPL-002",  3, OrderStatus::REJECTED  },
    };

    d.productionLines = {
        { "LINE-001", "ORD-002", LineStatus::RUNNING },
        { "LINE-002", "",        LineStatus::IDLE    },
    };

    return d;
}
