#include "server_functions.h"

int serverSocket;  // Declare the server socket as a global variable

void initializeServer() {
    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating server socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding server socket." << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for incoming connections." << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server initialized and listening on port " << PORT << std::endl;
}

int acceptClient() {
    sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    // Accept a client connection
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
    if (clientSocket == -1) {
        std::cerr << "Error accepting client connection." << std::endl;
        return -1;
    }

    std::cout << "Accepted connection from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;

    return clientSocket;
}

void handleClients() {
    while (true) {
        // Accept client connections
        int clientSocket = acceptClient();

        // Create a new thread to handle each client
        std::thread clientThread(processClient, clientSocket);
        clientThread.detach();  // Detach the thread to run independently
    }
}

void processClient(int clientSocket) {
    // Your code to handle a single client
    // Receive image path and processing instructions
    // Process the image using OpenCV or your own functions
    // Send the processed image back to the client

    // Example: Receive image path and filter type from the client
    char buffer[1024];
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived <= 0) {
        std::cerr << "Error receiving data from client." << std::endl;
        close(clientSocket);
        return;
    }

    std::string receivedData(buffer, bytesReceived);
    std::cout << "Received data from client: " << receivedData << std::endl;

    // For simplicity, let's assume the receivedData contains the image path and filter type
    std::string imagePath = "/path/to/your/images/" + receivedData;
    std::string filterType = "gamma";
    double filterParam = 1.2;  // Example parameter

    // Process the image
    cv::Mat originalImage = cv::imread(imagePath);
    if (originalImage.empty()) {
        std::cerr << "Error loading the image." << std::endl;
        close(clientSocket);
        return;
    }

    cv::Mat processedImage;
    if (filterType == "gamma") {
        processedImage = applyGammaCorrection(originalImage, filterParam);
    }
    // Add more filter cases as needed

    // Send the processed image back to the client
    sendImageToClient(clientSocket, processedImage);

    // Close the client socket
    close(clientSocket);
}

cv::Mat applyGammaCorrection(const cv::Mat &image, double gamma) {
    // Your implementation of gamma correction using OpenCV or your own functions
    cv::Mat correctedImage;
    cv::pow(image / 255.0, gamma, correctedImage);
    correctedImage *= 255;
    return correctedImage;
}

void sendImageToClient(int clientSocket, const cv::Mat &image) {
    // Your code to send the image back to the client

    // Example: Convert the image to a byte array and send it
    std::vector<uchar> buffer;
    cv::imencode(".png", image, buffer);

    ssize_t bytesSent = send(clientSocket, buffer.data(), buffer.size(), 0);
    if (bytesSent != buffer.size()) {
        std::cerr << "Error sending data to client." << std::endl;
    }
}

