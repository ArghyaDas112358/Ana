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
	float pt; 
	float eta; 
	float phi; 
	int q; 
	float m; 
	float vprob; 
	float fsig; 
	float lip; 
	int idx1;
	int idx2; 
	int idx3; 
	float sumdnn; 
	float dnn1; 
	float dnn2; 
	float dnn3; 

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




	static std::vector<float> WritePt(std::vector<Tau> collection) 
	{
		std::vector<float> result; 
		for (auto tau : collection) 
		{
			result.push_back(tau.pt); 
		}
		return result; 
	}; 

}; 


typedef std::vector<Tau> TauCollection; 
//typedef std::map<Tau, float> PtOrderedTauCollection; 


#endif 
