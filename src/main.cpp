#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "parser.hpp"
#include "dungeon.hpp"

#include "simulator.hpp"
#include "alice-bot.hpp"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "format: ./output <dungeon_description>\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  std::ofstream output("result.txt");
  if (!input)
  {
    std::cerr << "invalid file\n";
    return 1;
  }

  terra::ParseAnswer result{};
  std::string where_is_error = "";
  
  if (!terra::parseOptions(input, result, where_is_error))
  {
    output << where_is_error << "\n";
    return 2;
  }

  terra::Dungeon dungeon(std::move(result.rooms));
  AliceBot sim(dungeon, result.food, result.double_res);
  sim.runBot(output);
}
