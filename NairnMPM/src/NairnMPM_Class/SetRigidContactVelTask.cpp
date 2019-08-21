/********************************************************************************
	SetRigidContactVelTask.cpp
	nairn-mpm-fea

	Created by John Nairn on May 12, 2016
	Copyright (c) 2016 John A. Nairn, All rights reserved.

	The tasks are:
	* Check all rigid contact materials (nmpmsRB to nmpmsRC-1)
		* Set the velocity only if a direction is controlled by a user-defined functions
********************************************************************************/

#include "stdafx.h"
#include "NairnMPM_Class/SetRigidContactVelTask.hpp"
#include "MPM_Classes/MPMBase.hpp"
#include "Materials/RigidMaterial.hpp"
#include "Exceptions/CommonException.hpp"

extern double mtime;
extern double timestep;

#pragma mark CONSTRUCTORS

SetRigidContactVelTask::SetRigidContactVelTask(const char *name) : MPMTask(name)
{
}

#pragma mark REQUIRED METHODS

// Get mass matrix, find dimensionless particle locations,
//	and find grid momenta
void SetRigidContactVelTask::Execute(int taskOption)
{
	bool hasDir[3];

	// Set rigid contact material velocities separately
	CommonException *rcErr = NULL;
	
#pragma omp parallel for private(hasDir)
	for(int p=nmpmsRB;p<nmpmsRC;p++)
	{   MPMBase *mpmptr = mpm[p];
		const RigidMaterial *matID = (RigidMaterial *)theMaterials[mpm[p]->MatID()];
		try
		{	matID->GetVectorSetting(&mpmptr->vel,hasDir,mtime,&mpmptr->pos);
		}
		catch(CommonException &err)
		{   if(rcErr==NULL)
			{
#pragma omp critical (error)
				rcErr = new CommonException(err);
			}
		}
	}
	
	// throw now - only known error is problem with function for velocity setting
	if(rcErr!=NULL) throw *rcErr;
}
