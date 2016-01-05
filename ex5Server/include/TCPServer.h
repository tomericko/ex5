/*
 * TCPServer.h
 *
 *  Created on: Dec 23, 2015
 *      Author: tomericko
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_
#include "Server.h"
#include "MoviesSystem.h"


/*******************************************************************************
 * class name : TCPServer
 *******************************************************************************/
class TCPServer: public Server {
private:
	struct sockaddr_in client_sin;
	int client_sock;
	static TCPServer* serv;
	static bool serverConstruct;
	static pthread_mutex_t lock;
public:
	static TCPServer* getServerIns(int port);
	 /*******************************************************************************
	 * function name : ~TCPServer												       *
	 * input : nothing.														       *
	 * output : nothing.														       *
	 * explanation : destructor of a TCPServer.										   *
	 *******************************************************************************/
	~TCPServer();

	/*******************************************************************************
	 * function name : TCPServer												       *
	 * input : nothing.														       *
	 * output : ip as string.												       *
	 * explanation : constructor of a TCPServer.									   *
	 *******************************************************************************/
	TCPServer(int port);

	/*******************************************************************************
	* function name : connect												       *
	* input : connect to a certain socket.									       *
	* output : nothing.														       *
	* explanation : sending the data to the socket.								   *
	*******************************************************************************/
	void connect();

	/*******************************************************************************
	 * function name : connEstablish										       *
	 * input : nothing.														       *
	 * output : nothing.													       *
	 * explanation : creating a socket.											   *
	 *******************************************************************************/
	void connEstablish();
	static void* threadFactory(void* var);
	/*******************************************************************************
	* function name : getDataReceived										       *
	* input : nothing.														       *
	* output : the massage as string.										       *
	* explanation : return the massage as string.								   *
	*******************************************************************************/
	string getDataReceived();

	/*******************************************************************************
	 * function name : sendData												       *
	 * input : data as string.												       *
	 * output : nothing.													       *
	 * explanation : sending the data to the socket.							   *
	 *******************************************************************************/
	void sendData(string data);

	/*******************************************************************************
	 * function name : dataReceiver											       *
	 * input : nothing.														       *
	 * output : nothing.													       *
	 * explanation : receive the massage to the buffer.							   *
	 *******************************************************************************/
	void dataReceiver();

	/*******************************************************************************
	 * function name : bindSocket											       *
	 * input : nothing.														       *
	 * output : nothing.													       *
	 * explanation : make bind to a client.										   *
	 *******************************************************************************/
	void bindSocket();

	/*******************************************************************************
	* function name : setSocket												       *
	* input : ip address as string and socket as int.						       *
	* output : nothing.														       *
	* explanation : setting the socket.											   *
	*******************************************************************************/
	void setSocket(char* ip, int port);

	/*******************************************************************************
	* function name : createSocket											       *
	* input : nothing.														       *
	* output : nothing.														       *
	* explanation : creating a socket in this connection.						   *
	*******************************************************************************/
	void createSocket();

};

#endif /* TCPSERVER_H_ */
