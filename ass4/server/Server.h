/*
* Author: Adam Shapira; 3160044809
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "../Command/commands.h"
//#include "commands.h"
#include "../Command/CLI.h"
//#include "CLI.h"


using namespace std;

class ClientHandler {
public:
	virtual void handle(int clientID) = 0;
};


// an I/O dealing with socket communication
class SocketIO : public DefaultIO {
	int client_socket_fd;

public:
	explicit SocketIO(int clientSocketFd) : client_socket_fd(clientSocketFd) {}
	
	// reading a line from the client
	string read() override {
		string clintInput;
		char c = 0;
		int n;
		while ((n = recv(client_socket_fd, &c, sizeof(char), 0)) && c != '\n') {
			if (n == -1)
				throw "error on reciving";
			clintInput += c;
		}
		return clintInput;
	}
	
	// sending a line to the client
	void write(std::string text) override {
		if (send(client_socket_fd, text.c_str(), text.size(), 0) < 0)
			throw "error on sending";
	}
	
	// sending a float
	void write(float f) override {
		// converting the float to stream for formatting
		stringstream s;
		s << f;
		write(s.str());
	}
	
	// reading a float
	void read(float* f) override {
		auto str = read();
		*f = stof(str);
	}
};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
	// handling a client
	void handle(int clientID) override {
		
		DefaultIO* dio = new SocketIO(clientID);
		CLI cli(dio);
		cli.start();
		delete dio;
		// closing the socket
		close(clientID);
	}
};


class Server {
	std::unique_ptr<std::thread> t; // the thread to run the start() method in
	int socket_fd;
	
	// starting the server in a new thread
	void startServer(ClientHandler &ch) const noexcept(false);

public:
	
	explicit Server(int port) noexcept(false);
	
	virtual ~Server();
	
	// staring the serer
	void start(ClientHandler &ch) noexcept(false);
	
	// releasing all the resources and stopping the server
	void stop();
};

#endif /* SERVER_H_ */
