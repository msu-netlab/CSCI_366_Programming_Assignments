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
#include <string>
#include <cstring>
#include <bits/stdc++.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>


Client::~Client() {

}


void Client::initialize(unsigned int player, unsigned int board_size){
    string player_name;

    if(player == 1){
        player_name = "player_1.action_board.json";
    }else if(player == 2){
        player_name = "player_2.action_board.json";
    }

    vector<vector<int>> board(board_size, vector<int>(board_size, 0));
    ofstream ofp(player_name);
    if(ofp){
        {
            cereal::JSONOutputArchive jason(ofp);
            jason(cereal::make_nvp("board", board));
        }
        ofp.close();
        initialized = true;
    }

//    }



//    in.open(player_board);
//
//    if(in.is_open()){
//        while (!in.eof()) {
//            for (int row = 0; row < board_size; row++) {
//                for (int col = 0; col < board_size; col++) {
//                    in >> vect_bd[row][col];
//                }
//            }
//        }
//    }
//
//    for (int row = 0; row < vect_bd.size(); row++){
//        for (int col = 0; col < vect_bd[row].size(); col++){
//            cout << vect_bd[row][col];
//        }
//        cout << '\n';
//    }
//    in.close();
//    cout << render_action_board();


}


void Client::fire(unsigned int x, unsigned int y) {
    string player_board = "player_1.shot.json";

    int x_co = x;
    int y_co = y;

//    if (player == 1) {
//        player_board = "player_1.shot.json";
//    } else{
//        player_board = "player_2.shot.json";
//    }

    ofstream shot_f(player_board);

    cereal::JSONOutputArchive out_res(shot_f);
    out_res(cereal::make_nvp("x", x_co), cereal::make_nvp("y", y_co));
    shot_f.flush();






}


bool Client::result_available() {
    //1 is hit, -1 is miss, 0 is out of bounds

}


int Client::get_result() {
    string board = "player_1.result.json";
//    if(player == 1){
//        board = "player_1.result.json";
//    }else if(player == 2){
//        board = "player_2.result.json";
//    }
    int resul;
    ifstream ress_info(board);

    cereal::JSONInputArchive read(ress_info);
    read(resul);
    ress_info.close();

    remove("player_1.result.json");

    //    if (player == 1) {
//        remove("player_1.result.json");
//    } else if(player == 2) {
//        remove("player_2.result.json");
//    }

    if(resul == 1){
        return HIT;
    }else if(resul == -1){
        return MISS;
    }else if(resul == 0){
        return OUT_OF_BOUNDS;
    }else{
        throw new ClientException("That's a negative");
    }




}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    string board = "player_1.action_board.json";

    vector<vector<int>> uaboard(board_size, vector<int>(board_size, 0));

    ifstream in(board);

    cereal::JSONInputArchive upd(in);
    upd(uaboard);
    in.close();

    uaboard[x][y] = result;

    ofstream ouab(board);

    cereal::JSONOutputArchive out_uab(ouab);
    out_uab(cereal::make_nvp("board", uaboard));
    ouab.flush();

    //PLUNKBAT!
}


string Client::render_action_board(){
    vector<vector<char>> vect_bd(board_size,vector<char>(board_size));

    string output = "";
    string player_board;

    if(player == 1){
        player_board = "player_1.setup_board.txt";
    }else if(player == 2){
        player_board = "player_2.setup_board.txt";
    }

    ifstream in;
    in.open(player_board);

    if(in.is_open()){
        while (!in.eof()) {
            for (int row = 0; row < board_size; row++) {
                for (int col = 0; col < board_size; col++) {
                    in >> vect_bd[row][col];
                }
            }
        }
    }

    for (int row = 0; row < vect_bd.size(); row++){
        for (int col = 0; col < vect_bd[row].size(); col++){
            output += vect_bd[row][col];
        }
        output += '\n';
    }
    in.close();
    return output;
}