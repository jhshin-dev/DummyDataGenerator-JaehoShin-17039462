#pragma once
#include <string>
#include <vector>

enum class OrderStatus { RESERVED, REJECTED, PRODUCING, CONFIRMED, RELEASED };
enum class LineStatus  { IDLE, RUNNING };

inline std::string toString(OrderStatus s) {
    switch (s) {
        case OrderStatus::RESERVED:  return "RESERVED";
        case OrderStatus::REJECTED:  return "REJECTED";
        case OrderStatus::PRODUCING: return "PRODUCING";
        case OrderStatus::CONFIRMED: return "CONFIRMED";
        case OrderStatus::RELEASED:  return "RELEASED";
    }
    return "";
}

inline std::string toString(LineStatus s) {
    return s == LineStatus::IDLE ? "IDLE" : "RUNNING";
}

struct Sample {
    std::string sampleId;
    std::string name;
    int stock;
};

struct Order {
    std::string orderId;
    std::string customerId;
    std::string sampleId;
    int quantity;
    OrderStatus status;
};

struct ProductionLine {
    std::string lineId;
    std::string assignedOrderId; // empty string = null
    LineStatus status;
};

struct ScenarioData {
    std::string scenarioName;
    std::vector<Sample> samples;
    std::vector<Order> orders;
    std::vector<ProductionLine> productionLines;
};
