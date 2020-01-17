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

#include <chrono>
#include <thread>


int main(){
    cout << "Starting Battleship server" << endl;

    Server s;
    s.initialize(BOARD_SIZE, "player_1.setup_board.txt", "player_2.setup_board.txt");

    // run the server process in a loop
    while(true){
        while(s.process_shot(1) == NO_SHOT_FILE) {
            cout << "Waiting for shot from player 1" << endl;
            this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
        cout << "Processed shot from player 1" << endl;
        while(s.process_shot(2) == NO_SHOT_FILE) {
            cout << "Waiting for shot from player 2" << endl;
            this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
        cout << "Processed shot from player 2" << endl;
    }

}
