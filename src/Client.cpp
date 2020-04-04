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
#include "Server.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){

    this->player = player;
    this->board_size = board_size;
    if (player < 1 || player > MAX_PLAYERS ){
        throw ClientWrongPlayerNumberException();
    }

    this->board_name = "player_" + to_string(player) + ".action_board.json";      //Vector ffor action board
    vector<vector<int>> board(board_size, vector<int>(board_size) );
    ofstream actfile(board_name);
    cereal::JSONOutputArchive archive(actfile);
    //Casts vector to name value pair and archives it
    archive(CEREAL_NVP(board));
    this->initialized = true;

}


void Client::fire(unsigned int x, unsigned int y) {

    this->board_name = "player_" + to_string(player) + ".shot.json";      //Vector ffor action board
    ofstream actfile(board_name);
    cereal::JSONOutputArchive archive(actfile);
    archive(CEREAL_NVP(x), CEREAL_NVP(y));

    cout << "Shot fired at " + to_string(x) + ", " + to_string(y) << endl;
}


bool Client::result_available() {

    string f1 = "player_1.result.json";
    string f2 = "player_2.result.json";
    ifstream p1(f1);
    ifstream p2(f2);

    if (p1 || p2) {
        return true;
    } else {
        return false;
    }

}


int Client::get_result() {

    int result;
    ifstream resultfile("player_1.result.json");
    cereal::JSONInputArchive archive(resultfile);
    archive(CEREAL_NVP(result));

    switch (result) {
        case HIT:
            std::remove("player_1.result.json");
            std::remove("player_2.result.json");
            return HIT;
        case MISS:
            return MISS;
        case OUT_OF_BOUNDS:
            return OUT_OF_BOUNDS;
        default:
            throw ClientException("Bad hit");
    }

}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {

    vector<vector<int>> actVector;
    ifstream actfile("player_1.action_board.json");
    cereal::JSONInputArchive inarchive(actfile);
    inarchive(actVector);
    actVector[x][y] = result;

    ofstream outfile("player_1.action_board.json");
    cereal::JSONOutputArchive outArchive(outfile);
    outArchive(cereal::make_nvp("board", actVector));

}


string Client::render_action_board(){

    vector<vector<int>> actVector;
    ifstream actfile("player_1.action_board.json");
    cereal::JSONInputArchive inarchive(actfile);
    inarchive(actVector);
    printf("\n\nhello\n\n");
    int result;
    for(int i=0; i<10; i++) {
        for (int j=0; j<10; j++) {
            result = actVector[i][j];
            cout << result;
        }

    }

}