#include "material.hpp"

#include "lights.hpp"
#include "VecUtils.h"

using namespace FW;

Vec3f PhongMaterial::shade(const Ray &ray, const Hit &hit, 
		const Vec3f &dir_to_light, 
		const Vec3f &incident_intensity,
		bool shade_back) const
{
	// YOUR CODE HERE (R4)
	// Ambient light was already dealt with in R1; implement the rest
	// of the Phong reflectance model (diffuse and specular) here.
	// Start with diffuse and add specular when diffuse is working.
	// NOTE: if shade_back flag is set,
	// you should treat the material as two-sided, i.e., flip the
	// normal if the ray hits the surface from behind. Otherwise
	// you should return zero for hits coming from behind the surface.
	// Remember, when computing the specular lobe, you shouldn't add
	// anything if the light is below the local horizon!

	//Vec3f point = ray.pointAtParameter(hit.t);
	Vec3f normal = hit.normal;
	Vec3f dir = dir_to_light.normalized() /*- point*/;
	
	float d = normal.dot(dir);
	bool below = (d < 0);
	d = !shade_back ? (below ? 0 : d) : fabs(d);

	//if (below && shade_back) normal = -normal;
	Vec3f r = (2 * normal.dot(dir) * normal - dir).normalized();

	float s = -ray.direction.dot(r);
	s = below || s < 0 ? 0 : s;

	Vec3f diffuse = diffuse_color_ * d;
	Vec3f specular = /*below ? 0 : */specular_color_ * FW::pow(s, exponent_);	

	Vec3f answer = (specular + diffuse) * incident_intensity;

	return answer;
}

Vec3f ProceduralMaterial::diffuse_color(const Vec3f& point) const {
	Vec3f a1 = m1_->diffuse_color(point);
	Vec3f a2 = m2_->diffuse_color(point);
	Vec3f pt = VecUtils::transformPoint(matrix_, point);
	float v = interpolation(pt);
	return a1 * v + a2 * (1 - v);
}

Vec3f ProceduralMaterial::reflective_color(const Vec3f& point) const {
	Vec3f a1 = m1_->reflective_color(point);
	Vec3f a2 = m2_->reflective_color(point);
	Vec3f pt = VecUtils::transformPoint(matrix_, point);
	float v = interpolation(pt);
	return a1 * v + a2 * (1 - v);
}

Vec3f ProceduralMaterial::transparent_color( const Vec3f& point) const {
	Vec3f a1 = m1_->transparent_color(point);
	Vec3f a2 = m2_->transparent_color(point);
	Vec3f pt = VecUtils::transformPoint(matrix_, point);
	float v = interpolation(pt);
	return a1 * v + a2 * (1 - v);
}

float ProceduralMaterial::refraction_index(const Vec3f& point) const {
	float a1 = m1_->refraction_index(point);
	float a2 = m2_->refraction_index(point);
	Vec3f pt = VecUtils::transformPoint(matrix_, point);
	float v = interpolation(pt);
	return a1 * v + a2 * (1 - v);
}

Vec3f ProceduralMaterial::shade(const Ray &ray, const Hit &hit, 
		const Vec3f &dirToLight, 
		const Vec3f &lightColor,
		bool shade_back) const {
	Vec3f pt = ray.pointAtParameter(hit.t);
	Vec3f a1 = m1_->shade(ray,hit,dirToLight,lightColor, shade_back);
	Vec3f a2 = m2_->shade(ray,hit,dirToLight,lightColor, shade_back);
	Vec3f pt2 = VecUtils::transformPoint(matrix_, pt);
	float v = interpolation(pt2);
	Vec3f answer = a1 * v + a2 * (1 - v);
	return answer;
}

float Checkerboard::interpolation(const Vec3f& point) const {
	int count = 1;
	count += int(point[0]) % 2;
	count += int(point[1]) % 2;
	count += int(point[2]) % 2;
	if(point[0] < 0) count ++;
	if(point[1] < 0) count ++;
	if(point[2] < 0) count ++;
	return float(count & 0x01);
}

