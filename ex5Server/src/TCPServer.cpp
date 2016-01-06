#include "TCPServer.h"
#include <map>
#include <thread>


/*******************************************************************************
 * function name : TCPServer											       *
 * input : nothing.														       *
 * output : ip as string.												       *
 * explanation : constructor of a TCPServer.								   *
 *******************************************************************************/
TCPServer::TCPServer(int port) :
		Server(port) {
	serverConstruct = false;
	this->client_sock = 0;
	this->createSocket();
	this->bindSocket();
	this->connect();
	map<char, pthread_t> m;

}

/*******************************************************************************
 * function name : ~TCPServer											       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : destructor of a TCPServer.									   *
 *******************************************************************************/
TCPServer::~TCPServer() {
}

/*******************************************************************************
 * function name : connect												       *
 * input : connect to a certain socket.									       *
 * output : nothing.														       *
 * explanation : sending the data to the socket.								   *
 *******************************************************************************/
void TCPServer::connect() {
	int back_log = 1; //need to change
	if (listen(this->getSocket(), back_log) < 0) {
		perror("error listening to a socket");
	}
}

/*******************************************************************************
 * function name : createSocket											       *
 * input : nothing.														       *
 * output : nothing.														       *
 * explanation : creating a socket in this connection.						   *
 *******************************************************************************/
void TCPServer::createSocket() {
	this->sock = socket(AF_INET, SOCK_STREAM, 0);

	if (this->getSocket() < 0) {
		perror("error creating socket");
	}
}

/*******************************************************************************
 * function name : setSocket												       *
 * input : ip address as string and socket as int.						       *
 * output : nothing.														       *
 * explanation : setting the socket.											   *
 *******************************************************************************/
void TCPServer::setSocket(char* ip, int sock) {
	this->sock = sock;
	this->ip = NULL;
}

/*******************************************************************************
 * function name : bindSocket											       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : make bind to a client.										   *
 *******************************************************************************/
void TCPServer::bindSocket() {

	memset(&this->sin, 0, sizeof(this->sin));
	this->sin.sin_family = AF_INET;
	this->sin.sin_addr.s_addr = INADDR_ANY;
	this->sin.sin_port = htons(this->port);

	if (bind(this->sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
		perror("error binding socket");
	}
}

/*******************************************************************************
 * function name : connEstablish										       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : creating a socket.											   *
 *******************************************************************************/
void TCPServer::connEstablish() {

	unsigned int addr_len = sizeof(this->client_sin);
	this->client_sock = accept(this->getSocket(),
			(struct sockaddr *) &(this->client_sin), &addr_len);
	if (client_sock < 0) {
		perror("error accepting client");
	}

}

void TCPServer::threadFactory(){
	MoviesSystem* ms = MoviesSystem::getInstance();
	int statusCreate;
	while(true){
		pthread_t ptrd;
		this->connEstablish();
		statusCreate = pthread_create(&ptrd,NULL,ms->start,&this->client_sock);
		if(statusCreate != 0){
			//error
		}
		this->addThread(ptrd);
	}
}

/*******************************************************************************
 * function name : sendData												       *
 * input : data as string.												       *
 * output : nothing.													       *
 * explanation : sending the data to the socket.							   *
 *******************************************************************************/
void TCPServer::sendData(string data) {
	int data_len = data.length();
	if (data_len == 0) {
		data = "\0";
		data_len = 1;
	}
	int sent_bytes = send(this->client_sock, data.c_str(), data_len, 0);
	if (sent_bytes < 0) {
		perror("error sending to client");
	}

}

/*******************************************************************************
 * function name : dataReceiver											       *
 * input : nothing.														       *
 * output : nothing.													       *
 * explanation : receive the massage to the buffer.							   *
 *******************************************************************************/
void TCPServer::dataReceiver() {
	char buffer[4096];
	int expected_data_len = sizeof(buffer);
	memset(&(buffer), 0, sizeof(buffer));
	int read_bytes = recv(this->client_sock, buffer, expected_data_len, 0);
	if (read_bytes == 0) {
		cout << "connection is close\n";
	} else if (read_bytes < 0) {
		perror("error reading from client");
	}
	this->dataReceived = buffer;

}
