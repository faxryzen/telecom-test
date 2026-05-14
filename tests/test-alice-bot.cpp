#include <boost/test/unit_test.hpp>

#include <sstream>

#include "alice-bot.hpp"

BOOST_AUTO_TEST_CASE(alice_bot_output1)
{
  std::vector< terra::room_t > rooms(6);
  rooms[0] = {0, {1, 2}, {0, 0, 0, 0}};
  rooms[1] = {1, {0, 3}, {5, 2, 1, 15}};
  rooms[2] = {2, {0, 4}, {3, 2, 1, 10}};
  rooms[3] = {3, {1, 4}, {1, 0, 2, 40}};
  rooms[4] = {4, {2, 5}, {2, 4, 0, 15}};
  rooms[5] = {5, {4},   {0, 5, 4, 10}};

  terra::Dungeon dungeon(rooms);
  AliceBot sim(dungeon, 6, 2);

  std::ostringstream oss;
  sim.runBot(oss);

  std::string expected = R"(go 1
state 1 5 2 1 15
collect gems
state 1 5 2 _ 15
go 3
state 3 1 0 2 40
collect gems
state 3 1 0 _ 40
go 4
state 4 2 4 0 15
collect gold
state 4 2 _ 0 15
go 3
state 3 1 0 _ 40
go 1
state 1 5 2 _ 15
go 0
result 0 4 3 0 182
)";

  BOOST_CHECK_EQUAL(oss.str(), expected);
}

BOOST_AUTO_TEST_CASE(alice_bot_output2)
{
  std::vector< terra::room_t > rooms(17);
  rooms[0] = {0, {1, 15, 16}, {0, 0, 0, 0}};
  rooms[1] = {1, {0, 2}, {3, 0, 0, 10}};
  rooms[2] = {2, {1, 3}, {6, 6, 6, 6}};
  rooms[3] = {3, {2, 4, 10}, {0, 2, 0, 0}};
  rooms[4] = {4, {3, 5}, {0, 0, 1, 0}};
  rooms[5] = {5, {4, 6}, {0, 0, 0, 0}};
  rooms[6] = {6, {5, 7}, {0, 0, 0, 0}};
  rooms[7] = {7, {6, 8}, {0, 0, 0, 0}};
  rooms[8] = {8, {7, 9, 11, 14}, {5, 0, 0, 0}};
  rooms[9] = {9, {8, 10}, {0, 0, 0, 5}};
  rooms[10] = {10, {3, 9}, {0, 0, 3, 0}};
  rooms[11] = {11, {8, 12}, {1, 1, 1, 1}};
  rooms[12] = {12, {11, 13}, {0, 10, 0, 0}};
  rooms[13] = {13, {12}, {0, 0, 0, 2}};
  rooms[14] = {14, {8, 15}, {0, 0, 0, 0}};
  rooms[15] = {15, {0, 14}, {0, 0, 0, 0}};
  rooms[16] = {16, {0}, {0, 0, 0, 0}};

  terra::Dungeon dungeon(rooms);
  AliceBot sim(dungeon, 30, 3);

  std::ostringstream oss;
  sim.runBot(oss);

  std::string expected = R"(go 1
state 1 3 0 0 10
collect iron
state 1 _ 0 0 10
go 2
state 2 6 6 6 6
collect gems
state 2 6 6 _ 6
go 3
state 3 0 2 0 0
collect gold
state 3 0 _ 0 0
go 4
state 4 0 0 1 0
collect gems
state 4 0 0 _ 0
go 5
state 5 0 0 0 0
go 6
state 6 0 0 0 0
go 7
state 7 0 0 0 0
go 8
state 8 5 0 0 0
collect iron
state 8 _ 0 0 0
go 9
state 9 0 0 0 5
collect exp
state 9 0 0 0 _
go 10
state 10 0 0 3 0
collect gems
state 10 0 0 _ 0
go 9
state 9 0 0 0 _
go 8
state 8 _ 0 0 0
go 11
state 11 1 1 1 1
collect gems
state 11 1 1 _ 1
go 12
state 12 0 10 0 0
collect gold
state 12 0 _ 0 0
go 13
state 13 0 0 0 2
collect exp
state 13 0 0 0 _
go 12
state 12 0 _ 0 0
go 11
state 11 1 1 _ 1
collect gold
state 11 1 _ _ 1
collect iron
state 11 _ _ _ 1
collect exp
state 11 _ _ _ _
go 8
state 8 _ 0 0 0
go 9
state 9 0 0 0 _
go 10
state 10 0 0 _ 0
go 3
state 3 0 _ 0 0
go 2
state 2 6 6 _ 6
collect gold
state 2 6 _ _ 6
collect iron
state 2 _ _ _ 6
collect exp
state 2 _ _ _ _
go 1
state 1 _ 0 0 10
go 0
result 15 19 11 14 595
)";

  BOOST_CHECK_EQUAL(oss.str(), expected);
}
