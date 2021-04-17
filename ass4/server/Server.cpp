/*
* Author: Adam Shapira; 3160044809
 */
#include "Server.h"
#include <thread>
#include <netdb.h>

Server::Server(int port) noexcept(false): socket_fd(-1) {
	// setting up the server socket
	struct sockaddr_in serverAdress{};
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		throw ("socket\n");
	}
	// setting up the ip
	hostent* record = gethostbyname("localhost");
	auto* address = (in_addr*) record->h_addr;
	string ip_address = inet_ntoa(*address);
	serverAdress.sin_family = AF_INET;
	serverAdress.sin_port = htons(port);
	if (inet_aton(ip_address.c_str(), &serverAdress.sin_addr) == 0) {
		throw ("IP Address is not valid\n");
	}
	
	// binding the socket to the ip
	if (bind(socket_fd, (const sockaddr*) &serverAdress, sizeof(serverAdress)) < 0)
		throw ("Call to bind() failed\n");
}

void Server::start(ClientHandler &ch) noexcept(false) {
	// creating a new thread for the listening
	t = std::unique_ptr<std::thread>(new thread(&Server::startServer, this, std::ref(ch)));
}

void Server::startServer(ClientHandler &ch) const noexcept(false) {
	
	// Listen for new connections
	if (listen(socket_fd, 1) < 0) {
		throw ("Error when listening to new connections\n");
	}
	
	// setting the timeout for 1 sec
	struct timeval timeout{.tv_sec = 1, .tv_usec =0};
	// setting up the socket
	int client_socket_fd;
	struct sockaddr_in clientAddress{};
	socklen_t clientAddressLength = sizeof(clientAddress);
	
	size_t timeLen = sizeof(timeout);
	while (true) {
		// starting the timout
		if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, timeLen) < 0)
			throw ("setsockopt failed\n");
		client_socket_fd = accept(socket_fd, (sockaddr*) &clientAddress, &clientAddressLength);
		if (client_socket_fd == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				//we got timeout
				break;
			else
				throw "Error in accept\n";
		}
		// creating a new thread for every client
		thread(&ClientHandler::handle, &ch, client_socket_fd).detach();
	}
}

void Server::stop() {
	t->join(); // do not delete this!
}

Server::~Server() {
}


