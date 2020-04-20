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
#include "Socket.hpp"

#include <thread>
#include <string>
#include <semaphore.h>



// set up a global boolean variable to stop the threads
bool stop_threads = false;
// set up global semaphores to let server threads alternate between players when processing shots
sem_t player_sem[2];

/**
 * Create a server object that processes player shots
 * Threads take turns based on player_sem passed int process shot
 * @param sock - a socket connected to a player
 */
void run_server(ConnectionSocket *socket){
   Server s;
   s.initialize(BOARD_SIZE, socket, "player_1.setup_board.txt", "player_2.setup_board.txt");

   // run the loop until stop_threads is switched to true
   while(!stop_threads){
      if(s.shot_available())
         s.process_shot(player_sem);
   }

   //disconnect the socket
   delete socket;
   socket = nullptr;
}


int main(){
   cout << "Battleship server" << endl;

   // set up the semaphores, such that player 1 gets to start first
   sem_init(&player_sem[0], 0, 1);
   sem_init(&player_sem[1], 0, 0);

   // open a server socket
   ServerSocket server_socket(SERVER_PORT, 2);

   // connect with players and start server threads
   std::thread server_threads[2];
   for(int i=0; i<2; i++){
      ConnectionSocket *connection_socket = server_socket.accept();
      server_threads[i] = std::thread(run_server, connection_socket);
   }

   cout << "Press enter to stop server threads" << endl;
   cin.get(); // will wait until enter

   // stop the threads and wait for them to join
   stop_threads = true;
   for(int i=0; i<2; i++)
      server_threads[i].join();
   cout << "Server threads joined. Exiting." << endl;
}
