#include "MET.h"
#include <algorithm>

#define SetBranch(name, variable) BOOM->SetBranchStatus(name, true);  BOOM->SetBranchAddress(name, &variable);

//particle is a objet that stores multiple versions of the particle candidates
Met::Met(TTree* _BOOM, string _GenName,  vector<string> syst_names) : BOOM(_BOOM), GenName(_GenName) {

  SetBranch((GenName+"_px").c_str(), mMet[0]);
  SetBranch((GenName+"_py").c_str(), mMet[1]);
  SetBranch((GenName+"_pz").c_str(), mMet[2]);
  systVec["orig"] = new TLorentzVector();

  for( auto name : syst_names) {
    systVec[name] = new TLorentzVector();

    systdeltaMEx[name]=0;
    systdeltaMEy[name]=0;
  }
  //just in case add the Muon as met as well:

  systVec["muMET"] = new TLorentzVector();

  if( std::find(syst_names.begin(), syst_names.end(), "MetUncl_Up") != syst_names.end() && _BOOM->GetListOfBranches()->FindObject((GenName+"_UnclEnshiftedPtUp").c_str()) !=0){
    SetBranch((GenName+"_UnclEnshiftedPtUp").c_str(), MetUnclUp[0]);
    SetBranch((GenName+"_UnclEnshiftedPhiUp").c_str(), MetUnclUp[1]);
    SetBranch((GenName+"_UnclEnshiftedPtDown").c_str(), MetUnclDown[0]);
    SetBranch((GenName+"_UnclEnshiftedPhiDown").c_str(), MetUnclDown[1]);
    hasUncl=true;
  }

  activeSystematic="orig";
}

//void Met::setPtEtaPhiESyst(,double ipt,double ieta, double iphi, double ienergy, string syst){
  //if(systVec[syst]->size()< index ){
    //cout<<"syst  "<<syst<<" at index "<<index<<" does not exist"<<"  size "<<systVec[syst]->size()<<endl;
  //}
  //systVec[syst]->at(index).SetPtEtaPhiE(ipt,ieta,iphi,ienergy);
//}

void Met::addPtEtaPhiESyst(double ipt,double ieta, double iphi, double ienergy, string syst){
  systVec[syst]->SetPtEtaPhiE(ipt,ieta,iphi,ienergy);
}


void Met::addP4Syst(TLorentzVector mp4, string syst){
  systVec[syst]->SetPtEtaPhiE(mp4.Pt(),mp4.Eta(),mp4.Phi(),mp4.E());
}


void Met::init(){
  //cleanup of the particles
  //keep this if there is any ever some need for a unchanged met
  Reco.SetPxPyPzE(mMet[0],mMet[1],mMet[2],sqrt(pow(mMet[0],2) + pow(mMet[1],2)));
  for(auto &it: systVec){
    it.second->SetPtEtaPhiE(0,0,0,0);
    systdeltaMEx[it.first]=0;
    systdeltaMEy[it.first]=0;
  }
  addP4Syst(Reco,"orig");

  if(hasUncl){
    systVec["MetUncl_Up"]->SetPtEtaPhiE(MetUnclUp[0],0,MetUnclUp[1],MetUnclUp[0]);
    systVec["MetUncl_Down"]->SetPtEtaPhiE(MetUnclDown[0],0,MetUnclDown[1],MetUnclDown[0]);
  }
  cur_P=&Reco;
  activeSystematic="orig";
  syst_HT[activeSystematic]=0.;
  syst_MHT[activeSystematic]=0.;
  syst_MHTphi[activeSystematic]=99.;
}


void Met::update(string syst="orig"){
  ///Calculates met from values from each file plus smearing and treating muons as neutrinos

  systVec[syst]->SetPxPyPzE(cur_P->Px()+systdeltaMEx[syst], cur_P->Py()+systdeltaMEy[syst], cur_P->Pz(),
  TMath::Sqrt(pow(cur_P->Px()+systdeltaMEx[syst],2) + pow(cur_P->Py()+systdeltaMEy[syst],2)));
  setCurrentP(syst);
}

double Met::pt()const         {return cur_P->Pt();}
double Met::px()const         {return cur_P->Px();}
double Met::py()const         {return cur_P->Py();}
double Met::eta()const        {return cur_P->Eta();}
double Met::phi()const        {return cur_P->Phi();}
double Met::energy()const     {return cur_P->E();}


TLorentzVector Met::p4()const {return (*cur_P);}
TLorentzVector& Met::p4() {return *cur_P;}

double Met::HT() const {return syst_HT.at(activeSystematic);};
double Met::MHT() const {return syst_MHT.at(activeSystematic);};
double Met::MHTphi() const {return syst_MHTphi.at(activeSystematic);};


void Met::setCurrentP(string syst){
  cur_P = systVec[syst];
  activeSystematic=syst;
}


void Met::unBranch() {
  BOOM->SetBranchStatus((GenName+"*").c_str(), 0);
}

