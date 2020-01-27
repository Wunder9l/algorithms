#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <fstream>
#include "annealing.h"

struct City {
    int id;
    double x;
    double y;

    double Distance(City city) const {
        return sqrt((x - city.x) * (x - city.x) + (y - city.y) * (y - city.y));
    }
};

class TravelingSalesmanProblemState {
public:
    std::vector<City> Cities;
};

class TravelingSalesmanProblem : public IAnnealingSimulator<TravelingSalesmanProblemState> {
    double StartTemperature;
public:
    TravelingSalesmanProblem(double startTemperature) {
        StartTemperature = startTemperature;
    }

    double EnergyCallback(const TravelingSalesmanProblemState &state) override {
        double sumDistance = 0;
        for (size_t i = 1; i < state.Cities.size(); ++i) {
            sumDistance += state.Cities[i].Distance(state.Cities[i - 1]);
        }
        if (state.Cities.size() > 1) {
            sumDistance += state.Cities.front().Distance(state.Cities.back());
        }
        return sumDistance;
    }

    TravelingSalesmanProblemState NextStateCallback(const TravelingSalesmanProblemState &state) override {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, state.Cities.size() - 1);
        auto left = dis(gen), right = dis(gen);
        if (right < left) {
            std::tie(left, right) = std::tie(right, left);
        } else if (right == left) {
            if (left != 0) {
                left--;
            } else {
                right++;
            }
        }
//        std::cout << left << ' ' << right << std::endl;
        TravelingSalesmanProblemState nextState = state;
        while (left < right) {
            std::swap(nextState.Cities[left], nextState.Cities[right]);
            ++left;
            --right;
        }
        return nextState;
    }

    double TemperatureCallback(double curTemperature, uint64_t iteration) override {
        return StartTemperature / iteration;
    }
};

class DebugClass {
    std::ostringstream output;
public:
    void AddLogEntry(uint64_t i, const TravelingSalesmanProblemState &state, double energy, double temperature) {
        output << i << ' ' << energy << ' ' << temperature;
        for (auto c: state.Cities) {
            output << ' ' << c.x << ' ' << c.y;
        }
        output << std::endl;
    }

    bool WriteToFile(const std::string &filename) {
        std::ofstream fout(filename);
        fout << output.str();
    }
};

int main() {
    TravelingSalesmanProblemState startState;
    int n = 100;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> dist(0.0, 10.0);
    for (int i = 0; i < n; ++i) {
        startState.Cities.push_back(City{.id=i, .x = dist(gen), .y = dist(gen)});
    }

    double startTemp = 10.0;
    auto simulator = TravelingSalesmanProblem(startTemp);
    auto annealing = Annealing<TravelingSalesmanProblemState>(simulator);
    DebugClass debug;
    std::function<void(uint64_t, const TravelingSalesmanProblemState &, double, double)> callback =
            [&debug](uint64_t i, const TravelingSalesmanProblemState &state, double energy, double temperature) {
                debug.AddLogEntry(i, state, energy, temperature);
            };
    annealing.Run(startTemp, 0.0001, startState, 25000, &callback);

    debug.WriteToFile("out.txt");
}