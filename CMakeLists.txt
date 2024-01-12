cmake_minimum_required(VERSION 3.5)
project(DistributedImgProcessor VERSION 0.1.0)

# Set the OpenCV directory
set(OpenCV_DIR "/home/david/Documents/clones/opencv/build")

# Find OpenCV components
find_package(OpenCV REQUIRED)

# Add the client executable
add_executable(Client client.cpp client_functions.cpp)

# Add the server executable
add_executable(Server server.cpp server_functions.cpp)

# Link against OpenCV for both executables
target_include_directories(Client PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(Server PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(Client PRIVATE ${OpenCV_LIBS})
target_link_libraries(Server PRIVATE ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
