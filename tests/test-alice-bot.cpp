#include <boost/test/unit_test.hpp>

#include <sstream>

#include "alice-bot.hpp"

BOOST_AUTO_TEST_CASE(alice_bot_exact_output)
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
