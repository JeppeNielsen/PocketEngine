#include "BoundingFrustum.hpp"

using namespace Pocket;

BoundingFrustum::BoundingFrustum()
{
}

BoundingFrustum::~BoundingFrustum()
{
}
/*
void BoundingFrustum::SetFromViewProjection(const Matrix4x4& viewProjection) {

	planes[0].normal.x = viewProjection.m[3][0] + viewProjection.m[0][0];
	planes[0].normal.y = viewProjection.m[3][1] + viewProjection.m[0][1];
	planes[0].normal.z = viewProjection.m[3][2] + viewProjection.m[0][2];
	planes[0].d =		 viewProjection.m[3][3] + viewProjection.m[0][3];

	// Right plane
	planes[1].normal.x = viewProjection.m[3][0] - viewProjection.m[0][0];
	planes[1].normal.y = viewProjection.m[3][1] - viewProjection.m[0][1];
	planes[1].normal.z = viewProjection.m[3][2] - viewProjection.m[0][2];
	planes[1].d =		 viewProjection.m[3][3] - viewProjection.m[0][3];

	// Top plane
	planes[2].normal.x = viewProjection.m[3][0] - viewProjection.m[1][0];
	planes[2].normal.y = viewProjection.m[3][1] - viewProjection.m[1][1];
	planes[2].normal.z = viewProjection.m[3][2] - viewProjection.m[1][2];
	planes[2].d =	     viewProjection.m[3][3] - viewProjection.m[1][3];

	// Bottom plane
	planes[3].normal.x = viewProjection.m[3][0] + viewProjection.m[1][0];
	planes[3].normal.y = viewProjection.m[3][1] + viewProjection.m[1][1];
	planes[3].normal.z = viewProjection.m[3][2] + viewProjection.m[1][2];
	planes[3].d =		 viewProjection.m[3][3] + viewProjection.m[1][3];

	// Near plane
	planes[4].normal.x = viewProjection.m[2][0];
	planes[4].normal.y = viewProjection.m[2][1];
	planes[4].normal.z = viewProjection.m[2][2];
	planes[4].d =		 viewProjection.m[2][3];

	// Far plane
	planes[5].normal.x = viewProjection.m[3][0] - viewProjection.m[2][0];
	planes[5].normal.y = viewProjection.m[3][1] - viewProjection.m[2][1];
	planes[5].normal.z = viewProjection.m[3][2] - viewProjection.m[2][2];
	planes[5].d =		 viewProjection.m[3][3] - viewProjection.m[2][3];

	for (int i=0; i<6; i++) planes[i].Normalize();
}
 */
 
/*
void BoundingFrustum::SetFromViewProjection(const Matrix4x4& viewProjection) {

	planes[0].normal.x = viewProjection.m[0][3] + viewProjection.m[0][0];
	planes[0].normal.y = viewProjection.m[1][3] + viewProjection.m[1][0];
	planes[0].normal.z = viewProjection.m[2][3] + viewProjection.m[2][0];
	planes[0].d =		 viewProjection.m[3][3] + viewProjection.m[3][0];

	// Right plane
	planes[1].normal.x = viewProjection.m[0][3] - viewProjection.m[0][0];
	planes[1].normal.y = viewProjection.m[1][3] - viewProjection.m[1][0];
	planes[1].normal.z = viewProjection.m[2][3] - viewProjection.m[2][0];
	planes[1].d =		 viewProjection.m[3][3] - viewProjection.m[3][0];

	// Top plane
	planes[2].normal.x = viewProjection.m[0][3] - viewProjection.m[0][1];
	planes[2].normal.y = viewProjection.m[1][3] - viewProjection.m[1][1];
	planes[2].normal.z = viewProjection.m[2][3] - viewProjection.m[2][1];
	planes[2].d =	     viewProjection.m[3][3] - viewProjection.m[3][1];

	// Bottom plane
	planes[3].normal.x = viewProjection.m[0][3] + viewProjection.m[0][1];
	planes[3].normal.y = viewProjection.m[1][3] + viewProjection.m[1][1];
	planes[3].normal.z = viewProjection.m[2][3] + viewProjection.m[2][1];
	planes[3].d =		 viewProjection.m[3][3] + viewProjection.m[3][1];

	// Near plane
	planes[4].normal.x = viewProjection.m[0][2];
	planes[4].normal.y = viewProjection.m[1][2];
	planes[4].normal.z = viewProjection.m[2][2];
	planes[4].d =		 viewProjection.m[3][2];

	// Far plane
	planes[5].normal.x = viewProjection.m[0][3] - viewProjection.m[0][2];
	planes[5].normal.y = viewProjection.m[1][3] - viewProjection.m[1][2];
	planes[5].normal.z = viewProjection.m[2][3] - viewProjection.m[2][2];
	planes[5].d =		 viewProjection.m[3][3] - viewProjection.m[3][2];

	for (int i=0; i<6; i++) planes[i].Normalize();
}
*/





void BoundingFrustum::SetFromViewProjection(const Matrix4x4& viewProjection) {
    
	planes[0].normal.x = viewProjection.m[3][0] + viewProjection.m[0][0];
	planes[0].normal.y = viewProjection.m[3][1] + viewProjection.m[0][1];
	planes[0].normal.z = viewProjection.m[3][2] + viewProjection.m[0][2];
	planes[0].d =		 viewProjection.m[3][3] + viewProjection.m[0][3];
    
	// Right plane
	planes[1].normal.x = viewProjection.m[3][0] - viewProjection.m[0][0];
	planes[1].normal.y = viewProjection.m[3][1] - viewProjection.m[0][1];
	planes[1].normal.z = viewProjection.m[3][2] - viewProjection.m[0][2];
	planes[1].d =		 viewProjection.m[3][3] - viewProjection.m[0][3];
    
	// Top plane
	planes[2].normal.x = viewProjection.m[3][0] - viewProjection.m[1][0];
	planes[2].normal.y = viewProjection.m[3][1] - viewProjection.m[1][1];
	planes[2].normal.z = viewProjection.m[3][2] - viewProjection.m[1][2];
	planes[2].d =	     viewProjection.m[3][3] - viewProjection.m[1][3];
    
	// Bottom plane
	planes[3].normal.x = viewProjection.m[3][0] + viewProjection.m[1][0];
	planes[3].normal.y = viewProjection.m[3][1] + viewProjection.m[1][1];
	planes[3].normal.z = viewProjection.m[3][2] + viewProjection.m[1][2];
	planes[3].d =		 viewProjection.m[3][3] + viewProjection.m[1][3];
    
	// Near plane
	planes[4].normal.x = viewProjection.m[3][0] + viewProjection.m[2][0];
	planes[4].normal.y = viewProjection.m[3][1] + viewProjection.m[2][1];
	planes[4].normal.z = viewProjection.m[3][2] + viewProjection.m[2][2];
	planes[4].d =		 viewProjection.m[3][3] + viewProjection.m[2][3];
    
	// Far plane
	planes[5].normal.x = viewProjection.m[3][0] - viewProjection.m[2][0];
	planes[5].normal.y = viewProjection.m[3][1] - viewProjection.m[2][1];
	planes[5].normal.z = viewProjection.m[3][2] - viewProjection.m[2][2];
	planes[5].d =		 viewProjection.m[3][3] - viewProjection.m[2][3];
    
	for (int i=0; i<6; i++) planes[i].Normalize();
}












// Returns: INTERSECT : 0 
//          INSIDE : 1 
//          OUTSIDE : 2 


int BoundingFrustum::Count = 0;

BoundingFrustum::Intersection BoundingFrustum::Intersect(const BoundingBox& box) const { 
	Intersection ret = INSIDE; 
	Vector3 vmin, vmax; 
	Vector3 halfExtends = box.extends * 0.5f;
	Vector3 mins = box.center - halfExtends;
	Vector3 maxs = box.center + halfExtends;

	Count++;

	for(int i = 0; i < 6; ++i) { 
		// X axis 
		if(planes[i].normal.x > 0) { 
			vmin.x = mins.x; 
			vmax.x = maxs.x; 
		} else { 
			vmin.x = maxs.x; 
			vmax.x = mins.x; 
		} 
		// Y axis 
		if(planes[i].normal.y > 0) { 
			vmin.y = mins.y; 
			vmax.y = maxs.y; 
		} else { 
			vmin.y = maxs.y; 
			vmax.y = mins.y; 
		} 
		// Z axis 
		if(planes[i].normal.z > 0) { 
			vmin.z = mins.z; 
			vmax.z = maxs.z; 
		} else { 
			vmin.z = maxs.z; 
			vmax.z = mins.z;
		}
        if (planes[i].Distance(vmax)<0) return OUTSIDE;
		if (planes[i].Distance(vmin)<=0) ret = INTERSECT;

		//if (planes[i].Distance(vmin)>0) return 2;
		//if (planes[i].Distance(vmax)>=0) ret = 0;

		//if(Vector::DotProduct(planes[i].normal, vmin) + planes[i].d > 0) 
		//	return 2; 
		//if(Vector::DotProduct(planes[i].normal, vmax) + planes[i].d >= 0) 
		//	ret = 0; 
	} 
	return ret;
} 

