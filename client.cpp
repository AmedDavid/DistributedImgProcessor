#include "client_functions.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <server_ip:port> <image_path> <filter_type> <filter_param>" << std::endl;
        return 1;
    }

    std::string serverAddress = argv[1];
    std::string imagePath = argv[2];
    std::string filterType = argv[3];
    double filterParam = std::stod(argv[4]);

    connectToServer(serverAddress);
    sendRequest(imagePath, filterType, filterParam);
    receiveAndSaveImage();

    return 0;
}
