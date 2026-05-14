#ifndef ALICE_BOT_HPP
#define ALICE_BOT_HPP

#include "simulator.hpp"

class AliceBot: public Simulator
{
  int whichIsValuable(int room) const;

  std::vector< int > getForwardPath();
  std::vector< int > getReturnPath();
public:
  using Simulator::Simulator;
  void runBot(std::ostream & output) override;
};

#endif
