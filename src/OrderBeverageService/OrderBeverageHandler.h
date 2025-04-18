#ifndef VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/OrderBeverageService.h"
#include "../../gen-cpp/WeatherService.h"
#include "../../gen-cpp/BeveragePreferenceService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace vending_machine {

class OrderBeverageServiceHandler : public OrderBeverageServiceIf {
 public:
  OrderBeverageServiceHandler(
      ClientPool<ThriftClient<WeatherServiceClient>> *,
      ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *);
  ~OrderBeverageServiceHandler() override = default;

  void PlaceOrder(std::string& _return, const int64_t city) override;

 private:
  ClientPool<ThriftClient<WeatherServiceClient>> *_weather_client_pool;
  ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *_beverage_client_pool;
};

// Constructor
OrderBeverageServiceHandler::OrderBeverageServiceHandler(
    ClientPool<ThriftClient<WeatherServiceClient>> *weather_client_pool,
    ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *beverage_client_pool) {
  // Storing the client pools
  _weather_client_pool = weather_client_pool;
  _beverage_client_pool = beverage_client_pool;
}

// Remote Procedure "PlaceOrder"
void OrderBeverageServiceHandler::PlaceOrder(std::string& _return, const int64_t city) {
  // Print debug information
  printf("PlaceOrder\n");

  // 1. Get the weather service client pool
  auto weather_client_wrapper = _weather_client_pool->Pop();
  if (!weather_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
    se.message = "Failed to connect to weather-service";
    throw se;
  }
  auto weather_client = weather_client_wrapper->GetClient();

  // By default, set weather to COLD
  WeatherType::type weatherType = WeatherType::type::COLD;

  // 2. Call the remote procedure: GetWeather
  try {
    weatherType = weather_client->GetWeather(city);
  } catch (...) {
    _weather_client_pool->Push(weather_client_wrapper);
    LOG(error) << "Failed to send call GetWeather to weather-client";
    throw;
  }
  _weather_client_pool->Push(weather_client_wrapper);

  // 3. Determine the BeverageType based on the weather
  BeverageType::type beverageType =
      (weatherType == WeatherType::type::WARM) ? BeverageType::type::COLD : BeverageType::type::HOT;

  // 4. Get the beverage preference service client pool
  auto beverage_client_wrapper = _beverage_client_pool->Pop();
  if (!beverage_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
    se.message = "Failed to connect to beverage-preference-service";
    throw se;
  }
  auto beverage_client = beverage_client_wrapper->GetClient();

  // 5. Call the remote procedure: getBeverage
  try {
    beverage_client->getBeverage(_return, beverageType); // Pass _return and beverageType
  } catch (...) {
    _beverage_client_pool->Push(beverage_client_wrapper);
    LOG(error) << "Failed to send call getBeverage to beverage-client";
    throw;
  }
  _beverage_client_pool->Push(beverage_client_wrapper);
}

}  // namespace vending_machine

#endif  // VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H
