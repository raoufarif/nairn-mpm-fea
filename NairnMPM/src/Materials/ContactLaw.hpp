/********************************************************************************
	ContactLaw.hpp
	nairn-mpm-fea

	Created by John Nairn, Oct 24, 2015.
	Copyright (c) 2015 John A. Nairn, All rights reserved.

	Dependencies
		MaterialBase.hpp
********************************************************************************/

#ifndef CONTACTLAW

#define CONTACTLAW 60

#include "Materials/MaterialBase.hpp"

class MaterialController;
class NodalPoint;

// contact law properties for inptu only to support old style entry
typedef struct {
	double friction;
	double Dn;
	double Dnc;
	double Dt;
	int otherMatID;
	int contactLawID;
} ContactInput;

class ContactLaw : public MaterialBase
{
	public:
		ContactInput contactProps;
		int autoID;
	
		// constructors and destructors
		ContactLaw();
		ContactLaw(char *matName);
	
		// initialize
		virtual char *InputMaterialProperty(char *,int &,double &);
		virtual const char *VerifyAndLoadProperties(int);
		virtual void PrintMaterial(int) const;
		virtual void PrintContactLaw(void) const;
	
		// methods
		virtual bool GetFrictionalDeltaMomentum(Vector *,Vector *,double,double *,double,bool,double,bool,double,Vector *) const;
		virtual void GetInterfaceForces(Vector *,Vector *,double *,double,Vector *,double,double,Vector *,double,double,double) const;
		virtual double GetSslideAcDt(double,double,double,double,bool &,double) const;
	
		// accessors
		virtual int MaterialStyle(void) const;
		virtual const char *MaterialType(void) const;
		virtual double WaveSpeed(bool,MPMBase *) const;
		virtual bool IgnoreContact(void) const;
		virtual bool ContactIsDone(bool) const;
		virtual bool IsFrictionalContact(void) const;
		virtual bool ContactLawNeedsContactArea(void) const;
		virtual bool IsImperfectInterface(void) const;
	
		// Class methods
		static int ConvertOldStyleToContactLaw(MaterialController *,ContactInput *,double);
	
	protected:
	
};

#endif

