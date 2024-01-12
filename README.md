# DistributedImgProcessor

DistributedImgProcessor is a project that allows distributed image processing. It consists of a server and client components for processing images with various filters.

## Features

- **Client-Server Architecture:** The project follows a client-server architecture for distributed image processing.

- **Filter Processing:** Users can send images to the server with specific filter parameters, and the server processes the images accordingly.

## Prerequisites

Before you begin, ensure you have met the following requirements:

- CMake (Version 3.5 or later)
- OpenCV (Version 4.9.0, or the version you have installed)
- C++ compiler

## Getting Started

1. **Clone the Repository:**

    ```bash
    git clone https://github.com/AmedDavid/DistributedImgProcessor
    ```

2. **Build the Project:**

    ```bash
    cd DistributedImgProcessor
    mkdir build
    cd build
    cmake ..
    ninja
    ```

3. **Run the Server:**

    ```bash
    ./Server
    ```

4. **Run the Client:**

    ```bash
    ./Client <server_ip:port> <image_path> <filter_type> <filter_param>
    ```

## Usage

- Replace `<server_ip:port>` with the server's IP address and port.
- Provide the path to the image file in `<image_path>`.
- Specify the desired `<filter_type>` and `<filter_param>`.

## Contributing

If you'd like to contribute to this project, please follow these steps:

1. Fork the repository.
2. Create a new branch: `git checkout -b feature/your-feature`.
3. Make your changes and commit them: `git commit -m 'Add your feature'`.
4. Push to the branch: `git push origin feature/your-feature`.
5. Submit a pull request.

## License

DistributedImgProcessor is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

If you have any questions or issues, please contact David Amedi at amukoahdavid@gmail.com.
