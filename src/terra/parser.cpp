#include "parser.hpp"

#include <sstream>
#include <iostream>

namespace terra
{
  bool parseOptions(std::istream & input, ParseAnswer & result, std::string & error_line)
  {
    std::string current_line;
    if (!std::getline(input, current_line))
    {
      return false;
    }
    current_line.erase(0, current_line.find_first_not_of(" \t\r\n"));
    current_line.erase(current_line.find_last_not_of(" \t\r\n") + 1);

    try
    {
      size_t end_of_line = 0;
      result.n = std::stoi(current_line, &end_of_line);
      if (result.n < 1 || result.n > 255)
      {
        error_line = current_line;
        return false;
      }
      if (end_of_line != current_line.length())
      {
        error_line = current_line;
        return false;
      }
      result.rooms.resize(result.n + 1);

      for (int i = 0; i <= result.n; ++i)
      {
        if (!std::getline(input, current_line))
        {
          return false;
        }
        current_line.erase(0, current_line.find_first_not_of(" \t\r\n"));
        current_line.erase(current_line.find_last_not_of(" \t\r\n") + 1);
        if (current_line.empty())
        {
          error_line = "empty line";
          return false;
        }
        std::istringstream ss(current_line);

        terra::room_t room;

        int number = 0;
        ss >> number;
        if (!ss || number < 0 || number > result.n)
        {
          error_line = current_line;
          return false;
        }
        room.id = number;

        std::string adj;
        ss >> adj;
        size_t last = 0, next = 0;
        int val = 0;
        if (adj.empty())
        {
          error_line = current_line;
          return false;
        }
        while ((next = adj.find(',', last)) != std::string::npos)
        {
          std::string room_num = adj.substr(last, next - last);
          val = std::stoi(room_num, &end_of_line);
          if (end_of_line != room_num.length() || val < 0 || val > result.n)
          {
            error_line = current_line;
            return false;
          }
          room.adj.push_back(val);
          last = next + 1;
        }
        std::string room_num = adj.substr(last);
        val = std::stoi(room_num, &end_of_line);
        if (end_of_line != room_num.length() || val < 0 || val > result.n)
        {
          error_line = current_line;
          return false;
        }
        room.adj.push_back(val);

        for (size_t j = 0; j < 4; ++j)
        {
          ss >> number;
          if (!ss || number < 0 || number > 255)
          {
            error_line = current_line;
            return false;
          }
          room.res[j] = number;
        }

        if (result.rooms[room.id].id != -1)
        {
          error_line = current_line;
          return false;
        }
        result.rooms[room.id] = room;

        std::string extra;
        if (ss >> extra)
        {
          error_line = current_line;
          return false;
        }
      }
      std::getline(input, current_line);
      std::istringstream ss(current_line);

      ss >> result.food;
      if (!ss || result.food < 2 || result.food > 255)
      {
        error_line = current_line;
        return false;
      }
      std::string res;
      ss >> res;
      if (res == "iron")
      {
        result.double_res = 0;
      }
      else if (res == "gold")
      {
        result.double_res = 1;
      }
      else if (res == "gems")
      {
        result.double_res = 2;
      }
      else if (res == "exp")
      {
        result.double_res = 3;
      }
      else
      {
        error_line = current_line;
        return false;
      }

      if (ss >> res)
      {
        error_line = current_line;
        return false;
      }
    }
    catch (const std::exception & e)
    {
      error_line = current_line;
      return false;
    }
    return true;
  }
}
