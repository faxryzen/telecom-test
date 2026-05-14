#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <array>

#include "common-types.hpp"
#include "dungeon.hpp"

class Simulator
{
private:
  static constexpr int IRON_PRICE = 7;
  static constexpr int GOLD_PRICE = 11;
  static constexpr int GEMS_PRICE = 23;
  static constexpr int EXP_PRICE = 1;
  static constexpr int NEEDFUL_RES_MULT = 2;

  terra::Dungeon dung_;
  int food_;
  int dres_;
  int pos_;
  terra::res_value inventory_;

  struct what_room_t
  {
    int id = -1;
    enum class status
    {
      UNKNOWN,
      DISCRETE,
      VISIBLE,
      VISITED
    };
    status what;
    std::vector< int > adj;
    terra::res_value res;
    terra::res_value was_collected;
  };
  std::vector< what_room_t > data_;
  terra::res_value actual_price_;

  void addVisitedRoom(int id);
  bool isCollectingFreeHere() const;
protected:
  void printState(std::ostream & output) const;
  void printResult(std::ostream & output) const;
  void printResource(int res, std::ostream & output) const;

  void go(int next, std::ostream & output);

  int getPos() const noexcept;
  int getFood() const noexcept;
  terra::res_value getPrices() const noexcept
  {
    return actual_price_;
  }
  int getMaxPossibleRooms() const noexcept
  {
    return terra::MAX_N;
  }

  bool isVisited(int room) const
  {
    return data_[room].what == what_room_t::status::VISITED;
  }
  bool isVisible(int room) const
  {
    return data_[room].what == what_room_t::status::VISIBLE;
  }
  bool isKnown(int room) const
  {
    return data_[room].what == what_room_t::status::DISCRETE;
  }
  bool isUnknown(int room) const
  {
    return data_[room].what == what_room_t::status::UNKNOWN;
  }

  std::vector< int > getNeighbors(int room) const;

  int getNeedfulResource() const noexcept
  {
    return dres_;
  }
  bool hasResources(int room) const;
  bool hasResources() const
  {
    return hasResources(pos_);
  }
  terra::res_value getResources(int room) const;
  terra::res_value getResources() const
  {
    return getResources(pos_);
  }
  void collectResource(int index, std::ostream & output);

public:
  Simulator(terra::Dungeon dungeon, int food, int needful_resource):
    dung_(dungeon),
    food_(food),
    dres_(needful_resource),
    pos_(0),
    actual_price_({IRON_PRICE, GOLD_PRICE, GEMS_PRICE, EXP_PRICE})
  {
    inventory_.fill(0);
    data_.resize(getMaxPossibleRooms());
    actual_price_[dres_] *= NEEDFUL_RES_MULT;
    addVisitedRoom(0);
  }

  virtual ~Simulator() = default;
  virtual void runBot(std::ostream & output) = 0;
};

#endif
