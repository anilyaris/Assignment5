#include "objects.hpp"

#include "hit.hpp"
#include "VecUtils.h"

#include <cassert>

using namespace std;
using namespace FW;

Object3D* Group::operator[](int i) const {
	assert(i >= 0 && i < size());
	return objects_[i].get();
}

void Group::insert(Object3D* o) {
	assert(o);
	objects_.emplace_back(o);
}

bool Group::intersect(const Ray& r, Hit& h, float tmin) const {
	// We intersect the ray with each object contained in the group.
	bool intersected = false;
	for (int i = 0; i < size(); ++i) {
		Object3D* o = objects_[i].get();
		assert(o != nullptr);
		assert(h.t >= tmin);
		bool tmp = o->intersect(r, h, tmin);
		assert(h.t >= tmin);
		if (tmp)
			intersected = true;
	}
	assert(h.t >= tmin);
	return intersected;
}

bool Box::intersect(const Ray& r, Hit& h, float tmin) const {
// YOUR CODE HERE (EXTRA)
// Intersect the box with the ray!

  return false;
}

bool Plane::intersect( const Ray& r, Hit& h, float tmin ) const {
	// YOUR CODE HERE (R5)
	// Intersect the ray with the plane.
	// Pay attention to respecting tmin and h.t!
	// Equation for a plane:
	// ax + by + cz = d;
	// normal . p - d = 0
	// (plug in ray)
	// origin + direction * t = p(t)
	// origin . normal + t * direction . normal = d;
	// t = (d - origin . normal) / (direction . normal);
	float t = r.direction.dot(normal_);
	if (t == 0) return false;
	t = (offset_ - r.origin.dot(normal_)) / t;
	if (h.t > t  && t > tmin) {
		h.set(t, this->material(), normal_);
		return true;
	}
	return false;
}

Transform::Transform(const Mat4f& m, Object3D* o) :
	matrix_(m),
	object_(o)
{
	assert(o != nullptr);
	inverse_ = matrix_.inverted();
	inverse_transpose_ = inverse_.transposed();
}

bool Transform::intersect(const Ray& r, Hit& h, float tmin) const {
	// YOUR CODE HERE (EXTRA)
	// Transform the ray to the coordinate system of the object inside,
	// intersect, then transform the normal back. If you don't renormalize
	// the ray direction, you can just keep the t value and do not need to
	// recompute it!
	// Remember how points, directions, and normals are transformed differently!
	
	Vec3f origin_t = (inverse_ * Vec4f(r.origin, 1)).getXYZ();
	Vec3f point = r.origin + r.direction;
	Vec3f point_t = (inverse_ * Vec4f(point, 1)).getXYZ();
	Vec3f direction_t = point_t - origin_t;
	Ray r_t = Ray(origin_t, direction_t);
	bool intersect_ = object_->intersect(r_t, h, tmin * direction_t.length());
	if(intersect_) h.normal = (inverse_transpose_ * Vec4f(h.normal, 1)).getXYZ().normalized();
	return intersect_; 
}

bool Sphere::intersect( const Ray& r, Hit& h, float tmin ) const {
	// Note that the sphere is not necessarily centered at the origin.
	
	Vec3f tmp = center_ - r.origin;
	Vec3f dir = r.direction;

	float A = dot(dir, dir);
	float B = - 2 * dot(dir, tmp);
	float C = dot(tmp, tmp) - sqr(radius_);
	float radical = B*B - 4*A*C;
	if (radical < 0)
		return false;

	radical = sqrtf(radical);
	float t_m = ( -B - radical ) / ( 2 * A );
	float t_p = ( -B + radical ) / ( 2 * A );
	Vec3f pt_m = r.pointAtParameter( t_m );
	Vec3f pt_p = r.pointAtParameter( t_p );

	assert(r.direction.length() > 0.9f);

	bool flag = t_m <= t_p;
	if (!flag) {
		::printf( "sphere ts: %f %f %f\n", tmin, t_m, t_p );
		return false;
	}
	assert( t_m <= t_p );

	// choose the closest hit in front of tmin
	float t = (t_m < tmin) ? t_p : t_m;

	if (h.t > t  && t > tmin) {
		Vec3f normal = r.pointAtParameter(t);
		normal -= center_;
		normal.normalize();
		h.set(t, this->material(), normal);
		return true;
	}
	return false;
} 

Triangle::Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c,
	Material *m, const Vec2f& ta, const Vec2f& tb, const Vec2f& tc, bool load_mesh) :
	Object3D(m)
{
	vertices_[0] = a;
	vertices_[1] = b;
	vertices_[2] = c;
	texcoords_[0] = ta;
	texcoords_[1] = tb;
	texcoords_[2] = tc;

	if (load_mesh) {
		preview_mesh.reset((FW::Mesh<FW::VertexPNT>*)FW::importMesh("preview_assets/tri.obj"));
		set_preview_materials();
	}
}

bool Triangle::intersect( const Ray& r, Hit& h, float tmin ) const {
	// YOUR CODE HERE (R6)
	// Intersect the triangle with the ray!
	// Again, pay attention to respecting tmin and h.t!
	/*Vec3f ab = vertices_[0] - vertices_[1];
	Vec3f ac = vertices_[0] - vertices_[2];
	Vec3f b = vertices_[0] - r.direction;
	Mat3f A;
	A.setCol(0, ab);
	A.setCol(1, ac);
	A.setCol(2, r.direction);	
	float det_A = A.det();
	A.setCol(0, b);
	float beta = A.det() / det_A;
	A.setCol(0, ab);
	A.setCol(1, b);
	float gamma = A.det() / det_A;
	A.setCol(1, ac);
	A.setCol(2, b);
	float t = A.det() / det_A;
	float alpha = 1 - beta - gamma;
	if (h.t > t && t > tmin && alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1) {
		Vec3f normal = FW::cross(ab, ac).normalized();
		h.set(t, this->material(), normal);
		return true;
	}
	return false;*/

	const float EPSILON = 0.0000001;
	Vec3f vertex0 = vertices_[0];
	Vec3f vertex1 = vertices_[1];
	Vec3f vertex2 = vertices_[2];
	Vec3f edge1, edge2, hh, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	hh = r.direction.cross(edge2);
	a = edge1.dot(hh);
	if (a > -EPSILON && a < EPSILON) return false;
	f = 1 / a;
	s = r.origin - vertex0;
	u = f * (s.dot(hh));
	if (u < 0 || u > 1) return false;
	q = s.cross(edge1);
	v = f * r.direction.dot(q);
	if (v < 0 || u + v > 1) return false;
	float t = f * edge2.dot(q);
	if (t > EPSILON && h.t > t && t > tmin)
	{
		Vec3f normal = FW::cross(edge1, edge2).normalized();
		h.set(t, this->material(), normal);
		return true;
	}
	return false;
}

const Vec3f& Triangle::vertex(int i) const {
	assert(i >= 0 && i < 3);
	return vertices_[i];
}


