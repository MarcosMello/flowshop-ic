#include "../../bits/stdc++.h"

using namespace std;

inline auto defaultRandomEngine = default_random_engine(random_device()());

inline auto getRandomNumber(const size_t lowerBound, const size_t upperBound) {
    return uniform_int_distribution (lowerBound, upperBound) (defaultRandomEngine);
}

int jobs = 0;
int machines = 0;

bool distinctDeliveryDates = false;

void parseArguments(const int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (string argument = argv[i]; argument.find("-jobs=") == 0) {
            string strippedArgument = argument.substr(6);
            jobs = static_cast<int>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument.find("-machines=") == 0) {
            string strippedArgument = argument.substr(10);
            machines = static_cast<int>(strtol(strippedArgument.c_str(), nullptr, 10));
        } else if (argument == "-distinct") {
            distinctDeliveryDates = true;
        }
    }
}

int main(const int argc, char *argv[]) {
    parseArguments(argc, argv);

    ofstream outputFile(("n" + to_string(jobs) + "m" + to_string(machines) + ".dat"));

    outputFile << jobs << "\n" << machines << "\n";

    for (int i = 0; i < jobs; i++) {
        for (int j = 0; j < machines; j++) {
            outputFile << getRandomNumber(0, 99) << " ";
        }
        outputFile << "\n";
    }

    for (int i = 0; i < jobs; i++) {
        outputFile << (getRandomNumber(0, 100 * jobs) + 100) << " ";
    }

    outputFile.close();

    return 0;
}
