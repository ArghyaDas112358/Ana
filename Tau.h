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
	int idx1;
	int idx2; 
	int idx3; 
	float sumdnn; 
	float dnn1; 
	float dnn2; 
	float dnn3; 

	ClassDef(Tau, 0);

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
