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
    file->seekg(0, file->end);
    int len = file->std::istream::tellg();
    return len;
}


void Server::initialize(unsigned int board_size,
                        const char *p1_setup_board,
                        string p2_setup_board) {
    //Validate parameters
    std::string p1Board(p1_setup_board);
    if (p1Board.empty() || p2_setup_board.empty()) {
        throw "Bad file name!";
    }

    //Retrieve setup boards
    string lineP1; //First line of p1_setup_board
    ifstream inputP1;
    inputP1.open(p1_setup_board, ifstream::in);
    getline(inputP1, lineP1);
    inputP1.close();

    string lineP2; //First line of p2_setup_board
    ifstream inputP2;
    inputP2.open(p2_setup_board, ifstream::in);
    getline(inputP2, lineP2);
    inputP2.close();

    //Validate board sizes
    if (lineP1.length() > board_size || lineP1.length() < board_size) {
        throw "Wrong board size for player 1";
    }
    if (lineP2.length() > board_size || lineP2.length() < board_size) {
        throw "Wrong board size for player 2";
    }

}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    //Validate player number
    if (player < 1 || player > MAX_PLAYERS) {
        throw "Player number out of bounds!";
    }

    //Validate coordinates
    if ((x+1) < 1 || (x+1) > board_size) {
        return OUT_OF_BOUNDS;
    } if ((y+1) < 1 || (y+1) > board_size) {
        return OUT_OF_BOUNDS;
    }

    //Open opposite player setup board
    ifstream board;
    if (player == 1) {
        board.open("player_2.setup_board.txt", ifstream::in);
    } else if (player == 2) {
        board.open("player_1.setup_board.txt", ifstream::in);
    } else {
        throw "Bad player number!";
    }

    //Store board information in a vector
    vector<string> lines(board_size, "");
    string curLine;
    int i = 0;
    while (getline(board, curLine)) {
        lines[i] = curLine;
        i++;
    }
    board.close(); //Close board file

    //Read the board space
    char space = lines[y].at(x);

    //Check the result of the shot
    if (space == 'C' || space == 'B' || space == 'R' || space == 'S' || space == 'D') {
        return HIT;
    } else {
        return MISS;
    }

}


int Server::process_shot(unsigned int player) {
    //Validate player numbers
    if (player < 1 || player > MAX_PLAYERS) {
        throw "Player number out of bounds!";
    }

    //Open shot file
    std::string file("player_" + to_string(player) + ".shot.json");
    const char* f = file.c_str(); //const char* for remove()
    ifstream input;
    input.open(file, ifstream::in);

    //Read shot file
    unsigned int x, y;
    int result = 3;
    if (input) {
        cereal::JSONInputArchive archive_in(input);
        archive_in(x, y);
        input.close(); //Close shot file
        std::remove(f);
        result = evaluate_shot(player, x, y);
    } else {
        return NO_SHOT_FILE;
    }

    //Create result file
    string resultFile = "player_" + to_string(player) + ".result.json"; //File name
    ofstream output;
    output.open(resultFile, ofstream::out);

    //Serialize result
    if (output) {
        {
            cereal::JSONOutputArchive archive(output);
            archive(CEREAL_NVP(result));
        }
        output.close();
        return SHOT_FILE_PROCESSED;
    } else {
        throw "Result file not found";
    }

    return NO_SHOT_FILE;

}