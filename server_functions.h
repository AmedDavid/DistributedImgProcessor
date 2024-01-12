#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <vector>

const int PORT = 7895;

// Function declarations
void initializeServer();
int acceptClient(int serverSocket);
void handleClients(int serverSocket);
void processClient(int clientSocket);
cv::Mat applyGammaCorrection(const cv::Mat &image, double gamma);
void sendImageToClient(int clientSocket, const cv::Mat &image);

#endif  // SERVER_FUNCTIONS_H
