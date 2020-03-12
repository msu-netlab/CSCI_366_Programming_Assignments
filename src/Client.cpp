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
#include "Client.hpp"
#include <iostream>
#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

Client::~Client() {

}

void Client::initialize(unsigned int player, unsigned int board_size) {
    this->player = player;
    this->board_size = board_size;
    string fname = "player_" + to_string(player) + ".action_board.json";
    // 10 x 10  2D vector
    std::vector<std::vector<int>> board(board_size, vector<int>(board_size));
    std::ofstream file(fname);
    // create an output archive
    cereal::JSONOutputArchive archive(file);
    archive( CEREAL_NVP(board));
    initialized = true;
}

void Client::fire(unsigned int x, unsigned int y) {
string fname = "player_" + to_string(this->player) + ".shot.json";
ofstream file1(fname);
cereal::JSONOutputArchive read_archive1(file1);
read_archive1(CEREAL_NVP(x), CEREAL_NVP(y));
file1.close();
}



bool Client::result_available() {
    /* try to open file to read */
    ifstream player1 ("player_"+ to_string(player)+ ".result.json");
    if(player1) {
        cout<<"file exists";
    } else {
        cout<<"file doesn't exist";
   }
}


int Client::get_result() {
    string fname = ("player_" + to_string(player)+".result.json");
    int result;
    ifstream file3(fname);
    cereal::JSONInputArchive read_archive(file3);
        read_archive(result);

        if(result == HIT){
            return HIT;
        }
        if (result == MISS){
            return MISS;
        }
        if(result == OUT_OF_BOUNDS){
            return OUT_OF_BOUNDS;
        }
        if(result == 999) {
            throw ClientException("bad result");
        }



                    std:: remove("player_1.result.json");

     }








void Client::update_action_board(int result, unsigned int x, unsigned int y) {

    string fname = ("player_" + to_string(player)+".result.json");


    ifstream file3(fname);
    cereal::JSONInputArchive read_archive(file3);
    read_archive(CEREAL_NVP(x),CEREAL_NVP(y));

     x = 1;
     y = 1;



    ofstream file7( "player_" + to_string(player) + ".action_board.json");
    std::vector<std::vector<int>> board1(board_size, vector<int>(board_size));
    cereal::JSONOutputArchive outputArchive(file7);

    outputArchive(CEREAL_NVP(result));


}


string Client::render_action_board(){
}