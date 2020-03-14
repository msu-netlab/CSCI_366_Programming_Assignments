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

    if(board_size != 10){
        throw(ServerException("error"));
    }
    if(p1_setup_board != "player_1.setup_board.txt" or p2_setup_board != "player_2.setup_board.txt"){
        throw(ServerException("error"));
    }
    this->board_size = board_size;
}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    char player2Array [10][10];
    if(player != 1 and player != 2){
        throw(ServerException("error"));
    }


    if( (x >= board_size) or (y >= board_size)) {
        return 0;
    }
    else{
        if(player == 1) {
            ifstream file{"player_2.setup_board.txt"};
            file.clear();
            file.seekg(0,ios::beg);

            if(!file.is_open()){
                return -2;
            }

            for(int i = 0; i < 10; i++){
                for(int j=0; j < 10; j++){
                    file >> player2Array[j][i];
                }
            }
            file.close();
            if(player2Array[x][y] == '_'){
                return -1;
            }
            else{
                return 1;
            }
        }
    }

}


int Server::process_shot(unsigned int player) {
    ifstream input;
    ofstream output;
    if(player == 1) {
        input.open("player_1.shot.json");
        output.open("player_1.result.json");
    }
    else{
        input.open("player_2.shot.json");
        output.open("player_2.result.json");
    }
    rapidjson::IStreamWrapper isw(input);
    rapidjson::Document d;
    d.ParseStream(isw);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    string jsonstr(buffer.GetString());
    int x;
    int y;
    if(jsonstr.at(6) == '0'){
        x = 10;
        if(jsonstr.size() != 14){
            y = 10;
        }
        else {
            y = jsonstr.at(12) - 48;

        }
    }
    else{
        x = jsonstr.at(5) - 48;
        if(jsonstr.size() != 13){
            y = 10;
        }
        else {
            y = jsonstr.at(11) - 48;
        }
    }

    int result = evaluate_shot(player, (x), (y));

    string result_str = "{\n"
                            "    \"result\": "+to_string(result)+"\n"
                                                                 "}";
    output << result_str;



    input.close();
    output.close();

    remove("player_1.shot.json");
    remove("player_2.shot.json");

    return NO_SHOT_FILE;
}
