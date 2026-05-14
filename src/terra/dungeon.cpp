#include "dungeon.hpp"

void terra::Dungeon::fixSymmetry()
{
  for (size_t i = 0; i < rooms_.size(); ++i)
  {
    auto & room = rooms_[i];
    for (int adj: room.adj)
    {
      auto & adj_room = rooms_[adj];
      if (std::find(adj_room.adj.begin(), adj_room.adj.end(), static_cast< int >(i)) == adj_room.adj.end())
      {
        adj_room.adj.push_back(i);
      }
    }
  }
}

terra::Dungeon::Dungeon(std::vector< room_t > rooms):
  rooms_(rooms)
{
  fixSymmetry();
}

const terra::room_t & terra::Dungeon::getRoom(int id) const
{
  return rooms_[id];
}
