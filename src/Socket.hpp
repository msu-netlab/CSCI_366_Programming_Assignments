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

#ifndef BATTLESHIP_SOCKET_HPP
#define BATTLESHIP_SOCKET_HPP

#include <exception>
#include <string>

using namespace std;

class SocketException: public exception
{
private:
   char *cstr;

public:
   SocketException(string message){
      cstr = new char[message.size() + 1];
      message.copy(cstr, message.size() + 1);
      cstr[message.size()] = '\0';
   }

   ~SocketException(){
      delete cstr;
   }

   virtual const char* what() const throw(){
      return cstr;
   }
};


class ConnectionSocket {
private:
   unsigned int socket = 0;   ///< socket file descriptor

public:
   /**
    * Creates a ConnectionSocket given a file descriptor of a connected socket
    *
    * @param socket - file descriptor of a connected socket
    */
   ConnectionSocket(unsigned int socket);

   /**
    * Creates a ConnectionSocket by connecting to ip and port
    *
    * @param ip - IP address of a ServerSocket
    * @param port - port of ServerSocket
    */
   ConnectionSocket(string ip, unsigned int port);

   /**
    * Destroys a ConnectionSocket
    *
    * Also closes the file socket file descriptor
    */
   ~ConnectionSocket();

   /**
    * Sends data via socket
    *
    * @param msg - message data to send
    */
   virtual void send(string msg);

   /**
    * Checks whether socket has data buffered for reading
    *
    * @return - true, if data available for reading
    */
   virtual bool data_available();

   /**
    * Reads data from socket
    *
    * @return - bytes read from the socket as a string stream
    */
   virtual istringstream *receive();
};


class ServerSocket {
private:
   unsigned int socket = 0; ///< file descriptor of a server socket

public:
   /**
    * Creates a server socket and puts it into a listening state
    *
    * @param port - socket port
    * @param max_connections - the maximum number of clients allowed to connect to socket
    */
   ServerSocket(unsigned int port, unsigned int max_connections);

   /**
    * Accepts a connection to the server socket
    *
    * @return - a new ConnectionSocket from the accepted connection
    */
   ConnectionSocket *accept();
};


#endif //BATTLESHIP_SOCKET_HPP
