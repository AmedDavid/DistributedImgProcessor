#include "client_functions.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int clientSocket;
void connectToServer(const std::string &serverAddress) {
    // Split serverAddress into IP and port
    size_t pos = serverAddress.find(':');
    if (pos == std::string::npos) {
        std::cerr << "Invalid server address format." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string serverIP = serverAddress.substr(0, pos);
    int serverPort = std::stoi(serverAddress.substr(pos + 1));

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating client socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    sockaddr_in serverAddressStruct;
    serverAddressStruct.sin_family = AF_INET;
    serverAddressStruct.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddressStruct.sin_addr);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddressStruct, sizeof(serverAddressStruct)) == -1) {
        std::cerr << "Error connecting to the server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to the server." << std::endl;
}

void sendRequest(const std::string &imagePath, const std::string &filterType, double filterParam) {
    // Open the image file
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error opening image file: " << imagePath << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Send the image dimensions to the server
    int rows = image.rows;
    int cols = image.cols;
    ssize_t bytesSent = send(clientSocket, &rows, sizeof(rows), 0);
    if (bytesSent != sizeof(rows)) {
        std::cerr << "Error sending image dimensions to server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    bytesSent = send(clientSocket, &cols, sizeof(cols), 0);
    if (bytesSent != sizeof(cols)) {
        std::cerr << "Error sending image dimensions to server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Send the filter type and parameter to the server
    std::ostringstream oss;
    oss << filterType << " " << filterParam;
    std::string request = oss.str();
    bytesSent = send(clientSocket, request.c_str(), request.size(), 0);
    if (bytesSent != request.size()) {
        std::cerr << "Error sending request to server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Send the image data to the server
    std::vector<uchar> buffer;
    cv::imencode(".png", image, buffer);
    bytesSent = send(clientSocket, buffer.data(), buffer.size(), 0);
    if (bytesSent != buffer.size()) {
        std::cerr << "Error sending image data to server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Request sent to the server." << std::endl;
}

void receiveAndSaveImage() {
    // Receive the processed image dimensions from the server
    int rows, cols;
    ssize_t bytesReceived = recv(clientSocket, &rows, sizeof(rows), 0);
    if (bytesReceived != sizeof(rows)) {
        std::cerr << "Error receiving image dimensions from server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    bytesReceived = recv(clientSocket, &cols, sizeof(cols), 0);
    if (bytesReceived != sizeof(cols)) {
        std::cerr << "Error receiving image dimensions from server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Receive the processed image data from the server
    std::vector<uchar> buffer(rows * cols * 3); // Assuming 3 channels (BGR)
    bytesReceived = recv(clientSocket, buffer.data(), buffer.size(), 0);
    if (bytesReceived != buffer.size()) {
        std::cerr << "Error receiving image data from server." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Decode and save the received image
    cv::Mat receivedImage = cv::imdecode(cv::Mat(buffer), cv::IMREAD_UNCHANGED);
    if (receivedImage.empty()) {
        std::cerr << "Error decoding received image." << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::string savePath = "received_image.png";
    cv::imwrite(savePath, receivedImage);
    std::cout << "Image received and saved as: " << savePath << std::endl;

    // Close the client socket
    close(clientSocket);
}
