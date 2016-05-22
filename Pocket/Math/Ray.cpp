//
//  Ray.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/16/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Ray.hpp"
#include "Matrix4x4.hpp"
#include "BoundingBox.hpp"
#include <cmath>

using namespace Pocket;



void Ray::Transform(const Pocket::Matrix4x4 &matrix) {
    position = matrix.TransformPosition(position);
    direction = matrix.TransformVector(direction);
}

bool Ray::Intersect(const BoundingBox& box) const {
    
    
    Vector3 Diff;
    
	Diff.x = position.x - box.center.x;
	if(fabsf(Diff.x)>box.extends.x && Diff.x*direction.x>=0.0f)	return false;
    
	Diff.y = position.y - box.center.y;
	if(fabsf(Diff.y)>box.extends.y && Diff.y*direction.y>=0.0f)	return false;
    
    Diff.z = position.z - box.center.z;
	if(fabsf(Diff.z)>box.extends.z && Diff.z*direction.z>=0.0f)	return false;
    
	float fAWdU[3];
	fAWdU[0] = fabsf(direction.x);
	fAWdU[1] = fabsf(direction.y);
	fAWdU[2] = fabsf(direction.z);
    
	float f;
	f = direction.y * Diff.z - direction.z * Diff.y;	if(fabsf(f)>box.extends.y*fAWdU[2] + box.extends.z*fAWdU[1])	return false;
	f = direction.z * Diff.x - direction.x * Diff.z;	if(fabsf(f)>box.extends.x*fAWdU[2] + box.extends.z*fAWdU[0])	return false;
	f = direction.x * Diff.y - direction.y * Diff.x;	if(fabsf(f)>box.extends.x*fAWdU[1] + box.extends.y*fAWdU[0])	return false;
    
	return true;    
}