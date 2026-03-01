#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // neu fuer inet_pton()

int main(){
    // 1. Socket erstellen 
    int client_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (client_fd == -1) {
        std::cout << "Failed to create socket\n" << std::endl; 
        return 1; 
    }


    // 2. Server-Addresse festlegen 
    sockaddr_in server_address; 
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(8080); 
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); 

    // 3. Verbindung herstellen 
    if (connect(client_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Verbindung fehlgeschlagen!" << std::endl; 
    }
    
    std::cout <<"Mit dem Server verbunden!" << std::endl; 

    while (true) {
        std::string nachricht; 
        std::cout << "You: "; 
        std::getline (std::cin, nachricht); 

        // Senden 
        send(client_fd, nachricht.c_str(), nachricht.size(), 0); 

    }
    close(client_fd);
    return 0;
}