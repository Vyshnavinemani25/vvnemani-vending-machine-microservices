#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

#include "../utils.h"
#include "BeverageHandler.h"

using json = nlohmann::json;
using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace vending_machine;

// Signal handler
void sigintHandler(int sig) {
    exit(EXIT_SUCCESS);
}

// Entry point
int main(int argc, char** argv) {
    // 1: Notify signal handler
    signal(SIGINT, sigintHandler);

    // 1.1: Initialize logging
    init_logger();

    // 2: Read the config file for ports and addresses
    json config_json;
    if (load_config_file("config/service-config.json", &config_json) != 0) {
        exit(EXIT_FAILURE);
    }

    // 3: Get the port for this service
    int my_port = config_json["beverage-preference-service"]["port"];

    // 4: Configure the server
    TThreadedServer server(
        std::make_shared<BeveragePreferenceServiceProcessor>(
            std::make_shared<BeveragePreferenceServiceHandler>()),
        std::make_shared<TServerSocket>("0.0.0.0", my_port),
        std::make_shared<TFramedTransportFactory>(),
        std::make_shared<TBinaryProtocolFactory>());

    // 5: Start the server
    std::cout << "Starting the Beverage Preference Service server ..." << std::endl;
    server.serve();
    return 0;
}

