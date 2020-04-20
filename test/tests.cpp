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

#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include <string>
#include <fstream>
#include <streambuf>

#include "common.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "MockSocket.hpp"

using ::testing::Return;

/**
 * Compute the diff distance between two files using the dtl library
 * @param file1 - name of the first file
 * @param file2 - name of the second file
 * @return distance between the contents of two files
 */
#include <dtl.hpp> // a library for diffing strings
long get_diff_dist(string file1, string file2){
   // load the file contents
   ifstream ifstr1(file1);
   string file1_S((std::istreambuf_iterator<char>(ifstr1)),
                  std::istreambuf_iterator<char>());
   ifstr1.close();
   ifstream ifstr2(file2);
   string file2_S((std::istreambuf_iterator<char>(ifstr2)),
                  std::istreambuf_iterator<char>());
   ifstr2.close();

   // compute the distance between correct and constructed boards
   dtl::Diff<char, string> d(file1_S, file2_S);
   d.onOnlyEditDistance();
   d.compose();

   return d.getEditDistance();
}


TEST(BitArray2DCreate, Correct_1by1){
   BitArray2D *array;
   ASSERT_NO_THROW(array = new BitArray2D(1,1));
}

TEST(BitArray2DCreate, Correct_10by10){
   BitArray2D *array;
   ASSERT_NO_THROW(array = new BitArray2D(10,10));
}

TEST(BitArray2DCreate, Incorrect){
   BitArray2D *array;
   ASSERT_ANY_THROW(array = new BitArray2D(0,0));
}




class BitArray2DSetGet : public ::testing::Test{
protected:
   BitArray2D *array;

   void SetUp() override {
      array = new BitArray2D(10, 10);
   };

   void TearDown() override{
      delete array;
   }

};

TEST_F(BitArray2DSetGet, Zero){
   int row=0, col=0;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, One){
   int row=0, col=1;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Seven){
   int row=0, col=7;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Eight){
   int row=0, col=8;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Nine){
   int row=0, col=9;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Ten){
   int row=9, col=1;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Hundred){
   int row=9, col=9;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Get_Out_Of_Bounds_High){
   ASSERT_ANY_THROW(array->get(BOARD_SIZE, BOARD_SIZE));
}

TEST_F(BitArray2DSetGet, Get_Out_Of_Bounds_Low){
   ASSERT_ANY_THROW(array->get(-1, -1));
}

TEST_F(BitArray2DSetGet, Set_Out_Of_Bounds_High){
   ASSERT_ANY_THROW(array->set(BOARD_SIZE, BOARD_SIZE));
}

TEST_F(BitArray2DSetGet, Set_Out_Of_Bounds_Low){
   ASSERT_ANY_THROW(array->set(-1, -1));
}

class ServerInitialize : public ::testing::Test{
protected:
   Server srv;
   ConnectionSocket *socket;

   void SetUp() override {
      socket = new MockConnectionSocket(0);
   }

   void TearDown() override {
      delete socket;
   }
};

TEST_F(ServerInitialize, Correct_Board_Size){
   ASSERT_NO_THROW(srv.initialize(BOARD_SIZE, socket, "player_1.setup_board.txt", "player_2.setup_board.txt"));
}

TEST_F(ServerInitialize, Wrong_Board_Size){
   ASSERT_ANY_THROW(srv.initialize(BOARD_SIZE-1, socket, "player_1.setup_board.txt", "player_2.setup_board.txt"));
}

TEST_F(ServerInitialize, Bad_File_Name){
   ASSERT_ANY_THROW(srv.initialize(BOARD_SIZE, socket, "", ""));
}


class ServerEvaluateShot : public ::testing::Test{
protected:
   Server srv;
   MockConnectionSocket *socket;
   void SetUp() override{
      socket = new MockConnectionSocket(0);
      srv.initialize(BOARD_SIZE, socket, "player_1.setup_board.txt", "player_2.setup_board.txt");
   }
};

TEST_F(ServerEvaluateShot, Hit_Detected){
   ASSERT_EQ(HIT, srv.evaluate_shot(1,9,0));
}

TEST_F(ServerEvaluateShot, Miss_Detected){
   ASSERT_EQ(MISS, srv.evaluate_shot(1,9,1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_X_High){
   ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,srv.board_size+1,1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_X_Low){
   ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,-1,1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_Y_High){
   ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,1,srv.board_size+1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_Y_Low){
   ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,1,-1));
}

TEST_F(ServerEvaluateShot, Max_In_Bounds){
   ASSERT_NO_THROW(srv.evaluate_shot(1,srv.board_size-1,srv.board_size-1));
}

TEST_F(ServerEvaluateShot, Bad_Player_Number_Low){
   ASSERT_ANY_THROW(srv.evaluate_shot(0,0,0));
}

TEST_F(ServerEvaluateShot, Bad_Player_Number_High){
   ASSERT_ANY_THROW(srv.evaluate_shot(MAX_PLAYERS+1,0,0));
}


class ServerProcessShot : public ::testing::Test{
protected:
   Server srv;
   MockConnectionSocket *socket;
   sem_t player_sem[2];

   void SetUp() override{
      socket = new MockConnectionSocket(0);

      sem_init(&player_sem[0], 0, 1);
      sem_init(&player_sem[1], 0, 0);
      srv.initialize(BOARD_SIZE, socket, "player_1.setup_board.txt", "player_2.setup_board.txt");
   }

   void TearDown() override{
      delete socket;
   }
};


TEST_F(ServerProcessShot, Hit_Detected){
   //set up shot and result
   istringstream *shot = new istringstream(R"({"player": 1, "x": 0, "y": 1})");
   string result("{\n    \"result\": 1\n}");

   //set up call expectations in process_shot
   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(shot));

   EXPECT_CALL(*socket, send(result))
           .Times(1)
           .WillOnce(Return());

   // invoke the function we want to test
   EXPECT_NO_FATAL_FAILURE(srv.process_shot(player_sem));
}

TEST_F(ServerProcessShot, Miss_Detected){
   istringstream *shot = new istringstream(R"({"player": 1, "x": 1, "y": 1})");
   string result("{\n    \"result\": -1\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(shot));

   EXPECT_CALL(*socket, send(result))
           .Times(1)
           .WillOnce(Return());

   EXPECT_NO_FATAL_FAILURE(srv.process_shot(player_sem));
}

TEST_F(ServerProcessShot, Out_Of_Bounds_X){
   istringstream *shot = new istringstream(R"({"player": 1, "x": 10, "y": 1})");
   string result("{\n    \"result\": 0\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(shot));

   EXPECT_CALL(*socket, send(result))
           .Times(1)
           .WillOnce(Return());

   EXPECT_NO_FATAL_FAILURE(srv.process_shot(player_sem));
}

TEST_F(ServerProcessShot, Out_Of_Bounds_Y){
   istringstream *shot = new istringstream(R"({"player": 1, "x": 1, "y": 10})");
   string result("{\n    \"result\": 0\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(shot));

   EXPECT_CALL(*socket, send(result))
           .Times(1)
           .WillOnce(Return());

   EXPECT_NO_FATAL_FAILURE(srv.process_shot(player_sem));
}

TEST_F(ServerProcessShot, Max_In_Bounds){
   istringstream *shot = new istringstream(R"({"player": 1, "x": 9, "y": 9})");
   string result("{\n    \"result\": -1\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(shot));

   EXPECT_CALL(*socket, send(result))
           .Times(1)
           .WillOnce(Return());

   EXPECT_NO_FATAL_FAILURE(srv.process_shot(player_sem));
}

TEST_F(ServerProcessShot, Bad_Player_Number_Low){
   istringstream *shot = new istringstream(R"({"player": 0, "x": 9, "y": 9})");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(shot));

   ASSERT_ANY_THROW(srv.process_shot(player_sem));
}

TEST_F(ServerProcessShot, Bad_Player_Number_Low_High){
   istringstream *shot = new istringstream(R"({"player": 3, "x": 9, "y": 9})");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(shot));

   ASSERT_ANY_THROW(srv.process_shot(player_sem));
}


class ServerShotAvailable : public ::testing::Test{
protected:
   Server srv;
   MockConnectionSocket *socket;

   void SetUp() override{
      socket = new MockConnectionSocket(0);

      srv.initialize(BOARD_SIZE, socket, "player_1.setup_board.txt", "player_2.setup_board.txt");
   }

   void TearDown() override{
      delete socket;
   }
};

TEST_F(ServerShotAvailable, ShotUnavailable){
   EXPECT_CALL(*socket, data_available())
           .Times(1)
           .WillOnce(Return(false));
   ASSERT_FALSE(srv.shot_available());
}

TEST_F(ServerShotAvailable, ShotAvailable){
   EXPECT_CALL(*socket, data_available())
           .Times(1)
           .WillOnce(Return(true));

   ASSERT_TRUE(srv.shot_available());
}


class ClientInitialize : public ::testing::Test{
protected:
   Client client;
   ConnectionSocket *socket;

   void SetUp() override{
      socket = new MockConnectionSocket(0);
      client.initialize(1, socket, BOARD_SIZE);
   }

   void TearDown() override{
      delete socket;
      remove("player_1.action_board.json");
   }
};

TEST_F(ClientInitialize, Creates_Action_Board){
   ASSERT_EQ(0, get_diff_dist("player_1.action_board.json", "correct_start_action_board.json"));
}


class ClientFire : public ::testing::Test{
protected:
   Client client;
   MockConnectionSocket *socket;

   void SetUp() override{
      socket = new MockConnectionSocket(0);
      client.initialize(1, socket, BOARD_SIZE);
   }

   void TearDown() override{
      delete socket;
      remove("player_1.action_board.json");
   }
};

TEST_F(ClientFire, Creates_Fire_Message){
   string shot("{\n    \"player\": 1,\n    \"x\": 0,\n    \"y\": 1\n}");

   EXPECT_CALL(*socket, send(shot))
           .Times(1)
           .WillOnce(Return());

   EXPECT_NO_FATAL_FAILURE(client.fire(0,1));
}

class ClientResultAvailable : public ::testing::Test{
protected:
   Client client;
   MockConnectionSocket *socket;

   void SetUp() override{
      socket = new MockConnectionSocket(0);
      client.initialize(1, socket, BOARD_SIZE);
   }

   void TearDown() override{
      delete socket;
      remove("player_1.action_board.json");
   }
};

TEST_F(ClientResultAvailable, ResultUnavailable){
   EXPECT_CALL(*socket, data_available())
           .Times(1)
           .WillOnce(Return(false));
   ASSERT_FALSE(client.result_available());
}

TEST_F(ClientResultAvailable, ResultAvailable){
   EXPECT_CALL(*socket, data_available())
           .Times(1)
           .WillOnce(Return(true));

   ASSERT_TRUE(client.result_available());
}

class ClientGetResult : public ::testing::Test{
protected:
   Client client;
   MockConnectionSocket *socket;

   void SetUp() override{
      socket = new MockConnectionSocket(0);
      client.initialize(1, socket, BOARD_SIZE);
   }

   void TearDown() override{
      delete socket;
      remove("player_1.action_board.json");
   }
};

TEST_F(ClientGetResult, Return_Hit){
   istringstream *result = new istringstream("{\n    \"result\": 1\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(result));

   ASSERT_EQ(HIT, client.get_result());
}

TEST_F(ClientGetResult, Return_Miss){
   istringstream *result = new istringstream("{\n    \"result\": -1\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(result));

   ASSERT_EQ(MISS, client.get_result());
}

TEST_F(ClientGetResult, Return_Out_Of_Bounds){
   istringstream *result = new istringstream("{\n    \"result\": 0\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(result));

   ASSERT_EQ(OUT_OF_BOUNDS, client.get_result());
}

TEST_F(ClientGetResult, Catch_Bad_Result){
   istringstream *result = new istringstream("{\n    \"result\": 9\n}");

   EXPECT_CALL(*socket, receive())
           .Times(1)
           .WillOnce(Return(result));

   ASSERT_ANY_THROW(client.get_result());
}

class ClientUpdateActionBoard : public ::testing::Test{
protected:
   Client client;
   MockConnectionSocket *socket;

   void SetUp() override{
      socket = new MockConnectionSocket(0);
      client.initialize(1, socket, BOARD_SIZE);
   }

   void TearDown() override{
      delete socket;
      remove("player_1.action_board.json");
   }
};

TEST_F(ClientUpdateActionBoard, Record_Hit){
   client.update_action_board(HIT, 0, 0);
   ASSERT_EQ(0, get_diff_dist("player_1.action_board.json", "correct_hit_action_board.json"));
}

TEST_F(ClientUpdateActionBoard, Record_Miss){
   client.update_action_board(MISS, 0, 0);
   ASSERT_EQ(0, get_diff_dist("player_1.action_board.json", "correct_miss_action_board.json"));
}



