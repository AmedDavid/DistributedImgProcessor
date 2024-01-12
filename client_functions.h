#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Function declarations
void sendImage(const std::string &serverAddress, const std::string &imagePath, const std::string &filterType, double filterParam);
void receiveProcessedImage(const std::string &serverAddress);

#endif  // CLIENT_FUNCTIONS_H
