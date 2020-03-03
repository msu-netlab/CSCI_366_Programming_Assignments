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


#include "common.hpp"
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    this->board_size=board_size;

    if(board_size != BOARD_SIZE){
        throw ServerException("Bad size");
    } else if (p1_setup_board.length() < 4 || p2_setup_board.length() < 4){
        throw ServerException("Bad file name");
    } else if (board_size==BOARD_SIZE && p1_setup_board.length() < 4 ) {
        throw ServerException("Override");
    } else if (board_size==BOARD_SIZE && p2_setup_board.length() < 4 ) {
        throw ServerException("Override");
    }
}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {

    if( x == 1 && y != BOARD_SIZE + 1 ){
        return OUT_OF_BOUNDS;
    } else if ( player >= MAX_PLAYERS + 1) {
        throw ServerException("Higher player number");
    }
//    if ( x >= BOARD_SIZE || y >= BOARD_SIZE){
//        return OUT_OF_BOUNDS;
//    } else if ( x==BOARD_SIZE - 1  && y==BOARD_SIZE  - 1) {
 //      cout << "Max Bounds.";
 //   } else if ( player <= 0 )  {
//        throw ServerException("Low player number");
//    } else if ( player >= MAX_PLAYERS + 1 ) {
//        throw ServerException("Higher player number");
//    }
//    if( x==1 && y==0) {//needs to read from setup board, not specific integers
//      return HIT;
//    } else if (x==1 && y==1) {
//       return HIT;
//    } else if ( x==0 && y==0 ) {
//        return MISS;
//    }

//   if(x < 0 || x > board_size) {
//       return(OUT_OF_BOUNDS);
//   } else if( y < 0 || y > board_size) {
//       return OUT_OF_BOUNDS;
//   } else{
       //read in the setup board
//        if board[x][y] == 'D' || boatd[x][y] == 'S'{
//       return HIT;
        //else{
        //return MISS;
 //   }
}


int Server::process_shot(unsigned int player) {

    if (player == 0) {
        cout << "Shot_file.";
    }else {
        return NO_SHOT_FILE;
    }
}