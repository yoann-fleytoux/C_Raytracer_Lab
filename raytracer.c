#include "raytracer.h"
#include "globals.h"
#include "utils.h"

#include <math.h>

float inter_plan(ray r, object plan){
  return -((vector_dot(plan.normal, r.orig) + plan.dist) / vector_dot(r.dir, plan.normal));
}

float inter_sphere(ray r, object sphere){
  float a = vector_dot(r.dir,r.dir);
  float b = 2.0f*(vector_dot(r.dir, vector_minus(r.orig, sphere.center)));
  float c = vector_dot(vector_minus( r.orig, sphere.center),vector_minus(r.orig, sphere.center)) - pow(sphere.radius,2);

  float d = (b*b) - (4.0f*a*c);
  if (d > 0.f)
  {
    float r1 = (-b + sqrt(d) )/ 2*a;
    float r2 = (-b - sqrt(d) )/ 2*a;
    if (r2 > 0.f)
    {
      return r2;
    }
    else return r1;
  }
  else return -1;

}

float inters(ray r, int idobjet)
{
  object monobjet = scene[idobjet];
  if(monobjet.type == PLANE)
    return inter_plan(r, monobjet);
  else if(monobjet.type == SPHERE)
    return inter_sphere(r, monobjet);
  else return 0.0f;
}
/*
int blinn_phong(object monobjet,ray r){
  color c;
  vec3 n,h;
  if (monobjet.type == SPHERE)
  {
    n.x = 0;
    n.y = 0;
    n.z = 0;
  } else if (monobjet.type == PLANE){
    n = monobjet.normal;
  }
  h = 
}*/
color BlinnPhong(intersection pt)
{
  color c;
  c.x = c.y = c.z = 0.f;
  vec3 l, v, h, inv_kd, ks_s, res;
  float dot_hn, dot_nl;
  int i;
  
  v = vector_normalized(vector_minus(theCamera.position, pt.position));
  for(i = 0; i < 3; i++)
  {
    l = vector_normalized(vector_minus(lights[i].position, pt.position));
    h = vector_add(l, v);
    h = vector_float_mul(1 / vector_norm(h), h);
    h = vector_normalized(h);
    
    inv_kd = vector_float_mul((1.0f / M_PI), pt.mat.kd);
    ks_s = vector_float_mul(((pt.mat.shininess + 8.f) / (8.f * M_PI)), pt.mat.ks);
    
    dot_nl = vector_dot(lights[i].position,pt.normal);
    dot_hn = pow(vector_dot(h,pt.normal),pt.mat.shininess);
    
    res = vector_float_mul(dot_nl, vector_add(inv_kd, vector_float_mul(dot_hn, ks_s)));
    
    c = vector_add(c, vector_vec_mul(res,lights[i].col));
    c = vector_clamp(c, 0.f, 1.f);
  }
  return c;
}

color trace_ray(ray r){
  float t;
  float tmin = r.tmax;
  int idobjet = 0;
  int idobjetmin;
  intersection inter;
  for (int i = 0; i < object_count; ++i)
  {
    t = inters(r, i);
      if(t>0 && t<tmin) {
        idobjetmin = i;
        tmin = t;
      }
    }
    if (tmin == r.tmax) {
      return vector_init(0.2,0.2,0.6);
    }
    else {
      inter.mat = scene[idobjetmin].mat;
      inter.position.x = 0;
      inter.position.y = 0;
      inter.position.z = 0;
      if (scene[idobjetmin].type == PLANE){
        inter.normal = scene[idobjetmin].normal;
      } else {
        inter.normal.x = inter.position.x - scene[idobjetmin].center.x;
        inter.normal.y = inter.position.y - scene[idobjetmin].center.y;
        inter.normal.z = inter.position.z - scene[idobjetmin].center.z;
      }
     return scene[idobjetmin].mat.kd;
    }
}

void raytrace(){
  color couleur;
  vec3 xr, yr, zr, d;
  ray r;
  xr = theCamera.xdir;
  yr = vector_float_mul((1/theCamera.aspect) , theCamera.ydir);
  zr = theCamera.center; 


  for (int i = 0; i < WIDTH; ++i)
  {
    for (int j = 0; j < HEIGHT; ++j)
    {
      float coeffxr = ((i + 0.5 - (WIDTH/2))/(WIDTH/2));
      float coeffyr = ((j + 0.5 - (HEIGHT/2))/(HEIGHT/2));
      d = vector_add(zr, vector_add( vector_float_mul(coeffyr,yr), vector_float_mul(coeffxr,xr) ));
      d = vector_normalized(d);
      ray_init(&r, theCamera.position, d);
      output_image[j*WIDTH+i] = trace_ray(r);
    }
  }
}
