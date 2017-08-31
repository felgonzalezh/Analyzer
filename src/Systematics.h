#ifndef Systematics_hh
#define Systematics_hh

#include <string>
#include <iostream>
#include <functional>
#include <unordered_map>
#include "Particle.h"
//#include <boost/unordered_map.hpp>

// we will put stuff from the main analyser here once we know what


class TRandom3;

class Systematics {

public:
  Systematics();
  Systematics(std::unordered_map<string, PartStats> const &distats);
  ~Systematics();

  void init();

  void shiftParticle(Particle& jet, TLorentzVector recJet, double const& ratio, double& dPx, double& dPy, string syst);

private:
};
#endif /*Systematics_hh*/
