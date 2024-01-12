#include "server_functions.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int serverSocket;

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
        int clientSocket = acceptClient();

        if (clientSocket != -1) {
            std::thread clientThread(processClient, clientSocket);
            clientThread.detach();
        }
    }
}

void processClient(int clientSocket) {
    // Receive the image name from the client
    std::vector<char> imageData(256); // Assuming a maximum of 256 characters
    ssize_t imageBytesReceived = recv(clientSocket, imageData.data(), imageData.size(), 0);
    if (imageBytesReceived < 0) {
        std::cerr << "Error receiving image name from client." << std::endl;
        close(clientSocket);
        return;
    }

    std::string imageName(imageData.data());
    std::cout << "Received image name from client: " << imageName << std::endl;

    // Receive the filter type and parameter from the client
    std::vector<char> filterData(256); // Assuming a maximum of 256 characters
    ssize_t filterBytesReceived = recv(clientSocket, filterData.data(), filterData.size(), 0);
    if (filterBytesReceived < 0) {
        std::cerr << "Error receiving filter data from client." << std::endl;
        close(clientSocket);
        return;
    }

    std::istringstream iss(std::string(filterData.data()));
    std::string filterType;
    double filterParam;
    std::string imagePath; // Add this line to store the image path

    // Parse filter data and image path
    iss >> filterType >> filterParam >> imagePath;

    // Load the image from the provided path
    cv::Mat originalImage = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (originalImage.empty()) {
        std::cerr << "Error loading the image." << std::endl;
        close(clientSocket);
        return;
    }

    // Apply the requested filter
    cv::Mat processedImage;
    if (filterType == "gamma") {
        processedImage = applyGammaCorrection(originalImage, filterParam);
    } else if (filterType == "resize") {
        processedImage = resizeImage(originalImage, filterParam);
    } else if (filterType == "rotate") {
        processedImage = rotateImage(originalImage, filterParam);
    } else if (filterType == "crop") {
        processedImage = cropImage(originalImage, filterParam);
    } else if (filterType == "flip") {
        processedImage = flipImage(originalImage, filterParam);
    } else if (filterType == "brightness") {
        processedImage = adjustBrightness(originalImage, filterParam);
    } else if (filterType == "contrast") {
        processedImage = adjustContrast(originalImage, filterParam);
    } else {
        std::cerr << "Invalid filter type." << std::endl;
        close(clientSocket);
        return;
    }

    // Send the processed image back to the client
    sendImageToClient(clientSocket, processedImage);
    std::cout << "Processed image sent to the client." << std::endl;

    // Close the client socket
    close(clientSocket);
}

cv::Mat applyGammaCorrection(const cv::Mat &image, double gamma) {
    cv::Mat correctedImage;
    cv::pow(image / 255.0, gamma, correctedImage);
    correctedImage *= 255;
    return correctedImage;
}

cv::Mat resizeImage(const cv::Mat &image, double scale) {
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(), scale, scale);
    return resizedImage;
}

cv::Mat rotateImage(const cv::Mat &image, double angle) {
    cv::Mat rotatedImage;
    cv::rotate(image, rotatedImage, cv::ROTATE_90_CLOCKWISE);
    return rotatedImage;
}

cv::Mat cropImage(const cv::Mat &image, double factor) {
    cv::Rect cropRegion(image.cols * factor, image.rows * factor, image.cols * (1 - 2 * factor), image.rows * (1 - 2 * factor));
    cv::Mat croppedImage = image(cropRegion);
    return croppedImage;
}

cv::Mat flipImage(const cv::Mat &image, double flipCode) {
    cv::Mat flippedImage;
    cv::flip(image, flippedImage, flipCode);
    return flippedImage;
}

cv::Mat adjustBrightness(const cv::Mat &image, double factor) {
    cv::Mat brightenedImage;
    image.convertTo(brightenedImage, -1, 1.0, factor * 255.0);
    return brightenedImage;
}

cv::Mat adjustContrast(const cv::Mat &image, double factor) {
    cv::Mat contrastedImage;
    image.convertTo(contrastedImage, -1, factor);
    return contrastedImage;
}

void sendImageToClient(int clientSocket, const cv::Mat &image) {
    std::vector<uchar> buffer;
    cv::imencode(".png", image, buffer);

    ssize_t totalBytesSent = 0;
    size_t bufferSize = buffer.size();

    while (totalBytesSent < bufferSize) {
        ssize_t bytesSent = send(clientSocket, buffer.data() + totalBytesSent, bufferSize - totalBytesSent, 0);

        if (bytesSent == -1) {
            std::cerr << "Error sending data to client: " << strerror(errno) << std::endl;
            close(clientSocket);
            return;
        }

        totalBytesSent += bytesSent;
    }

    std::cout << "Image sent to the client." << std::endl;
}

