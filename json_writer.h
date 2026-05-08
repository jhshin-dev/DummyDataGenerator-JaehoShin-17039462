#pragma once
#include "models.h"
#include <string>
#include <vector>

namespace json {

inline std::string quoted(const std::string& s) {
    return "\"" + s + "\"";
}

inline std::string kv(const std::string& key, const std::string& val, int ind) {
    return std::string(ind, ' ') + quoted(key) + ": " + quoted(val);
}

inline std::string kv(const std::string& key, int val, int ind) {
    return std::string(ind, ' ') + quoted(key) + ": " + std::to_string(val);
}

inline std::string kvNull(const std::string& key, int ind) {
    return std::string(ind, ' ') + quoted(key) + ": null";
}

inline std::string serialize(const Sample& s, int ind) {
    std::string p(ind, ' ');
    return p + "{\n" +
        kv("sampleId", s.sampleId, ind + 2) + ",\n" +
        kv("name",     s.name,     ind + 2) + ",\n" +
        kv("stock",    s.stock,    ind + 2) + "\n" +
        p + "}";
}

inline std::string serialize(const Order& o, int ind) {
    std::string p(ind, ' ');
    return p + "{\n" +
        kv("orderId",    o.orderId,          ind + 2) + ",\n" +
        kv("customerId", o.customerId,       ind + 2) + ",\n" +
        kv("sampleId",   o.sampleId,         ind + 2) + ",\n" +
        kv("quantity",   o.quantity,         ind + 2) + ",\n" +
        kv("status",     toString(o.status), ind + 2) + "\n" +
        p + "}";
}

inline std::string serialize(const ProductionLine& l, int ind) {
    std::string p(ind, ' ');
    std::string assigned = l.assignedOrderId.empty()
        ? kvNull("assignedOrderId", ind + 2)
        : kv("assignedOrderId", l.assignedOrderId, ind + 2);
    return p + "{\n" +
        kv("lineId", l.lineId, ind + 2) + ",\n" +
        assigned + ",\n" +
        kv("status", toString(l.status), ind + 2) + "\n" +
        p + "}";
}

template <typename T>
inline std::string array(const std::vector<T>& items, int ind) {
    if (items.empty()) return "[]";
    std::string result = "[\n";
    for (size_t i = 0; i < items.size(); ++i) {
        result += serialize(items[i], ind + 2);
        if (i + 1 < items.size()) result += ",";
        result += "\n";
    }
    result += std::string(ind, ' ') + "]";
    return result;
}

inline std::string serialize(const ScenarioData& d) {
    return "{\n"
        "  \"scenario\": \"" + d.scenarioName + "\",\n"
        "  \"samples\": "          + array(d.samples,         2) + ",\n"
        "  \"orders\": "           + array(d.orders,          2) + ",\n"
        "  \"productionLines\": "  + array(d.productionLines, 2) + "\n"
        "}";
}

} // namespace json
