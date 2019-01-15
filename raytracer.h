#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

#include "ray.h"
#include "scene.h"
#include <stdbool.h>


//! INTERSECTION
//! this structure stores the data corresponding to an intersection, if the intersection is found
typedef struct intersection_s{
  vec3 normal; //! the normal at the intersection point
  point3 position; //! the 3D position of the intersection point
  material mat; //! material of the intersected object
} intersection;

//! render the scene : fill the global variable output_image
void raytrace();
color trace_ray(ray r);
float inter_plan(ray r, object plan);
float inter_sphere(ray r, object sphere);
float inters(ray r, int idobjet);
color BlinnPhong(intersection pt);

#endif
