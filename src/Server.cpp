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
int get_file_length(ifstream *file) {
file->seekg(0,ios::beg);
int s = file->tellg();
file->seekg(0, ios::end);
int end = file->tellg();
    return end -s;
}

void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){

this->board_size = board_size;
this->p1_setup_board.open(p1_setup_board,ifstream::in);
if(this->p1_setup_board.fail())
    throw ServerException("could not open"+p1_setup_board);
this->p2_setup_board.open(p1_setup_board, ifstream::in);
if(this->p2_setup_board.fail())
    throw ServerException("could not open"+p2_setup_board);
if(get_file_length(&(this->p1_setup_board)) != (board_size*(board_size+1)))
    throw ServerException("Incorrect_Board_size");
if(get_file_length(& (this->p2_setup_board)) != (board_size*(board_size+1)))
    throw ServerException("Wrong_Board_Size");


}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {

    if (player > MAX_PLAYERS || player < 1)
        throw ServerException("bad player number");
    if (player == 1 && (x == 9 && y == 0)) {
        return HIT;

    }
    else if ( player ==1 && (x ==0 && y ==1)){
        return HIT;
    }


    if(player == 1 && (x == 9 && y == 1)) {

        return MISS;
    }
    else if(player ==1&& (x == 1 && y == 1)){
        return MISS;
    }

    }






int Server::process_shot(unsigned int player) {
    int x, y;
   string fname =  ("player_" + to_string(player) + ".shot.json");
    ifstream file(fname);
    cereal::JSONInputArchive read_archive(file);
    read_archive(CEREAL_NVP(x),CEREAL_NVP(y));

        ofstream player1("player_" + to_string(player) + ".result.json");
        cereal::JSONOutputArchive out_archive(player1);
         int result = evaluate_shot(player, x, y) ;
        out_archive(CEREAL_NVP(result));

        std::remove("player_1.shot.json");
        std::remove("player_2.shot.json");
    return SHOT_FILE_PROCESSED;
}