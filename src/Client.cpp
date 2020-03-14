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
    this->board_size = board_size;
    this->player = player;
    string result_str = "{\n"
                        "    \"board\": [\n";
                        for(int i = 0; i < 9; i ++){
                            result_str += "        [\n";
                            for(int j = 0; j < 9; j ++){
                                result_str += "            " +to_string(0) + ",\n";
                            }
                            result_str += "            " +to_string(0) + "\n";
                            result_str += "        ],\n";
                        }
    result_str += "        [\n";
    for(int j = 0; j < 9; j ++){
        result_str += "            " +to_string(0) + ",\n";
    }
    result_str += "            " +to_string(0) + "\n";
    result_str += "        ]\n";
    result_str += "    ]\n";
    result_str += "}";
    if(player == 1) {
        ofstream result_file("player_1.action_board.json");
        result_file << result_str;
        result_file.close();
    }
    else{
        ofstream result_file("player_2.action_board.json");
        result_file << result_str;
        result_file.close();
    }
}


void Client::fire(unsigned int x, unsigned int y) {
    string result_str = "{\n"
                        "    \"x\": " + to_string(x) + ",\n"
                        "    \"y\": " + to_string(y) + "\n"
                                                       "}";
    ofstream result_file("player_1.shot.json");
    result_file << result_str;
    result_file.close();
}


bool Client::result_available() {
    ifstream Infield("player_1.result.json");
    return Infield.good();
}


int Client::get_result() {

    std::ifstream input("player_1.result.json");
    rapidjson::IStreamWrapper isw(input);
    rapidjson::Document d;
    d.ParseStream(isw);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    string jsonstr(buffer.GetString());

    input.close();
    remove("player_1.result.json");
    if(jsonstr[10] == '1'){
        return 1;
    }
    else if(jsonstr[10] == '-'){
        return -1;
    }
    else if(jsonstr[10] == '0'){
        return 0;
    }
    else{
        throw("error");
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    string result_str = "{\n"
                        "    \"board\": [\n";
    for(int i = 0; i < 9; i ++){
        result_str += "        [\n";
        for(int j = 0; j < 9; j ++){
            if(result == 1 and i == x and j == y){
                result_str += "            " +to_string(1) + ",\n";
            }
            else if(result == -1 and i == x and j == y ){
                result_str += "            " +to_string(-1) + ",\n";
            }
            else{
                result_str += "            " +to_string(0) + ",\n";
            }

        }
        result_str += "            " +to_string(0) + "\n";
        result_str += "        ],\n";
    }
    result_str += "        [\n";
    for(int j = 0; j < 9; j ++){
        result_str += "            " +to_string(0) + ",\n";
    }
    result_str += "            " +to_string(0) + "\n";
    result_str += "        ]\n";
    result_str += "    ]\n";
    result_str += "}";
    ofstream result_file("player_1.action_board.json");
    result_file << result_str;
    result_file.close();


}


string Client::render_action_board(){
}
