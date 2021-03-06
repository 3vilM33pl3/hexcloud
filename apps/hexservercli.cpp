#include <iostream>
#include <boost/program_options.hpp>
#include <hexlib/HexagonService.hpp>

namespace po = boost::program_options;

using namespace std;

void RunServer(const string ServerAddress) {
    HexagonService Service;
    ServerBuilder Builder;

    Builder.AddListeningPort(ServerAddress, grpc::InsecureServerCredentials());
    Builder.RegisterService(&Service);

    std::unique_ptr<Server> server(Builder.BuildAndStart());
    std::cout << "Server listening on port: " << ServerAddress << ":" << std::endl;

    server->Wait();
}


int main(int ac, char** av) {
    string ServerAddress = "0.0.0.0";
    int ServerPort = 8080;
    po::options_description desc("Hexagon server options");
    desc.add_options()
            ("help", "help message")
            ("address", po::value<string>(&ServerAddress)->default_value("0.0.0.0"), "address to listen on [ip]")
            ("port", po::value<int>(&ServerPort)->default_value(8080), "port listening on [port], can be overridden by PORT environment variable");


    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    ServerPort = vm["port"].as<int>();


    char* P =std::getenv("PORT");
    if(P != NULL) {
        ServerPort = std::atoi(P);
        std::cout << "port set by PORT environment variable" << std::endl;
    } else {
        ServerPort = vm["port"].as<int>();
    }
    ServerAddress = vm["address"].as<string>();

    auto ServerAddressPort = ServerAddress + ":" + to_string(ServerPort);

    // Run server
    std::cout << "Running server on " << ServerAddressPort << std::endl;
    RunServer(ServerAddressPort);

    return 0;
}