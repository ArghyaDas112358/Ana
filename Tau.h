#ifndef TAU_H
#define TAU_H
#ifdef __MAKECINT__ 
#pragma link C++ class vector<Tau>; 
#endif // __MAKECINT__

#include <TObject.h>
#include <vector>
#include <map>


class Tau : public TObject 
{
  public: 
	float pt = -999.; 
	float eta = -999.; 
	float phi = -999.; 
	int q = -999; 
	float m = -999.; 
	float vprob = -999.; 
	float fsig = -999.; 
	float lip = -999.; 
	int idx1 = -999; 
	int idx2 = -999; 
	int idx3 = -999; 
	float sumdnn = -999.; 
	float dnn1 = -999.; 
	float dnn2 = -999.; 
	float dnn3 = -999.; 
	float alpha = -999.; 
	float fl = -999.; 
	float pvip = -999.; 
	float pvips = -999.; 
	float dr = -999.; 

	float pi1pt = -999.; 
	float pi2pt = -999.; 
	float pi3pt = -999.; 
	float pi1eta = -999.; 
	float pi2eta = -999.; 
	float pi3eta = -999.; 
	float pi1phi = -999.; 
	float pi2phi = -999.; 
	float pi3phi = -999.; 

	float rhomass1; 
	float rhomass2; 

	bool match1 = false; 
	bool match2 = false; 
	bool match3 = false; 

	float B_m = -999.; 
	float B_q2 = -999.; 

	//float maxdaupt; 
	//float mindaupt; 
	//float maxdaueta; 
	//float mindaueta; 
	//float maxdauphi; 
	//float mindauphi; 

	ClassDef(Tau, 0);

	Tau() = default; 

	Tau(const float& initPt, const float& initEta, const float& initPhi, const int charge, const float& mass) 
	  : pt(initPt), eta(initEta), phi(initPhi), q(charge), m(mass) {}

	void SetIndices(const int myIdx1, const int myIdx2, const int myIdx3) 
	{
		idx1 = myIdx1; 
		idx2 = myIdx2; 
		idx3 = myIdx3; 
	}

	void SetDNN(const float& myDNN1, const float& myDNN2, const float&myDNN3, const float& mySumOfDNN) 
	{
		dnn1 = myDNN1; 
		dnn2 = myDNN2; 
		dnn3 = myDNN3; 
		sumdnn = mySumOfDNN; 
	}

	void SetKinematics(const float& vertexProb, const float& flightSig, const float& longIP) 
	{
		vprob = vertexProb; 
		fsig = flightSig; 
		lip = longIP; 
	}


	bool operator>(const Tau& other) const 
	{ 
    	return pt > other.pt;
  	}

  	bool operator<(const Tau& other) const 
	{ 
    	return pt < other.pt;
  	}


  	void SetEventKinematics(const float& angle, const float& deltaR, const float& flightLenght, const float& IPtoPV, const float& sigIPtoPV) 
  	{
  		alpha = angle; 
  		dr = deltaR; 
  		fl = flightLenght; 
  		pvip = IPtoPV; 
  		pvips = sigIPtoPV; 
  	}


  	void SetDau1Kin(const float& dauPt, const float& dauEta, const float& dauPhi) 
  	{
  		pi1pt = dauPt; 
  		pi1eta = dauEta; 
  		pi1phi = dauPhi; 
  	}


  	void SetDau2Kin(const float& dauPt, const float& dauEta, const float& dauPhi) 
  	{
  		pi2pt = dauPt; 
  		pi2eta = dauEta; 
  		pi2phi = dauPhi; 
  	}


  	void SetDau3Kin(const float& dauPt, const float& dauEta, const float& dauPhi) 
  	{
  		pi3pt = dauPt; 
  		pi3eta = dauEta; 
  		pi3phi = dauPhi; 
  	}

  	void SetRhoMasses(const float& m12, const float& m23) 
  	{
  		rhomass1 = m12; 
  		rhomass2 = m23; 
  	}

  	void SetMatch(const bool genMatch1, const bool genMatch2, const bool genMatch3) 
  	{
  		match1 = genMatch1; 
  		match2 = genMatch2; 
  		match3 = genMatch3; 
  	}

  	void SetMatch(const int genMatch1, const int genMatch2, const int genMatch3) 
  	{
  		match1 = static_cast<bool>(genMatch1); 
  		match2 = static_cast<bool>(genMatch2); 
  		match3 = static_cast<bool>(genMatch3); 
  	}

  	void SetBQuantities(const float mass, const float q2) 
  	{
  		B_m = mass; 
  		B_q2 = q2; 
  	}





	static float WritePt(const Tau& tau) 
	{
		return tau.pt; 
	}; 

	static float WriteEta(const Tau& tau) 
	{
		return tau.eta; 
	}; 

	static float WritePhi(const Tau& tau) 
	{
		return tau.phi; 
	}; 

	static int WriteCharge(const Tau& tau) 
	{
		return tau.q; 
	}; 

	static float WriteMass(const Tau& tau) 
	{
		return tau.m; 
	}; 

	static float WriteVprob(const Tau& tau) 
	{
		return tau.vprob; 
	}; 

	static float WriteFsig(const Tau& tau) 
	{
		return tau.fsig; 
	}; 

	static float WriteLip(const Tau& tau) 
	{
		return tau.lip; 
	}; 

	static int WriteIdx1(const Tau& tau) 
	{
		return tau.idx1; 
	}; 

	static int WriteIdx2(const Tau& tau) 
	{
		return tau.idx2; 
	}; 

	static int WriteIdx3(const Tau& tau) 
	{
		return tau.idx3; 
	}; 

	static float WriteDNN1(const Tau& tau) 
	{
		return tau.dnn1; 
	}; 

	static float WriteDNN2(const Tau& tau) 
	{
		return tau.dnn2; 
	}; 

	static float WriteDNN3(const Tau& tau) 
	{
		return tau.dnn3; 
	}; 

	static float WriteSumDNN(const Tau& tau) 
	{
		return tau.sumdnn; 
	}; 

	static float WriteAlpha(const Tau& tau) 
	{
		return tau.alpha; 
	}; 

	static float WriteFl(const Tau& tau) 
	{
		return tau.fl; 
	}; 

	static float WritePVIP(const Tau& tau) 
	{
		return tau.pvip; 
	}; 

	static float WritePVIPsig(const Tau& tau) 
	{
		return tau.pvips; 
	}; 

	static float WriteDr(const Tau& tau) 
	{
		return tau.dr; 
	}; 

	static float WriteDau1Pt(const Tau& tau) 
	{
		return tau.pi1pt; 
	}; 

	static float WriteDau1Eta(const Tau& tau) 
	{
		return tau.pi1eta; 
	}; 

	static float WriteDau1Phi(const Tau& tau) 
	{
		return tau.pi1phi; 
	}; 

	static float WriteDau2Pt(const Tau& tau) 
	{
		return tau.pi2pt; 
	}; 

	static float WriteDau2Eta(const Tau& tau) 
	{
		return tau.pi2eta; 
	}; 

	static float WriteDau2Phi(const Tau& tau) 
	{
		return tau.pi2phi; 
	}; 

	static float WriteDau3Pt(const Tau& tau) 
	{
		return tau.pi3pt; 
	}; 

	static float WriteDau3Eta(const Tau& tau) 
	{
		return tau.pi3eta; 
	}; 

	static float WriteDau3Phi(const Tau& tau) 
	{
		return tau.pi3phi; 
	}; 

	static float WriteRhomass1(const Tau& tau) 
	{
		return tau.rhomass1; 
	}; 

	static float WriteRhomass2(const Tau& tau) 
	{
		return tau.rhomass2; 
	}; 

	static int WriteMatch(const Tau& tau) 
	{
		return static_cast<int>(tau.match1 || tau.match2 || tau.match3); 
	}; 

	static int WriteSumMatch(const Tau& tau) 
	{
		return static_cast<int>(tau.match1) + static_cast<int>(tau.match2) + static_cast<int>(tau.match3); 
	}; 

	static float WriteBmass(const Tau& tau) 
	{
		return tau.B_m; 
	}; 

	static float WriteBq2(const Tau& tau) 
	{
		return tau.B_q2; 
	}; 

}; 


typedef std::vector<Tau> TauCollection; 
//typedef std::map<Tau, float> PtOrderedTauCollection; 


#endif 
