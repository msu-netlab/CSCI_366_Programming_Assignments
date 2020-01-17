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

#include <chrono>
#include <thread>

int main()
{
    printf("Starting Battleship client\n");

    // initialize the client with a correct player number
    Client c;
    while(! c.initialized){
        try{
            cout << "Enter player number: ";
            int player;
            cin >> player;
            c.initialize(player, BOARD_SIZE);
        } catch (ClientWrongPlayerNumberException& e){
            cout << e.what() << '\n';
        }

    }

    // keep firing and updating the action board with results from the server
    unsigned int x, y;
    while(true){
        // get coordinates
        cout << "Enter fire x position: ";
        cin >> x;
        cout << "Enter fire y position: ";
        cin >> y;

        // fire and get result
        c.fire(x, y);
        while(! c.result_available()) {
            cout << "Waiting for result" << endl;
            this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
        int result = c.get_result();
        printf("Shot at (%d, %d) result %d\n", x, y, result);
        if(result == OUT_OF_BOUNDS){
            cout << "Entered coordinates are out of bounds" << "\n";
            continue;
        }

        // update the action board
        c.update_action_board(result, x, y);
        cout << c.render_action_board();
    }

    return 0;
}
