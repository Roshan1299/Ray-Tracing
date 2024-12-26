#include "vector.h"
#include <math.h>

Vec3 add(Vec3 v1, Vec3 v2){
	Vec3 sum = {v1.x + v2.x,v1.y + v2.y,v1.z + v2.z};
	return sum;
};

Vec3 subtract(Vec3 v1, Vec3 v2){
	Vec3 sub = {v1.x - v2.x,v1.y - v2.y,v1.z - v2.z};
        return sub ;  
};

Vec3 scalarMultiply(float s, Vec3 v){
	Vec3 smul = {s*v.x,s*v.y,s*v.z};
	return smul ;
};

Vec3 scalarDivide(Vec3 v, float d){
    if (d == 0) {
        return v;  // return the same vector if division by zero
    }

    Vec3 result = {v.x / d, v.y / d, v.z / d};
    return result;
   }

//Vec3 normalize(Vec3 v){
//	float len = length(v);
//    	if (len == 0) return v; 
//    	return scalarDivide(v, len);	
//};

float dot(Vec3 v1, Vec3 v2){
	float vdot = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return vdot;
};

float length2(Vec3 v){
	float len2 = dot(v,v);
	return len2;
};
float length(Vec3 v){
	float len = sqrt(dot(v,v));
	return len;
};
float distance2(Vec3 v1, Vec3 v2){
	float dist2 = length2(subtract(v1,v2));
	return dist2;
};
float distance(Vec3 v1, Vec3 v2){
	float dist = length(subtract(v1,v2));
	return dist;
};

Vec3 normalize(Vec3 v){
        float len = length(v);
        if (len == 0) return v;
        return scalarDivide(v, len);
};
