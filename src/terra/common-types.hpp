#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <vector>
#include <array>

namespace terra
{
  constexpr int MAX_RESOURCES = 4;
  constexpr int MAX_N = 256;

  using res_value = std::array< int, MAX_RESOURCES >;

  struct room_t
  {
    int id = -1;
    std::vector< int > adj;
    res_value res; // <iron|gold|gems|exp>
  };
}

#endif
