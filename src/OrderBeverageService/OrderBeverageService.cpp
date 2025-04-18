#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

#include "../utils.h"
#include "OrderBeverageHandler.h"

using json = nlohmann::json;
using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace vending_machine;

// Signal handler code
void sigintHandler(int sig) {
    exit(EXIT_SUCCESS);
}

// Entry point of this service
int main(int argc, char **argv) {
    // 1: Notify the signal handler if interrupted
    signal(SIGINT, sigintHandler);

    // 1.1: Initialize logging
    init_logger();

    // 2: Read the config file for ports and addresses
    json config_json;
    if (load_config_file("config/service-config.json", &config_json) != 0) {
        exit(EXIT_FAILURE);
    }

    // 3: Get my port
    int my_port = config_json["order-beverage-service"]["port"];

    // 4: Get the weather service's port and address
    int weather_service_port = config_json["weather-service"]["port"];
    std::string weather_service_addr = config_json["weather-service"]["addr"];

    // 5: Get the beverage preference service's port and address
    int beverage_service_port = config_json["beverage-preference-service"]["port"];
    std::string beverage_service_addr = config_json["beverage-preference-service"]["addr"];

    // 6: Create client pools for WeatherService and BeveragePreferenceService
    ClientPool<ThriftClient<WeatherServiceClient>> weather_client_pool(
        "weather-service", weather_service_addr, weather_service_port, 0, 128, 1000);

    ClientPool<ThriftClient<BeveragePreferenceServiceClient>> beverage_client_pool(
        "beverage-preference-service", beverage_service_addr, beverage_service_port, 0, 128, 1000);

    // 7: Configure this server
    TThreadedServer server(
        std::make_shared<OrderBeverageServiceProcessor>(
            std::make_shared<OrderBeverageServiceHandler>(
                &weather_client_pool,
                &beverage_client_pool)), // Pass both client pools
        std::make_shared<TServerSocket>("0.0.0.0", my_port),
        std::make_shared<TFramedTransportFactory>(),
        std::make_shared<TBinaryProtocolFactory>()
    );

    // 8: Start the server
    std::cout << "Starting the order-beverage server ..." << std::endl;
    server.serve();
    return 0;
}

