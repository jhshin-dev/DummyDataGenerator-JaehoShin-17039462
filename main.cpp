#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include "scenarios.h"
#include "json_writer.h"

namespace fs = std::filesystem;

struct WriteResult {
    std::string filename;
    size_t sampleCount;
    size_t orderCount;
    size_t lineCount;
    bool success;
};

static WriteResult writeScenario(const ScenarioData& data, const fs::path& dir) {
    std::string filename = "scenario_" + data.scenarioName + ".json";
    WriteResult result{ filename, data.samples.size(), data.orders.size(), data.productionLines.size(), false };

    std::ofstream file(dir / filename);
    if (!file.is_open()) return result;

    file << json::serialize(data);
    result.success = file.good();
    return result;
}

int main(int argc, char* argv[]) {
    fs::path outDir = (argc > 1) ? fs::path(argv[1]) : fs::current_path();

    if (!fs::exists(outDir)) {
        std::cerr << "Error: output directory does not exist: " << outDir << "\n";
        return 1;
    }

    const std::vector<ScenarioData> scenarios = {
        makeScenarioA(),
        makeScenarioB(),
        makeScenarioC(),
        makeScenarioD(),
    };

    std::cout << "DummyDataGenerator — S-Semi PoC\n";
    std::cout << "Output: " << outDir << "\n\n";

    bool anyFailed = false;
    for (const auto& scenario : scenarios) {
        auto r = writeScenario(scenario, outDir);
        if (r.success) {
            std::cout << "[OK]   " << r.filename
                      << "  (samples=" << r.sampleCount
                      << ", orders=" << r.orderCount
                      << ", lines=" << r.lineCount << ")\n";
        } else {
            std::cerr << "[FAIL] " << r.filename << "\n";
            anyFailed = true;
        }
    }

    return anyFailed ? 1 : 0;
}
