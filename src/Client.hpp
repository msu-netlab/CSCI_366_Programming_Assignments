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
#include <iostream>
#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>


using namespace std;

class ClientWrongPlayerNumberException: public exception{
public:
   virtual const char* what() const throw(){
      return "Player number out of bounds.";
   }
};

class ClientException: public exception
{
private:
   char *cstr;

public:
   ClientException(string message){
      cstr = new char[message.size() + 1];
      message.copy(cstr, message.size() + 1);
      cstr[message.size()] = '\0';
   }

   ~ClientException(){
      delete cstr;
   }

   virtual const char* what() const throw(){
      return cstr;
   }
};

class Client {
private:
   unsigned int player; ///< the number of this player - either 1 or 2
   string action_board; ///< the name of the action board file
   ConnectionSocket *socket; ///< socket connected to the server

public:
   unsigned int board_size; ///< the size of the game board

public:
   /**
    * Destructor to remove the action board created in initialize()
    */
   ~Client();

   /**
    * Initialize the client
    *
    * Creates player_#.action_board.json to store results of player shots.
    * @param player - the id of the player
    * @param socket - socket connected to the server
    * @param board_size - the square size of the action board
    */
   void initialize(unsigned int player, ConnectionSocket *socket, unsigned int board_size);


   /**
    * Fires a shot on the coordinate target
    *
    * Uses socket to send the coordinates to server
    * @param x - coordinate
    * @param y - coordinate
    */
   void fire(unsigned int x, unsigned int y);

   /**
    * Checks if a result from the server is available in socket
    *
    * @return true if result is available, false otherwise
    */
   bool result_available();

   /**
    * Gets the result from socket
    *
    * @return the result as either HIT, MISS, or OUT_OF_BOUNDS
    */
   int get_result();

   /**
    * Updates the internal representation of player action board with the results of a shot.
    *
    * @param result - the result returned from the server
    * @param x - coordinate
    * @param y - coordinate
    */
   void update_action_board(int result, unsigned int x, unsigned int y);

   /**
    * Formats an ASCII string representing the action board
    *
    * @return ASCII representation of the action board
    */
   string render_action_board();

};


