#include "simulator.hpp"

#include <string>
#include <algorithm>

void Simulator::addVisitedRoom(int id)
{
  auto & first = data_[id];
  first.what = what_room_t::status::VISITED;

  if (first.id == -1)
  {
    first.id = dung_.getRoom(id).id;
    first.adj = dung_.getRoom(id).adj;
    first.res = dung_.getRoom(id).res;
  }

  for (auto & i: first.adj)
  {
    auto & second = data_[i];
    if (second.what < what_room_t::status::VISIBLE)
    {
      second.what = what_room_t::status::VISIBLE;
    }
    if (second.id == -1)
    {
      second.id = i;
      second.adj = dung_.getRoom(i).adj;
      second.res = dung_.getRoom(i).res;
    }
    for (auto & j: second.adj)
    {
      auto & third = data_[j];
      if (third.what < what_room_t::status::DISCRETE)
      {
        third.what = what_room_t::status::DISCRETE;
      }
      if (third.id == -1)
      {
        third.id = j;
        third.adj = dung_.getRoom(j).adj;
        third.res = dung_.getRoom(j).res;
      }
    }
  }
}
bool Simulator::isCollectingFreeHere() const
{
  auto & current = data_[pos_];
  return std::all_of(current.was_collected.begin(), current.was_collected.end(), [](int n)
  {
    return n == 0;
  });
}

void Simulator::go(int next, std::ostream & output)
{
  auto it = std::find(data_[pos_].adj.begin(), data_[pos_].adj.end(), next);
  if (it == data_[pos_].adj.end())
  {
    throw std::runtime_error("go bot: cant go from " + std::to_string(pos_) + " to " + std::to_string(next));
  }
  if (food_ == 0)
  {
    throw std::runtime_error("go bot: ran out of food");
  }
  pos_ = next;
  if (data_[pos_].what != what_room_t::status::VISITED)
  {
    addVisitedRoom(pos_);
  }
  --food_;
  output << "go " << pos_ << '\n';
}

void Simulator::printState(std::ostream & output) const
{
  auto & current = data_[pos_];
  output << "state " << pos_;
  for (size_t i = 0; i < 4; ++i)
  {
    output << " " << (current.was_collected[i] ? "_" : std::to_string(current.res[i]));
  }
  output << "\n";
}
void Simulator::printResult(std::ostream & output) const
{
  output << "result";
  int sum = 0;
  for (size_t i = 0; i < 4; ++i)
  {
    output << " " << inventory_[i];
    sum += inventory_[i] * actual_price_[i];
  }
  output << " " << sum << "\n";
}
void Simulator::printResource(int res, std::ostream & output) const
{
  switch (res)
  {
  case 0:
    output << "iron";
    break;
  case 1:
    output << "gold";
    break;
  case 2:
    output << "gems";
    break;
  case 3:
    output << "exp";
    break;
  default:
    break;
  }
}

int Simulator::getPos() const noexcept
{
  return pos_;
}
int Simulator::getFood() const noexcept
{
  return food_;
}
std::vector< int > Simulator::getNeighbors(int room) const
{
  auto adj = data_[room].adj;
  if (!isVisited(room) && !isVisible(room))
  {
    throw std::runtime_error("getNeighbors bot: not enough knowledge about room");
  }
  std::sort(adj.begin(), adj.end());
  return adj;
}
bool Simulator::hasResources(int room) const
{
  auto & current = data_[room];
  if (!isVisited(room))
  {
    throw std::runtime_error("hasResources bot: room is not visited");
  }
  for (int r: current.res)
  {
    if (r > 0)
    {
      return true;
    }
  }
  return false;
}
terra::res_value Simulator::getResources(int room) const
{
  if (!isVisited(room))
  {
    throw std::runtime_error("hasResources bot: room is not visited");
  }
  return data_[room].res;
}
void Simulator::collectResource(int index, std::ostream & output)
{
  auto & current = data_[pos_];
  if (current.res[index] == 0)
  {
    throw std::runtime_error("collectResource bot: no such resource");
  }
  inventory_[index] += current.res[index];
  current.res[index] = 0;
  if (!isCollectingFreeHere())
  {
    if (food_ == 0)
    {
      throw std::runtime_error("collectResource bot: ran out of food");
    }
    --food_;
  }
  current.was_collected[index] = 1;

  output << "collect ";
  printResource(index, output);
  output << '\n';
}
