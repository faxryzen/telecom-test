#include <boost/test/unit_test.hpp>

#include <sstream>

#include "parser.hpp"

BOOST_AUTO_TEST_CASE(simple_parse)
{
  std::string input_data = R"(2
0 1 0 0 0 0
1 0,2 5 3 1 10
2 1 0 2 0 5
30 gems)";

  std::istringstream iss(input_data);
  terra::ParseAnswer result;
  std::string error_line;

  bool good = terra::parseOptions(iss, result, error_line);
  BOOST_CHECK(good);

  std::cout << "test simple_parse: " << (error_line == "" ? "good" : error_line) << '\n';

  BOOST_CHECK_EQUAL(result.n, 2);
  BOOST_CHECK_EQUAL(result.food, 30);
  BOOST_CHECK_EQUAL(result.double_res, 2);
  BOOST_CHECK_EQUAL(result.rooms.size(), 3);
  BOOST_CHECK_EQUAL(result.rooms[1].res[0], 5);
}

BOOST_AUTO_TEST_CASE(simple_parse2)
{
  std::string input_data = R"(5
0 1,2 0 0 0 0
1 0,3 5 2 1 15
2 0,4 3 2 1 10
3 1,4 1 0 2 40
4 2,5 2 4 0 15
5 4 0 5 4 10
6 gems)";

  std::istringstream iss(input_data);
  terra::ParseAnswer result;
  std::string error_line;

  bool good = terra::parseOptions(iss, result, error_line);
  BOOST_CHECK(good);

  std::cout << "test simple_parse: " << (error_line == "" ? "good" : error_line) << '\n';

  BOOST_CHECK_EQUAL(result.n, 5);
  BOOST_CHECK_EQUAL(result.food, 6);
  BOOST_CHECK_EQUAL(result.double_res, 2);
  BOOST_CHECK_EQUAL(result.rooms.size(), 6);
  BOOST_CHECK_EQUAL(result.rooms[0].id, 0);
  BOOST_CHECK_EQUAL(result.rooms[0].adj[0], 1);
  BOOST_CHECK_EQUAL(result.rooms[0].adj[1], 2);
}

BOOST_AUTO_TEST_CASE(simple_parse3)
{
  std::string input_data = R"(5 
0 1,2 
1 0,3 5 2 1 15 
2 0,4 3 2 1 10 
3 1,4 1 0 2 40 
4 2|5 2 4 0 15 
5 4 0 5 4 10 
6 gems )";

  std::istringstream iss(input_data);
  terra::ParseAnswer result;
  std::string error_line;

  bool good = terra::parseOptions(iss, result, error_line);
  BOOST_CHECK(!good);
  BOOST_TEST(error_line == "0 1,2");
}

BOOST_AUTO_TEST_CASE(simple_parse4)
{
  std::string input_data = R"(5 
0 1,2 0 0 0 0
1 0,3 5 2 1 15 
2 0,4 3 2 1 10 
3 1,4 1 0 2 40 
4 2|5 2 4 0 15 
5 4 0 5 4 10 
6 gems )";

  std::istringstream iss(input_data);
  terra::ParseAnswer result;
  std::string error_line;

  bool good = terra::parseOptions(iss, result, error_line);
  BOOST_CHECK(!good);
  BOOST_TEST(error_line == "4 2|5 2 4 0 15");
}

BOOST_AUTO_TEST_CASE(empty_line)
{
  std::string input_data = R"(5 
0 1,2 0 0 0 0
1 0,3 5 2 1 15 
2 0,4 3 2 1 10 

3 1,4 1 0 2 40 
4 2|5 2 4 0 15 
5 4 0 5 4 10 
6 gems )";

  std::istringstream iss(input_data);
  terra::ParseAnswer result;
  std::string error_line;

  bool good = terra::parseOptions(iss, result, error_line);
  BOOST_CHECK(!good);
  BOOST_TEST(error_line == "empty line");
}

BOOST_AUTO_TEST_CASE(invalid_n)
{
  std::string input_data = "256\n";

  std::istringstream iss(input_data);
  terra::ParseAnswer result;
  std::string error_line;

  bool good = terra::parseOptions(iss, result, error_line);

  BOOST_CHECK(!good);
  BOOST_CHECK(!error_line.empty());
}

BOOST_AUTO_TEST_CASE(missing_data)
{
  std::string input_data = R"(2
0 1 0 0 0 0
1 0,2 5 3 1 10)";

  std::istringstream iss(input_data);
  terra::ParseAnswer result;
  std::string error_line;

  bool good = terra::parseOptions(iss, result, error_line);

  BOOST_CHECK(!good);
}
