/********************************************************************************
    CommonUtilities.hpp
    nairn-mpm-fea
    
    Created by John Nairn on 1/12/06.
    Copyright (c) 2006 John A. Nairn, All rights reserved.
********************************************************************************/

#include "stdafx.h"

#define T_SQRT3    1.73205080756887729352744634151   // sqrt(3)

// local globals
static int section=1;

#pragma mark Miscellaneous Functions

/*********************************************************************
    Start next section in results window
*********************************************************************/

void PrintSection(const char *text)
{
    char sline[200];
    
    sprintf(sline,"*****%3d. %s\n\n",section,text);
    cout << sline;
	section++;
}

/************************************************************
	See if two double precision numbers are sufficiently
		equal (currently 100 ppm)
	If both are less than 1e-12, they are assumed equal
************************************************************/

#define MAX_DIFF 1.0e-16
#define MAX_REL_DIFF 1.0e-7

bool DbleEqual(double A, double B)
{
	// Check if the numbers are really close -- needed
	// when comparing numbers near zero.
	double diff = fabs(A - B);
	if (diff <= MAX_DIFF)
		return true;

	// Check relative difference
	A = fabs(A);
	B = fabs(B);
	double largest = (B > A) ? B : A;
	if (diff <= largest * MAX_REL_DIFF)
		return true;

	// assuming unequal
	return false;
}

/************************************************************
    Reverse block of bytes to convert between Intel
	processor and Mac processor
************************************************************/

int Reverse(char *nptr,int nlen)
{
    int i,imax;
    char *rptr,temp;
    
    rptr=nptr+nlen-1;
    imax=nlen>>1;
    for(i=0;i<imax;i++)
    {	temp=*nptr;
        *nptr++=*rptr;
        *rptr--=temp;
    }
    return nlen;
}

/************************************************************
	Factor integer num into prime factors (not including 1)
************************************************************/

void PrimeFactors(int num,vector<int> &factors)
{	int maxFactor = (int)(sqrt((double)num)+.001);
	for(int i=2;i<=maxFactor;i++)
	{	// does i go into num evenly?
		if((num % i) == 0)
		{	factors.push_back(i);
			PrimeFactors(num/i, factors);
			return;
		}
	}
	// if did not find any factors, it is a prime
	factors.push_back(num);
}

#pragma mark Vector Functions

// return vector from components
Vector MakeVector(double x,double y,double z)
{	Vector v;
	v.x=x;
	v.y=y;
	v.z=z;
	return v;
}

// return vector from components all times scale factor
Vector MakeScaledVector(double x,double y,double z,double scale)
{	Vector v;
	v.x=x*scale;
	v.y=y*scale;
	v.z=z*scale;
	return v;
}

// return vector from difference of two vectors
Vector SetDiffVectors(const Vector *a,const Vector *b)
{	Vector v;
	v.x=a->x-b->x;
	v.y=a->y-b->y;
	v.z=a->z-b->z;
	return v;
}

// return vector from sum of two vectors
Vector SetSumVectors(const Vector *a,const Vector *b)
{	Vector v;
	v.x=a->x+b->x;
	v.y=a->y+b->y;
	v.z=a->z+b->z;
	return v;
}

// return vector from components all times scale factor
Vector SetScaledVector(const Vector *a,const double scale)
{	Vector v;
	v.x=a->x*scale;
	v.y=a->y*scale;
	v.z=a->z*scale;
	return v;
}

// zero a vector and return pointer to zeroed v
Vector *ZeroVector(Vector *v)
{	v->x=v->y=v->z=0.;
	return v;
}

// copy vector and return pointer to new one
Vector *CopyVector(Vector *v,const Vector *oldv)
{	v->x=oldv->x;
	v->y=oldv->y;
	v->z=oldv->z;
	return v;
}

// scale a vector and return pointer to changed vector
Vector *ScaleVector(Vector *v,const double scale)
{	v->x*=scale;
	v->y*=scale;
	v->z*=scale;
	return v;
}

// scale a vector and copy to new one, then return pointer to new v
Vector *CopyScaleVector(Vector *v,const Vector *oldv,const double scale)
{	v->x=scale*oldv->x;
	v->y=scale*oldv->y;
	v->z=scale*oldv->z;
	return v;
}

// add vector toadd to vector v and return pointer to changed v
Vector *AddVector(Vector *v,const Vector *toadd)
{	v->x+=toadd->x;
	v->y+=toadd->y;
	v->z+=toadd->z;
	return v;
}

// sub vector toadd from vector v and return pointer to changed v
Vector *SubVector(Vector *v,const Vector *toadd)
{	v->x-=toadd->x;
	v->y-=toadd->y;
	v->z-=toadd->z;
	return v;
}

// add vector toadd scaled by scale to vector v and return pointer to changed v
Vector *AddScaledVector(Vector *v,const Vector *toadd,const double scale)
{	v->x+=toadd->x*scale;
	v->y+=toadd->y*scale;
	v->z+=toadd->z*scale;
	return v;
}

// Dot product of two vectors (if used for 2D make sure z's are zero)
double DotVectors(const Vector *v1,const Vector *v2)
{	return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z ;
}

// Dot prodiuct of two 2D vectors
double DotVectors2D(const Vector *v1,const Vector *v2)
{	return v1->x*v2->x + v1->y*v2->y ;
}

// Vector cross product of two vectors in a third vector
Vector *CrossProduct(Vector *cp,const Vector *a,const Vector *b)
{	cp->x = a->y*b->z - b->y*a->z ;
	cp->y = b->x*a->z - a->x*b->z ;
	cp->z = a->x*b->y - b->x*a->y ;
	return cp;
}

// Z component of vector cross product of two vectors ion x-y plane
double CrossProduct2D(const Vector *a,const Vector *b)
{	return a->x*b->y - b->x*a->y ;
}

// Print vector to cout when debugging
void PrintVector(const char *label,const Vector *v)
{	if(strlen(label)>0) cout << label;
	cout << "(" << v->x << ", " << v->y << ", " << v->z << ") ";
}

#pragma mark Tensor Functions

// return tensor from components
Tensor MakeTensor(double xx,double yy,double zz,double yz,double xz,double xy)
{	Tensor t;
	t.xx = xx;
	t.yy = yy;
	t.zz = zz;
#ifdef MPM_CODE
	t.yz = yz;
	t.xz = xz;
#endif
	t.xy = xy;
	return t;
}

// return tensor from components
Tensor MakeTensor2D(double xx,double yy,double zz,double xy)
{	Tensor t;
	t.xx = xx;
	t.yy = yy;
	t.zz = zz;
#ifdef MPM_CODE
	t.yz = 0.;
	t.xz = 0.;
#endif
	t.xy = xy;
	return t;
}

// zero a vector and return pointer to zeroed v
Tensor *ZeroTensor(Tensor *t)
{	t->xx=0.;
	t->yy=0.;
	t->zz=0.;
#ifdef MPM_CODE
	t->yz=0.;
	t->xz=0.;
#endif
	t->xy=0.;
	return t;
}

// add tensor toadd to tensor t and return pointer to changed t
Tensor *AddTensor(Tensor *t,Tensor *toadd)
{	t->xx+=toadd->xx;
	t->yy+=toadd->yy;
	t->zz+=toadd->zz;
#ifdef MPM_CODE
	t->yz+=toadd->yz;
	t->xz+=toadd->xz;
#endif
	t->xy+=toadd->xy;
	return t;
}

// subtract tensor tosub from tensor t and return pointer to changed t
Tensor *SubTensor(Tensor *t,Tensor *tosub)
{	t->xx-=tosub->xx;
	t->yy-=tosub->yy;
	t->zz-=tosub->zz;
#ifdef MPM_CODE
	t->yz-=tosub->yz;
	t->xz-=tosub->xz;
#endif
	t->xy-=tosub->xy;
	return t;
}

// scale tensor t and return pointer to changed t
Tensor *ScaleTensor(Tensor *t,double scale)
{	t->xx*=scale;
	t->yy*=scale;
	t->zz*=scale;
#ifdef MPM_CODE
	t->yz*=scale;
	t->xz*=scale;
#endif
	t->xy*=scale;
	return t;
}

// Dot product of two vectors (if used for 2D make sure z's are zero)
double DotTensors2D(const Tensor *t1,const Tensor *t2)
{	return t1->xx*t2->xx + t1->yy*t2->yy + t1->zz*t2->zz + t1->xy*t2->xy;
}

#ifdef MPM_CODE
// Dot product of two vectors (if used for 2D make sure z's are zero)
double DotTensors(const Tensor *t1,const Tensor *t2)
{	return t1->xx*t2->xx + t1->yy*t2->yy + t1->zz*t2->zz
	+ t1->yz*t2->yz + t1->xz*t2->xz + t1->xy*t2->xy;
}

// Find Eigenvalues of symmetric tensor in Voight form
// stress is true for stress vector or false for strain vector
// is2D is true for 2D tensor or false for 3D
Vector TensorEigenvalues(Tensor *t,bool stress,bool is2D)
{
	Vector lam;
	
	if(is2D)
	{   // solving x^2 + bx + c = 0 (see Numerical Recipes in C, page 156)
		double b = -(t->xx+t->yy);
		double c = stress ? t->xx*t->yy - t->xy*t->xy : t->xx*t->yy - 0.25*t->xy*t->xy ;
		double arg = b*b-4.*c;
		if(arg<0.)
		{	// assuming here all matrices are positive definite, which means
			// a negative value should be interpreted as zero
			lam.x = -0.5*b;
		}
		else
		{	arg = sqrt(arg);
			lam.x = b>0 ? -0.5*(b+arg) : -0.5*(b-arg) ;
		}
		lam.y = lam.x==0. ? 0. : c/lam.x;
		lam.z = t->zz;
	}
	else
	{	double mm, c1, c0;
		
		// Determine coefficients of characteristic poynomial. We write
		//       | a   d   f  |
		//  m =  | d   b   e  |
		//       | f   e   c  |
		double fde,dd,ee,ff;		// products of elements
		if(stress)
		{	fde = t->xy*t->yz*t->xz;
			dd = t->xy*t->xy;
			ee = t->yz*t->yz;
			ff = t->xz*t->xz;
		}
		else
		{	fde = 0.125*t->xy*t->yz*t->xz;
			dd = 0.25*t->xy*t->xy;
			ee = 0.25*t->yz*t->yz;
			ff = 0.25*t->xz*t->xz;
		}
		mm  = t->xx + t->yy + t->zz;
		c1 = (t->xx*t->yy + t->xx*t->zz + t->yy*t->zz)
					- (dd + ee + ff);				// a*b + a*c + b*c - d^2 - e^2 - f^2
		c0 = t->zz*dd + t->xx*ee + t->yy*ff - t->xx*t->yy*t->zz
					- 2.0*fde;						// c*d^2 + a*e^2 + b*f^2 - a*b*c - 2*f*d*e)
		
		double p, sqrt_p, q, c, s, phi;
		p = mm*mm - 3.0*c1;
		q = mm*(p - (3.0/2.0)*c1) - (27.0/2.0)*c0;
		sqrt_p = sqrt(fabs(p));
		
		phi = 27.0 * ( 0.25*c1*c1*(p - c1) + c0*(q + 27.0/4.0*c0));
		phi = (1.0/3.0) * atan2(sqrt(fabs(phi)), q);
		
		c = sqrt_p*cos(phi);
		s = (1.0/T_SQRT3)*sqrt_p*sin(phi);
		
		lam.y  = (1.0/3.0)*(mm - c);
		lam.z  = lam.y + s;
		lam.x  = lam.y + c;
		lam.y -= s;
	}
	
	return lam;
}

// convert tensor to matrix (stress is true for straight convertion of false to assume strain tensor)
Matrix3 TensorToMatrix(Tensor *t,bool stress)
{	return stress ?
			Matrix3(t->xx,t->xy,t->xz,t->xy,t->yy,t->yz,t->xz,t->yz,t->zz) :
			Matrix3(t->xx,0.5*t->xy,0.5*t->xz,0.5*t->xy,t->yy,0.5*t->yz,0.5*t->xz,0.5*t->yz,t->zz) ;
}

// convert tensor to matrix (stress is true for straight convertion of false to assume strain tensor)
Matrix3 TensorToMatrix2D(Tensor *t,bool stress)
{	return stress ?
			Matrix3(t->xx,t->xy,t->xy,t->yy,t->zz) :
			Matrix3(t->xx,0.5*t->xy,0.5*t->xy,t->yy,t->zz) ;
}

#endif

// get element of contracted tensor by ID
double Tensor_i(Tensor *t,int contractedID)
{
	switch(contractedID)
	{	case XX:
			return t->xx;
		case YY:
			return t->yy;
		case ZZ:
			return t->zz;
#ifdef MPM_CODE
		case YZ:
			return t->yz;
		case XZ:
			return t->xz;
#endif
		case XY:
			return t->xy;
		default:
			return 0.;
	}
}

// get element of tensor by row and colume
// For FEA can only get xx or 1,1, xy or 1,2 or 2,1, yy or 2,2, anbd zz or 3,3
double Tensor_ij(Tensor *t,int row,int col)
{
	switch(row)
	{	case 1:
			switch(col)
			{	case 1:
					return t->xx;
				case 2:
					return t->xy;
#ifdef MPM_CODE
				case 3:
					return t->xz;
#endif
				default:
					return 0.;
			}
		case 2:
			switch(col)
			{	case 1:
					return t->xy;
				case 2:
					return t->yy;
#ifdef MPM_CODE
				case 3:
					return t->yz;
#endif
				default:
					return 0.;
			}
		case 3:
			switch(col)
			{
#ifdef MPM_CODE
				case 1:
					return t->xz;
				case 2:
					return t->yz;
#endif
				case 3:
					return t->zz;
				default:
					return 0.;
			}
		default:
			return 0.;
	}
}

// Print tensor to cout when debuggingin form of a matrix
void PrintTensor(const char *label,Tensor *t)
{
	int i,lead=(int)strlen(label);
	for(i=0;i<lead;i++) cout << " ";
#ifdef MPM_CODE
	cout << "(" << t->xx << ", " << t->xy << "," << t->xz << ")" << endl;
	cout << label << "(" << t->xy << ", " << t->yy << "," << t->yz << ")" << endl;
#else
	cout << "(" << t->xx << ", " << t->xy << ", 0.0)" << endl;
	cout << label << "(" << t->xy << ", " << t->yy << ", 0.0)" << endl;
#endif
	for(i=0;i<lead;i++) cout << " ";
#ifdef MPM_CODE
	cout << "(" << t->xz << ", " << t->yz << "," << t->zz << ")" << endl;
#else
	cout << "( 0.0, 0.0," << t->zz << ")" << endl;
#endif
}

#ifdef MPM_CODE

TensorAntisym *ZeroTensorAntisym(TensorAntisym *a)
{	a->xy=a->xz=a->yz=0.;
	return a;
}

#endif

#pragma mark String Functions

// convert character to lower case
unsigned char ConvertToLower(unsigned char ch)
{	if (ch >= 'A' && ch <= 'Z')
		ch = 'a' + (ch - 'A');
	return ch;
}

// convert path with / to DOS path with back slashes
// return pointer to input string
char *MakeDOSPath(char *unixPath)
{	for (unsigned int i = 0; i < strlen(unixPath); i++)
	{	if (unixPath[i] == '/')
			unixPath[i] = '\\';
	}
	return unixPath;
}

// case insensitive compare
// return 0 if match, return <0 or >0 if first case insentive mismatch character
//      has lower value in s1 or in s2, respectively
int CIstrcmp(const char *s1, const char *s2)
{
	unsigned char *us1 = (unsigned char *)s1;
	unsigned char *us2 = (unsigned char *)s2;
	
	while(ConvertToLower(*us1) == ConvertToLower(*us2++))
	{	// they are equal, so still matching
		
		// if matching at the end, then found a match
		if(*us1 == '\0') return (0);
		
		// otherwise on to next character
		us1++;
	}
	
	// found mismatch
	us2--;
	return (int)(ConvertToLower(*us1) - ConvertToLower(*us2));
}

// file extension of last component in the path (or empty string if none)
void GetFileExtension(const char *fileName,char *ext,int maxLength)
{
	int endLoc = (int)strlen(fileName)-1;
	int dotLoc = endLoc;
	while(dotLoc>=0 && fileName[dotLoc]!='.' && fileName[dotLoc]!='/') dotLoc--;
	
	// found an extension
	ext[0] = 0;
	if(dotLoc>=0)
	{	// empty if hit folder divider
		if(fileName[dotLoc]=='/') return;
		
		// first character after the period
		dotLoc++;
		int ei = 0;
		
		// last one gets the zero terminator
		while(ei<maxLength && dotLoc<=endLoc)
			ext[ei++]=fileName[dotLoc++];
		ext[ei] = 0;
	}
}

#pragma mark Other Function

// The cummulative dist function is p(x,mean,s) = 0.5(1+erf((x-mean)/(s*sqrt(2))))
// Given p(d,0,1), return d and set (x-mean)/s = d or x = mean + s*d
// Or relative to mean value/mean = 1 + s*d/mean = 1 + d*(CV)
// where (CV) is coefficient of variation.
// A property
// found at http://www.johndcook.com/blog/normal_cdf_inverse/
double NormalCDFInverse(double p)
{	if(p <= 0.0 || p >= 1.0)
	{	return 0.;
	}
	else if(p < 0.5)
	{	// F^-1(p) = - G^-1(p)
		return -RationalApproximation( sqrt(-2.0*log(p)) );
	}
	else if (p < 1.0)
	{	// F^-1(p) = G^-1(1-p)
		return RationalApproximation( sqrt(-2.0*log(1-p)) );
	}
	else
		return 1.;
}

// Abramowitz and Stegun formula 26.2.23.
// The absolute value of the error should be less than 4.5 e-4.
// found at http://www.johndcook.com/blog/normal_cdf_inverse/
double RationalApproximation(double t)
{	double c[] = {2.515517, 0.802853, 0.010328};
	double d[] = {1.432788, 0.189269, 0.001308};
	return t - ((c[2]*t + c[1])*t + c[0]) /
	(((d[2]*t + d[1])*t + d[0])*t + 1.0);
}

#ifdef MPM_CODE
/****************************************************************
 *  Functions for an intersect of plane and  unit cube
 ****************************************************************/

double ci[5]={-1.,1.,1.,-1.,-1.};

// Find intersected edges
// Prechecked to have intersection and intersection is not a corner
// On input v = 0.5*(dx,dy,dz)
Vector IntersectEdges(int i,int j,Vector *v,Vector *n)
{
	// corner vectors (0,1,2,3 on +dx face, 4,5,6,7 on -dx face)
	Vector Vertex_i = (i<4) ? MakeVector(v->x,ci[i]*v->y,-ci[i+1]*v->z) :
						MakeVector(-v->x,ci[i-4]*v->y,-ci[i-3]*v->z);
	Vector Edge_ij = (j<4) ? MakeVector(v->x,ci[j]*v->y,-ci[j+1]*v->z) :
						MakeVector(-v->x,ci[j-4]*v->y,-ci[j-3]*v->z);
	
	// Fined edge vector difference of coordinates
	Edge_ij.x -=Vertex_i.x;
	Edge_ij.y -=Vertex_i.y;
	Edge_ij.z -=Vertex_i.z;
	
	// Start calculating lambda (assume has an intersection
	double lambda = -DotVectors(&Vertex_i,n)/DotVectors(&Edge_ij,n);
	
	// if not in [0,1] then it doesn't intersect
	if(lambda>1.0 || lambda<0.0)
	{	cout << "# failed to find intersection " << i << "," << j ;
		PrintVector(", c=",&Vertex_i);
		PrintVector(", e=",&Edge_ij);
		cout << ", lambda=" << lambda << endl;
	}
	
	// Find point and return it
	AddScaledVector(&Vertex_i,&Edge_ij,lambda);
	return Vertex_i;
}

// Find are of parallelogram by vectors (v-c) and (-v-c) in plane with normal n
double PGramArea(Vector *c,Vector *v,Vector *n)
{
	// get sides of parallelogram (v-c and -v-c)
	Vector v1 = *v;
	SubVector(&v1,c);
	Vector v2 = SetScaledVector(v,-1.);
	SubVector(&v2,c);
	
	// get cross product
	Vector cp;
	CrossProduct(&cp,&v1,&v2);
	
	// return area of parallogram enclosed by v1 and v2
	// assume in plane with normal n
	return fabs(DotVectors(&cp,n));
}

// get the area of interection between plane through center of box (dx,dy,dz) and the box
double AreaOverVolume3D(Vector *norm,double dx,double dy,double dz)
{
	// For notes see JANOSU-14-44+
	
	// examine corners on face with n=(1,0,0)
	double cx = -dx*norm->x;
	int sgn[5],firstZero=-1;;
	
	// look at corners in order (dx,-dy,-dz),(dx,dy,-dz),(dx,dy,dz),(dx,-dy,dz)
	for(int i=0;i<4;i++)
	{	double cyz = ci[i]*dy*norm->y - ci[i+1]*dz*norm->z;
		
		// find sign (or zero) of n.xc = cyz - cx
		if(DbleEqual(cx,cyz))
		{	// n.xc is zero
			if(firstZero>=0)
			{	// Case 1: found two intersections. Found  diagonal plane, but which diagonal?
				if(i==1)
				{	// edge 0-1
					return sqrt(dx*dx+dz*dz)/(dx*dz);
				}
				else if(i==2)
				{	// diagonal 0-2 or edge 1-2, respectively
					double area = firstZero==0 ? sqrt(dy*dy+dz*dz)/(dy*dz) : sqrt(dx*dx+dy*dy)/(dx*dy);
					return area;
				}
				// corner 3 with 0, 1, or 2
				if(firstZero==0)
				{	// edge 0-3
					return sqrt(dx*dx+dy*dy)/(dx*dy);
				}
				else if(firstZero==1)
				{	// diagonal 1-3
					return sqrt(dy*dy+dz*dz)/(dy*dz);
				}
				// edge 2-3
				return sqrt(dx*dx+dz*dz)/(dx*dz);
			}
			
			// first one
			firstZero = i;
			sgn[i]=0;
		}
		else if(cyz>cx)
			sgn[i]=1;
		else
			sgn[i]=-1;
	}
	
	// store the size
	Vector v1,v2,sz = MakeVector(0.5*dx,0.5*dy,0.5*dz);
	sgn[4]=sgn[0];			// to help some algorithms
	
	// count postive values
	int numPositive=0;
	for(int i=0;i<4;i++)
	{	if(sgn[i]>0) numPositive++;
	}
	
	// Was there one intersection?
	if(firstZero>=0)
	{	// The one corner
		Vector c = MakeVector(0.5*dx,0.5*ci[firstZero]*dy,-0.5*ci[firstZero+1]*dz);
		
		if(numPositive==0 || numPositive==3)
		{	// Case 2: one corner intersection, zero edge intersections
			//cout << "# Case 2: one corner intersection, zero edge intersections" << endl;
			// find any edge except the one ending in the 0 corner
			int c1 = firstZero==0 ? 3 : firstZero-1;
			v1 = IntersectEdges(c1,c1+4,&sz,norm);
		}
		
		else
		{	// Case 3: one corner intersection, one edge intersections
			// a sign change - find it
			int i;
			for(i=0;i<4;i++)
			{	if(sgn[i]*sgn[i+1]<0)
				{	int c1 = i==3 ? 0 : i+1;
					v1 = IntersectEdges(i,c1,&sz,norm);
					break;
				}
			}
			if(i==4) cout << "# failed to find intersected edge" << endl;
		}
				
		// area of parallelogram
		return PGramArea(&c,&v1,norm)/(dx*dy*dz);
	}
	
	// No corner intersections
	
	if(numPositive==4 || numPositive==0)
	{	// Case 4: no intersections or all nodes on +dx face same side of plane
		// It is parallelgram intersecting edges opn the for dx edges; pick any two
		Vector c = IntersectEdges(0,4,&sz,norm);
		v1 = IntersectEdges(1,5,&sz,norm);
		return PGramArea(&c,&v1,norm)/(dx*dy*dz);
	}
	
	else if(numPositive==1 || numPositive==3)
	{	// Case 5: two intersections on adjacent edges will intersect with a hexagon
		// find the corner with differing sign
		int c1 = -1;
		for(int i=0;i<4;i++)
		{	if((numPositive==1 && sgn[i]==1) || (numPositive==3 && sgn[i]==-1))
			{	c1 = i;
				break;
			}
		}
		if(c1<0) cout << "# cound not find the one corner" << endl;
		
		// find two instersection on +dx plane
		int c2 = c1==3 ? 0 : c1+1;				// corner after c1
		v1 = IntersectEdges(c1,c2,&sz,norm);
		c2 = c1==0 ? 3 : c1-1;					// corner before c1
		v2 = IntersectEdges(c1,c2,&sz,norm);
		Vector v3 = IntersectEdges(c2,c2+4,&sz,norm);	// one edge to -dx face
		
		// first parallolgram
		double area = PGramArea(&v2,&v1,norm);
		
		// second parallogram v2-v3 and -v1-v3
		SubVector(&v2,&v3);
		ScaleVector(&v1,-1.);
		SubVector(&v1,&v3);
		Vector cp;
		CrossProduct(&cp,&v1,&v2);
		area += fabs(DotVectors(&cp,norm));
		return area/(dx*dy*dz);
	}
	
	// Case 6: intersects 2 opposite edges: parallelogram  with corners on along dy or dz edges
	if(sgn[0]==sgn[1])
	{	v1 = IntersectEdges(1,2,&sz,norm);
		v2 = IntersectEdges(3,0,&sz,norm);
	}
	else
	{	v1 = IntersectEdges(0,1,&sz,norm);
		v2 = IntersectEdges(2,3,&sz,norm);
	}
	return PGramArea(&v1,&v2,norm)/(dx*dy*dz);
}
#endif
