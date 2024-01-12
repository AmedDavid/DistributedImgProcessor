#include <iostream>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "client_functions.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <server_ip:port> <image_path> <filter_type> <filter_param>" << std::endl;
        return 1;
    }

    // Parse command line arguments
    std::string serverAddress = argv[1];
    std::string imagePath = argv[2];
    std::string filterType = argv[3];
    double filterParam = std::stod(argv[4]);

    // Client logic
    sendImage(serverAddress, imagePath, filterType, filterParam);
    receiveProcessedImage(serverAddress);

    return 0;
}
