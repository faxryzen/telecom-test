#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>
#include <algorithm>

#include "common-types.hpp"

namespace terra
{
  class Dungeon
  {
    std::vector< room_t > rooms_;

    void fixSymmetry();
  public:
    Dungeon(std::vector< room_t > rooms);

    const room_t & getRoom(int id) const;
  };
}

#endif
