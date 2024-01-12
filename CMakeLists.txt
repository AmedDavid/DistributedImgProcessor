cmake_minimum_required(VERSION 3.5)
# cmake_minimum_required(VERSION 3.27.9)
project(DistributedImgProcessor VERSION 0.1.0)

find_package(OpenCV REQUIRED PATHS "/home/david/Documents/clones/opencv/build")

include_directories(${OpenCV_INCLUDE_DIRS})

# Add the client executable
add_executable(Client client.cpp client_functions.cpp)

# Add the server executable
add_executable(Server server.cpp server_functions.cpp)

# Link against OpenCV for both executables
target_link_libraries(Client ${OpenCV_LIBS})
target_link_libraries(Server ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
