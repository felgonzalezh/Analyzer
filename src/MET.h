#ifndef Met_h
#define Met_h

// system include files
#include <memory>

// user include files
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>
#include "Particle.h"


#include "tokenizer.hpp"
#include "Cut_enum.h"

using namespace std;
typedef unsigned int uint;


class Met {

public:
  Met();
  Met(TTree*, string, vector<string>);
  virtual ~Met() {}

  virtual void findExtraCuts() {}
  void init();
  void unBranch();
  double pt() const;
  double px() const;
  double py() const;
  double eta() const;
  double phi() const;
  double energy() const;
  double HT() const;
  double MHT() const;
  double MHTphi() const;
  TLorentzVector p4() const;
  TLorentzVector& p4();

  void setPtEtaPhiESyst( double, double, double, double, string);
  void addPtEtaPhiESyst(double, double, double, double, string);
  void addP4Syst(TLorentzVector, string);
  void setCurrentP(string);
  string getName() {return GenName;};
  void update(string);

  TLorentzVector Reco;
  TLorentzVector *cur_P;

  unordered_map<string, TLorentzVector* > systVec;
  unordered_map<string, double > systdeltaMEx;
  unordered_map<string, double > systdeltaMEy;
  unordered_map<string, double > syst_HT;
  unordered_map<string, double > syst_MHT;
  unordered_map<string, double > syst_MHTphi;


  string activeSystematic;

protected:
  TTree* BOOM;
  string GenName;
  double mMet[3] = {0, 0, 0};
  //note this is only for pt and phi
  double MetUnclUp[2] = {0, 0};
  double MetUnclDown[2] = {0, 0};
  bool hasUncl=false;

};



#endif
