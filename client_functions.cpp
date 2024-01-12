#include "client_functions.h"

void sendImage(const std::string &serverAddress, const std::string &imagePath, const std::string &filterType, double filterParam) {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating client socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Parse server address and port
    size_t colonPos = serverAddress.find(":");
    if (colonPos == std::string::npos) {
        std::cerr << "Invalid server address format." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::string serverIP = serverAddress.substr(0, colonPos);
    int serverPort = std::stoi(serverAddress.substr(colonPos + 1));

    // Set up server address struct
    sockaddr_in serverAddressStruct;
    serverAddressStruct.sin_family = AF_INET;
    serverAddressStruct.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddressStruct.sin_port = htons(serverPort);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddressStruct, sizeof(serverAddressStruct)) == -1) {
        std::cerr << "Error connecting to the server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Send image path and filter type to the server
    std::string requestData = imagePath + " " + filterType + " " + std::to_string(filterParam);
    ssize_t bytesSent = send(clientSocket, requestData.c_str(), requestData.size(), 0);
    if (bytesSent != requestData.size()) {
        std::cerr << "Error sending data to the server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Close the client socket
    close(clientSocket);
}

void receiveProcessedImage(const std::string &serverAddress) {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating client socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Parse server address and port
    size_t colonPos = serverAddress.find(":");
    if (colonPos == std::string::npos) {
        std::cerr << "Invalid server address format." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::string serverIP = serverAddress.substr(0, colonPos);
    int serverPort = std::stoi(serverAddress.substr(colonPos + 1));

    // Set up server address struct
    sockaddr_in serverAddressStruct;
    serverAddressStruct.sin_family = AF_INET;
    serverAddressStruct.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddressStruct.sin_port = htons(serverPort);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddressStruct, sizeof(serverAddressStruct)) == -1) {
        std::cerr << "Error connecting to the server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Receive the processed image from the server
    std::vector<uchar> buffer(1024);  // Adjust the buffer size as needed
    ssize_t bytesRead = recv(clientSocket, buffer.data(), buffer.size(), 0);
    if (bytesRead <= 0) {
        std::cerr << "Error receiving data from the server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Decode the received data into an image
    cv::Mat receivedImage = cv::imdecode(buffer, cv::IMREAD_COLOR);

    // Save the processed image
    cv::imwrite("processed_image.png", receivedImage);

    // Close the client socket
    close(clientSocket);
}
