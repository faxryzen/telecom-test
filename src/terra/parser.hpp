#ifndef PARSER_HPP
#define PARSER_HPP

#include "common-types.hpp"

#include <iostream>
#include <vector>
#include <string>

namespace terra
{
  struct ParseAnswer
  {
    int n = 0;
    int food = 0;
    std::vector< terra::room_t > rooms;
    int double_res;
  };
  bool parseOptions(std::istream & input, ParseAnswer & result, std::string & error_line);
}

#endif
