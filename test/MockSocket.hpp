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

#ifndef BATTLESHIP_MOCKSOCKET_HPP
#define BATTLESHIP_MOCKSOCKET_HPP


#include "gmock/gmock.h"

#include "Socket.hpp"

class MockConnectionSocket : public ConnectionSocket {
public:
   MockConnectionSocket(unsigned int socket) : ConnectionSocket(socket) {};

   MOCK_METHOD(void, send, (string msg), (override));
   MOCK_METHOD(bool, data_available, (), (override));
   MOCK_METHOD(istringstream *, receive, (), (override));
};

#endif //BATTLESHIP_MOCKSOCKET_HPP
