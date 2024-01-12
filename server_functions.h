#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <iterator>
#include <opencv2/opencv.hpp>

const int PORT = 7895;

void initializeServer();
int acceptClient();
void handleClients();
void processClient(int clientSocket);

cv::Mat applyGammaCorrection(const cv::Mat &image, double gamma);
cv::Mat resizeImage(const cv::Mat &image, double scale);
cv::Mat rotateImage(const cv::Mat &image, double angle);
cv::Mat cropImage(const cv::Mat &image, double factor);
cv::Mat flipImage(const cv::Mat &image, double flipCode);
cv::Mat adjustBrightness(const cv::Mat &image, double factor);
cv::Mat adjustContrast(const cv::Mat &image, double factor);
void sendImageToClient(int clientSocket, const cv::Mat &image);

#endif
