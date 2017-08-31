#include "Systematics.h"


Systematics::Systematics(){}

Systematics::Systematics(std::unordered_map<string, PartStats> const &distats){

}
Systematics::~Systematics(){}

void Systematics::init(){

}


void Systematics::shiftParticle(Particle& jet, TLorentzVector recJet, double const& ratio, double& dPx, double& dPy, string syst){

   //add the shifted part up
   dPx+=recJet.Px()*(ratio-1);
   dPy+=recJet.Py()*(ratio-1);
   //WARNING change the particle content for the particle
   recJet*=ratio;
   jet.addP4Syst(recJet, syst);
   return;
}
