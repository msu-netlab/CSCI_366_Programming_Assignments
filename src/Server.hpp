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

#include "BitArray2D.hpp"
#include "Socket.hpp"

#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <cstdio>
#include <semaphore.h>
#include <cereal/archives/json.hpp>

#define SHIPS "CBRSD" //Carrier, Battleship, cRuiser, Submarine, and Destroyer

using namespace std;


class ServerException: public exception
{
private:
   char *cstr;

public:
   ServerException(string message){
      cstr = new char[message.size() + 1];
      message.copy(cstr, message.size() + 1);
      cstr[message.size()] = '\0';
   }

   ~ServerException(){
      delete cstr;
   }

   virtual const char* what() const throw(){
      return cstr;
   }
};



class Server{
public:
   unsigned int board_size;   ///< the size of the game board files

private:
   BitArray2D *p1_setup_board = nullptr;  ///< internal representation of a player setup board
   BitArray2D *p2_setup_board = nullptr;  ///< internal representation of a player setup board

   ConnectionSocket *socket; ///< socket connected to a client

   /**
    * Sets up a BitArray2D pointer with bits filled in based on a setup_board_name
    * @param setup_board_name - the name of the setup board file
    * @return an internally allocated pointer to a BitArray2D object
    */
   BitArray2D *scan_setup_board(string setup_board_name);

public:
   /**
    * Performs Server initialization
    *
    * The method opens player setup boards containing ship positions and checks the size of boards compared to
    * board_size parameter.
    * @param board_size - the size of a square board
    * @param socket - socket connect to client
    * @param p1_setup_board - file name of player 1's board
    * @param p2_setup_board - file name of player 2's board
    */
   void initialize(unsigned int board_size,
                   ConnectionSocket *socket,
                   string p1_setup_board,
                   string p2_setup_board);

   /**
    * Checks if a shot is available in socket
    *
    * @return true if shot is available, false otherwise
    */
   bool shot_available();

   /**
    * Checks the coordinates of a shot against setup board of player
    *
    * Check that player number within bounds, checks that shot coordinates within bounds, determines if the shot
    * results in a HIT, or a MISS.
    * @param x - coordinate
    * @param y - coordinate
    * @return returns shot result as either HIT, MISS, or OUT_OF_BOUNDS
    */
   int evaluate_shot(unsigned int player, unsigned int x, unsigned int y);

   /**
    * Processes a shot issued by player
    *
    * Gets the shot from player, extracts coordinates, passes the information of evaluate_shot, and writes the result
    * into the internal player board representation.
    * Player number comes from the fire message sent by the client.
    * @param player_sem - an array of two semaphores that controls if this, or the other player number should have their shot processed
    */
   void process_shot(sem_t *player_sem);

   /**
    * Deallocates Server memory
    */
   ~Server();
};
