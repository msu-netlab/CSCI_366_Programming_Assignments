//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Socket.hpp"

#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <sstream>



ConnectionSocket::ConnectionSocket(unsigned int socket){
   // save the socket as an class variable
}


ConnectionSocket::ConnectionSocket(string ip, unsigned int port){
   // Create a socket

   // Set up an address for the connection to the server

   // Convert IPv4 and IPv6 addresses from text to binary form

   // Connect to the server
}


void ConnectionSocket::send(string msg){
   // send data through socket
}



ConnectionSocket::~ConnectionSocket() {
   // close the socket
}



bool ConnectionSocket::data_available(){
   fd_set rfds;         // set up file descriptor mask
   struct timeval tv;   // set up timeout structure
   tv.tv_sec = 1;       // set timeout to 1 second

   FD_ZERO(&rfds);      // zero the structure for each call
   FD_SET(socket, &rfds); // configure the mask for the socket file descriptor
   int recVal = select(socket + 1, &rfds, NULL, NULL, &tv); // set mask fields

   return FD_ISSET(socket, &rfds); //check if the socket is ready for reading
}



istringstream *ConnectionSocket::receive(){
   // receive data from socket
   char buffer[1024] = {0}; //set up a buffer of memory into which we can read data

   //read up to the number of bytes in buffer

   //convert buffer to istringstream for use by cereal
   return new istringstream(string(buffer, 0));
}



ServerSocket::ServerSocket(unsigned int port, unsigned int max_connections){
   // create a socket

   // Enable forceful attach to address and port

   // Bind the socket to an address and port

   // Listen for new connections
}


ConnectionSocket *ServerSocket::accept(){
   // Accept a connection from client and creating a new socket for that connection

   return new ConnectionSocket(0);
}






