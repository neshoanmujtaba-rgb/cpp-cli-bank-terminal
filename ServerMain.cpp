#include <iostream>
#include <string>
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h> 

int main() {
    // 1. Socket erstellen AF_INET bedeutet IPv4
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM ist TCP 
    if (server_fd == -1){
        std::cerr << "Could not create socket" << std::endl; 
        return 1; 
    }

    // 2. Addresse und port festlegen
    sockaddr_in address; // declare a struct of type sockaddr_in and call it address
    address.sin_family = AF_INET; // address family = ipv4
    address.sin_addr.s_addr = INADDR_ANY; // all network interfaces 
    address.sin_port = htons(8080); // port 8080

    // 3. Socket an Port binden 
    if(bind(server_fd, (sockaddr*)&address, sizeof(address)) == -1) {
        std::cerr << "Bind failed" << std::endl; 
        return 1; 
    }
 
    // 4. auf verbindungen warten (max. 5 in der warteschlange)
    listen(server_fd, 5); 
    std::cout << "Server listening on port 8080..." <<std::endl; // add a get port number function 

    // 5. Accept connection 
    int client_fd = accept(server_fd, nullptr, nullptr); 

while(true){

    // 6. Nachrichte empfangen 
    char buffer[1024] = {}; 
    int bytes = read(client_fd, buffer, sizeof(buffer)); 

    if(bytes <=0){
        std::cout << "Client hat die Verbindung unterbrochen" << std::endl; 
    }
    
    std::cout << "Empfangen: " << buffer << std::endl; 

    // 7. Antwort senden 
    std::string antwort = "Hello from server!\n"; 

    send(client_fd, antwort.c_str(), antwort.size(), 0); 

    if (std::string(buffer) == "close connection") break;
    
    }

   
    // 8. verbindung schliessen 
    close(client_fd);
    close(server_fd); 
    return 0; 
}