#include "alice-bot.hpp"

#include <algorithm>
#include <numeric>
#include <queue>
#include <map>

int AliceBot::whichIsValuable(int room) const
{
  auto values = getResources(room);
  auto prices = getPrices();
  for (size_t i = 0; i < 4; ++i)
  {
    values[i] = prices[i] * values[i];
  }
  auto it = std::max_element(values.begin(), values.end());
  return std::distance(values.begin(), it);
}

std::vector< int > AliceBot::getForwardPath()
{
  std::vector< int > visibles;
  for (int i: getNeighbors(getPos()))
  {
    if (isVisible(i))
    {
      visibles.push_back(i);
    }
  }
  if (!visibles.empty())
  {
    auto it = std::min_element(visibles.begin(), visibles.end());
    return {*it};
  }

  int max_n = getMaxPossibleRooms() - 1;
  std::vector< int > dist(max_n + 1, -1);
  std::vector< int > parent(max_n, -1);
  std::queue< int > q;
  int pos = getPos();
  dist[pos] = 0;
  q.push(pos);

  while (!q.empty())
  {
    int u = q.front();
    q.pop();
    if (!isVisited(u) && !isVisible(u))
    {
      continue;
    }
    for (int v: getNeighbors(u))
    {
      if (isUnknown(v))
      {
        continue;
      }
      if (dist[v] == -1)
      {
        dist[v] = dist[u] + 1;
        parent[v] = u;
        q.push(v);
      }
    }
  }
  int best_dist = std::numeric_limits< int >::max();
  int best_target = -1;
  for (int i = 0; i <= max_n; ++i)
  {
    if (isVisible(i) || isKnown(i))
    {
      if (dist[i] != -1 && dist[i] < best_dist)
      {
        best_dist = dist[i];
        best_target = i;
      }
      else if (dist[i] == best_dist && i < best_target)
      {
        best_target = i;
      }
    }
  }
  if (best_target == -1)
  {
    return {};
  }
  std::vector< int > path;
  int at = best_target;
  while (at != pos)
  {
    int best_prev = -1;
    if (!isVisited(at) && !isVisible(at))
    {
      continue;
    }
    for (int v: getNeighbors(at))
    {
      if (!isUnknown(v) && dist[v] == dist[at] - 1)
      {
        if (best_prev == -1 || v < best_prev)
        {
          best_prev = v;
        }
      }
    }
    path.push_back(at);
    at = best_prev;
  }
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector< int > AliceBot::getReturnPath()
{
  int max_n = getMaxPossibleRooms() - 1;
  std::vector< int > dist(max_n + 1, -1);
  std::vector< int > parent(max_n + 1, -1);
  std::queue< int > q;
  dist[0] = 0;
  q.push(0);
  while (!q.empty())
  {
    int u = q.front(); 
    q.pop();
    if (!isVisited(u) && !isVisible(u))
    {
      continue;
    }
    std::vector< int > neighbors = getNeighbors(u);
    std::sort(neighbors.begin(), neighbors.end());
    for (int v: neighbors)
    {
      if (!isVisited(v))
      {
        continue;
      }
      if (dist[v] == -1)
      {
        dist[v] = dist[u] + 1;
        parent[v] = u;
        q.push(v);
      }
      else if (dist[v] == dist[u] + 1 && u < parent[v])
      {
        parent[v] = u;
      }
    }
  }
  std::vector< int > path;
  int pos = getPos();
  if (dist[pos] == -1)
  {
    return path;
  }
  for (int at = parent[pos]; at != -1; at = parent[at])
  {
    path.push_back(at);
  }
  return path;
}

void AliceBot::runBot(std::ostream & output)
{
  int food = getFood() / 2;

  while (food < getFood())
  {
    auto path = getForwardPath();
    if (path.empty())
    {
      break;
    }

    for (int step: path)
    {
      go(step, output);
      printState(output);

      if (hasResources())
      {
        collectResource(whichIsValuable(getPos()), output);
        printState(output);
      }
    }
  }

  std::vector< int > path = getReturnPath();
  if (path.empty())
  {
    output << "player dead =C\n";
    return;
  }

  int extra = getFood() - path.size();
  for (int room: path)
  {
    while (extra > 0)
    {
      if (!hasResources())
      {
        break;
      }
      collectResource(whichIsValuable(getPos()), output);
      --extra;
      printState(output);
    }
    go(room, output);
    if (room != 0)
    {
      printState(output);
    }
  }
  printResult(output);
}
