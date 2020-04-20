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
#include <string>


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}


void Server::initialize(unsigned int board_size, string p1_setup_board, string p2_setup_board){
    Server::board_size = board_size;
    int size_o_board = board_size;

    if (board_size != BOARD_SIZE)
        throw new ServerException("Wrong board size");

    if(p1_setup_board.length() == 0 && p2_setup_board.length() == 0)
        throw new ServerException("Bad Files");
//
//    vector<vector<char>> s_vect_bd1(board_size,vector<char>(board_size));
//    vector<vector<char>> s_vect_bd2(board_size,vector<char>(board_size));
//
//    string player_board1;
//    string player_board2;
//
//    player_board1 = "player_1.setup_board.txt";
//    player_board2 = "player_2.setup_board.txt";
//
//    ifstream in1;
//    in1.open(player_board1);
//
//    if(in1.is_open()){
//        while (!in1.eof()) {
//            for (int row = 0; row < board_size; row++) {
//                for (int col = 0; col < board_size; col++) {
//                    in1 >> s_vect_bd1[row][col];
//                }
//            }
//        }
//    }
//    in1.close();
//
//    ifstream in2;
//    in2.open(player_board2);
//
//    if(in2.is_open()){
//        while (!in2.eof()) {
//            for (int row = 0; row < board_size; row++) {
//                for (int col = 0; col < board_size; col++) {
//                    in2 >> s_vect_bd2[row][col];
//                }
//            }
//        }
//    }
//    in2.close();
}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if(player > 2 || player < 1){
        throw new ServerException("NOPE!");
    }
    if(x > BOARD_SIZE){
        return OUT_OF_BOUNDS;
    }
    if(y > BOARD_SIZE){
        return OUT_OF_BOUNDS;
    }
    string player_board;
    if(player == 1){
        player_board = "player_2.setup_board.txt";
    }else{
        player_board = "player_1.setup_board.txt";
    }

    ifstream in;

    char temp;
    in.open(player_board);
        if(in.is_open()){
            while (!in.eof()) {
                for (int row = 0; row < board_size; row++) {
                    for (int col = 0; col < board_size; col++) {
                        in >> temp;
                        if(x == row && y == col){
                            if(temp == '_'){
                                return MISS;
                            }else{
                                return HIT;
                            }
                    }
                }
            }
        }

    }
    in.close();


}


int Server::process_shot(unsigned int player) {

    if(player > 2 || player < 1){
        throw new ServerException("NOPE!");
    }
    string player_board;
    if (player == 1) {
        player_board = "player_1.shot.json";
    } else if(player == 2) {
        player_board = "player_2.shot.json";
    }else{
        return NO_SHOT_FILE;
    }

    unsigned int x;
    unsigned int y;

    ifstream shot_info(player_board);

    cereal::JSONInputArchive read(shot_info);
    read(x,y);
    shot_info.close();


    int res = evaluate_shot(player, x, y);

    string res_file;

    if(player == 1){
        res_file = "player_1.result.json";
    }else if(player == 2){
        res_file = "player_2.result.json";
    }

    ofstream ofres(res_file);

    cereal::JSONOutputArchive out_res(ofres);
    out_res(cereal::make_nvp("result", res));
    ofres.flush();

    if (player == 1) {
        remove("player_1.shot.json");
    } else if(player == 2) {
        remove("player_2.shot.json");
    }
    return SHOT_FILE_PROCESSED;
}