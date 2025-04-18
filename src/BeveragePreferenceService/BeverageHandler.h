#ifndef VENDING_MACHINE_MICROSERVICES_BEVERAGEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_BEVERAGEHANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../../gen-cpp/BeveragePreferenceService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace vending_machine {

class BeveragePreferenceServiceHandler : public BeveragePreferenceServiceIf {
 public:
  // Constructor
  BeveragePreferenceServiceHandler();
  ~BeveragePreferenceServiceHandler() override = default;

  // Corrected method signature to match Thrift interface
  void getBeverage(std::string& _return, const BeverageType::type btype) override;
};

// Constructor Implementation
BeveragePreferenceServiceHandler::BeveragePreferenceServiceHandler() {
    std::srand(std::time(nullptr));  // Seed the random number generator
}

// Implementation of getBeverage
void BeveragePreferenceServiceHandler::getBeverage(std::string& _return, const BeverageType::type btype) {
    // Define the hot and cold beverage lists
    std::vector<std::string> hotBeverages = {"cappuccino", "latte", "espresso"};
    std::vector<std::string> coldBeverages = {"lemonade", "ice tea", "soda"};

    // Select a random beverage based on the type
    if (btype == BeverageType::type::HOT) {
        _return = hotBeverages[std::rand() % hotBeverages.size()];
    } else if (btype == BeverageType::type::COLD) {
        _return = coldBeverages[std::rand() % coldBeverages.size()];
    } else {
        _return = "Unknown Beverage Type";
    }
}

}  // namespace vending_machine

#endif  // VENDING_MACHINE_MICROSERVICES_BEVERAGEHANDLER_H

