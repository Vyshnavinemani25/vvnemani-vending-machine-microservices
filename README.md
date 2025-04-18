# Vending Machine Microservices

A modular, multi-language microservices architecture simulating a vending machine system. This project leverages Apache Thrift for cross-language communication and includes services written in C++, Python, and Lua, orchestrated using Docker and Nginx.

---

## 🧩 Architecture Overview

- **Service Communication**: Utilizes Apache Thrift for defining and generating cross-language service interfaces.
- **Languages Used**: C++ (core logic), Python, and Lua (additional services).
- **Containerization**: Employs Docker for containerizing services and Docker Compose for orchestration.
- **Reverse Proxy**: Nginx is configured as a reverse proxy to route requests to appropriate services.

---

## 📁 Project Structure

```plaintext
├── cmake/                  # CMake configuration files
├── config/                 # Configuration files for services
├── gen-cpp/                # Thrift-generated C++ code
├── gen-lua/                # Thrift-generated Lua code
├── gen-py/                 # Thrift-generated Python code
├── nginx_reverse_proxy/    # Nginx configuration for reverse proxy
├── script/                 # Utility scripts
├── src/                    # Source code for services
├── thrift/                 # Thrift IDL files
├── CMakeLists.txt          # CMake build script
├── Dockerfile              # Dockerfile for building the image
├── docker-compose.yml      # Docker Compose configuration
├── vending_machine.thrift  # Thrift service definitions
└── README.md               # Project documentation
```

---

## 🚀 Getting Started

### Prerequisites

- Docker and Docker Compose installed on your system.
- CMake and a C++ compiler (e.g., GCC or Clang) for building C++ services.
- Python 3.x and Lua installed for respective services.

### Build and Run

1. Clone the repository:

    ```bash
    git clone https://github.com/Vyshnavinemani25/vvnemani-vending-machine-microservices.git
    cd vvnemani-vending-machine-microservices
    ```

2. Generate Thrift code:

    ```bash
    thrift --gen cpp vending_machine.thrift
    thrift --gen py vending_machine.thrift
    thrift --gen lua vending_machine.thrift
    ```

3. Build the C++ services:

    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```

4. Start all services using Docker Compose:

    ```bash
    docker-compose up --build
    ```

    This will build and start all services, including the Nginx reverse proxy.

---

## 🧪 Testing the Services

Once all services are up and running, you can test the endpoints via the Nginx reverse proxy. Assuming Nginx is configured to listen on `localhost:8080`, you can send requests to:

- `http://localhost:8080/service1/endpoint`
- `http://localhost:8080/service2/endpoint`

Replace `service1/endpoint` and `service2/endpoint` with the actual service paths defined in your Nginx configuration.

---

## 🛠️ Technologies Used

- **Languages**: C++, Python, Lua
- **RPC Framework**: Apache Thrift
- **Build System**: CMake
- **Containerization**: Docker, Docker Compose
- **Web Server**: Nginx

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## 🙌 Acknowledgments

Inspired by modular microservices architectures and the need for cross-language service communication using Apache Thrift.
