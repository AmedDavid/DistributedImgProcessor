#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <iostream>
#include <opencv2/opencv.hpp>

void connectToServer(const std::string &serverAddress);
void sendRequest(const std::string &imagePath, const std::string &filterType, double filterParam);
void receiveAndSaveImage();

#endif
