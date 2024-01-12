#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server_functions.h"

int main() {
    // Server initialization code
    initializeServer();

    // Handle multiple clients using threads
    std::thread clientHandlerThread(handleClients);
    clientHandlerThread.join();

    return 0;
}
