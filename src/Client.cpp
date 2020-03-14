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

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    //Validate player numbers
    if (player < 1 || player > MAX_PLAYERS) {
        throw "Player number out of bounds!";
    } else {

        //Initialize player fields
        this->player = player;
        this->board_size = board_size;
        this->board_name = "player_" + to_string(player) + ".action_board.json";

        //Create empty 2d vector board
        vector<int> rows(this->board_size, 0);
        vector<vector<int>> board(this->board_size, rows);

        //Initialize output stream
        ofstream output;
        output.open(this->board_name, ofstream::out);

        //Create board json
        if (output) {
            {
                cereal::JSONOutputArchive archive(output);
                archive(CEREAL_NVP(board));
            }
            output.close(); //Close board json
            initialized = true;
        } else {
            throw "Couldn't open file!";
        }

    }

}


void Client::fire(unsigned int x, unsigned int y) {
    //Initialize file name
    string name = "player_" + to_string(this->player) + ".shot.json";

    //Initialize output stream
    ofstream output;
    output.open(name, ofstream::out);

    //Serialize shot
    if (output) {
        {
            cereal::JSONOutputArchive archive(output);
            archive(CEREAL_NVP(x), CEREAL_NVP(y));
        }
        output.close(); //Close shot file
    } else {
        throw "Shot file not found!";
    }

}


bool Client::result_available() {
    //Open result file
    string file = "player_" + to_string(this->player) + ".result.json";
    ifstream input;
    input.open(file, ifstream::in);

    //Assure the file opened
    if (input) {
        return true;
    } else {
        return false;
    }

}


int Client::get_result() {
    //Open result file
    std::string file("player_" + to_string(this->player) + ".result.json");
    const char* f = file.c_str(); //const char* for remove()
    ifstream input;
    input.open(file, ifstream::in);

    int result;

    //Read result file
    if (input) {
        cereal::JSONInputArchive archive_in(input);
        archive_in(result);
        input.close();
        std::remove(f);
    } else {
        throw "No result available!";
    }

    //Validate result
    if (result < -1 || result > 1) {
        throw "Bad result file!";
    } else {
        return result;
    }

}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    //Retrieve current action board
    ifstream input;
    input.open(this->board_name);

    //Create empty 2d vector
    vector<int> rows(this->board_size, 0);
    vector<vector<int>> board(this->board_size, rows);

    //Read action board
    if (input) {
        {
            cereal::JSONInputArchive archive(input);
            archive(board);
        }
        input.close();
    } else {
        throw "Couldn't open action board file!";
    }

    //Replace value at (x,y)
    board[y][x] = result;

    //Open action board
    ofstream output;
    output.open(this->board_name);

    //Overwrite the action board
    if (output) {
        {
            cereal::JSONOutputArchive archive(output);
            archive(CEREAL_NVP(board));
        }
        output.close();
    } else {
        throw "Couldn't open action board file";
    }

}


string Client::render_action_board(){
    //Retrieve action board
    ifstream input;
    input.open(this->board_name);

    //Create empty 2d vector
    vector<int> rows(this->board_size, 0);
    vector<vector<int>> board(this->board_size, rows);

    //Read action board
    if (input) {
        {
            cereal::JSONInputArchive archive(input);
            archive(board);
        }
        input.close();
    } else {
        throw "Couldn't open action board file!";
    }

    //Create string from vector
    string output;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
             output += to_string(board[i][j]) + " ";
        }
        output += "\n";
    }

    return output;

}