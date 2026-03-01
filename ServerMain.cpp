#include <iostream>
#include <string>
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h> 
#include <vector>

class Socket {

    // RAII (Resource Acquisition Is Initialisation) means you tie a resource's lifetime to an object, when the object is destroyed, the resource is cleaned up automatically.
    public: 
    int fd; 

    // Constructor creates the socket 
    Socket(int domain, int type, int protocol){
        fd = socket(domain, type, protocol);
    }

    // Destructor: automatically called when object goes out of scope
    ~Socket() {
        if (fd != -1) {
            close(fd);
            std::cout << "Socket closed automatically" << std::endl;
        }
    }

    // Prevent accidental copying (important as two objects closing same fd is bad)
    Socket(const Socket&) = delete; 
    Socket& operator=(const Socket&) = delete; 
};

int main() {

    // 1. Socket erstellen AF_INET bedeutet IPv4
    Socket server(AF_INET, SOCK_STREAM, 0); // socket created 
    if (server.fd == -1){
        std::cerr << "Could not create socket" << std::endl; 
        return 1; 
    }

    // 2. Addresse und port festlegen
    sockaddr_in address; // declare a struct of type sockaddr_in and call it address
    address.sin_family = AF_INET; // address family = ipv4
    address.sin_addr.s_addr = INADDR_ANY; // all network interfaces 
    address.sin_port = htons(8080); // port 8080

    // Telling the OS to allow reuse of tis port even if it is in TIME_WAIT
    int opt =1; 
    if (setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "setsockopt failed" << std::endl; 
    }
    
    // 3. Socket an Port binden 
    if(bind(server.fd, (sockaddr*)&address, sizeof(address)) == -1) {
        std::cerr << "Bind failed" << std::endl; 
        return 1; 
    }
 
    // 4. auf verbindungen warten (max. 5 in der warteschlange)
    listen(server.fd, 5); 
    std::cout << "Server listening on port 8080..." <<std::endl; // add a get port number function 

    // 5. Accept connection 
    int client_fd = accept(server.fd, nullptr, nullptr); 

    while(true){

        // 6. Nachrichte empfangen 
        std::vector<uint8_t>buffer(1024,0); // 1024 bytes, all zeroed 

        // buffer.data(0) gives the raw pointer read() needs...
        int bytes = read(client_fd, buffer.data(), buffer.size() -1); 

        if(bytes <=0){
            std::cout << "Client hat die Verbindung unterbrochen" << std::endl; 
            break; 
        }
        
        buffer[bytes] = '\0'; // null termination discipline! 
        std::string received(buffer.begin(), buffer.begin() + bytes); 
        std::cout << "Empfangen: " << received << std::endl; 
        
        if (received == "close connection") break;

        // 7. Antwort senden 
        std::string antwort = "Hello from server!\n"; 

        send(client_fd, antwort.c_str(), antwort.size(), 0);         
    }  
    // 8. verbindung schliessen 
    close(client_fd);
    close(server.fd); 
    return 0;
}