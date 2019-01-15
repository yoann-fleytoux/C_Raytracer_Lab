#ifdef USE_FREEIMAGE
#include "FreeImage.h"
#endif

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "vector.h"
#include "scene.h"
#include "ray.h"
#include "raytracer.h"
#include "globals.h"

// globals
light lights[3];
int num_lights = 1;
object scene[SCENE_SIZE];
int object_count;

color output_image[WIDTH*HEIGHT];
camera theCamera;

void fill_red(color *img){
  for (int i=0; i<WIDTH; i++){
    for (int j=0; j<HEIGHT; j++){
      img[j*WIDTH+i]=vector_init(1,0,0);
    }
  }
}

void fill(color *img, color c){
  for (int i=0; i<WIDTH; i++){
    for (int j=0; j<HEIGHT; j++){
      img[j*WIDTH+i]=c;
    }
  }
}

void horizontal_gradient(color *img, color c1, color c2){
  float coef;
  for (int j=0; j<HEIGHT; j++){
    coef=(j/(float)HEIGHT);
    for (int i=0; i<WIDTH; i++){
      img[j*WIDTH+i]=vector_add(vector_float_mul(coef,c1),vector_float_mul(1-coef,c2));
    }
  }
}

void fill_rect(color *img, color c, point3 p1, point3 p2){
  point3 pmin = vector_min(p1,p2);
  point3 pmax = vector_max(p1,p2);
  for (int i=pmin.x; i<pmax.x; i++){
    for (int j=pmin.y; j<pmax.y; j++){
      img[j*WIDTH+i]=c;
    }
  } 
}

void fill_circle(color *img, float radius, point3 center, color c){
  for (int i=(center.x)-radius;i<(center.x)+radius;i++){
    for (int j=(center.y)-radius;j<(center.y)+radius;j++){
      if(vector_sqnorm(vector_minus(vector_init(i,j,0),center))<(radius*radius)){
        img[j*WIDTH+i]=c;
      }
    }
  }
}


void setup_scene_0(){
    theCamera = init_camera(vector_init(2,1,3), vector_init(0,0.3,0), vector_init(0,1,0), 60, (float)WIDTH/(float)HEIGHT);

    lights[0].position = vector_init(-200, 170, 300);
    lights[0].col = vector_init(2, 2, 2);
    num_lights=1;
  
    object_count = 0;
    material mat;

    mat.kd = vector_init(.5, .5, .5);
    mat.ks = vector_init(.5,.5,.5);
    mat.shininess = 200;
    scene[object_count ++] = init_sphere(0,0,0,.3, mat);

    mat.kd = vector_init(.9,0,0);
    mat.ks = vector_init(.1,.1,.1);
    mat.shininess = 500;
    scene[object_count ++] = init_sphere(1,-.05,0,.15, mat);

    mat.kd = vector_init(0,1,0);
    mat.ks = vector_init(.0,.0,.0);
    mat.shininess = 0.01;
    scene[object_count ++] = init_sphere(0,1,0,.25, mat);

    mat.kd = vector_init(0,0,0.6);
    mat.ks = vector_init(.4,.4,.4);
    mat.shininess = 20;
    scene[object_count ++] = init_sphere(0,-.05,1,.20, mat);
}

void setup_scene_1(){
    theCamera = init_camera(vector_init(2,1,3), vector_init(0,0.3,0), vector_init(0,1,0), 60, (float)WIDTH/(float)HEIGHT);

    lights[0].position = vector_init(-2, 1.7, 3);
    lights[0].col = vector_init(1, 1, 1);
    lights[1].position = vector_init(3, 2, 3);
    lights[1].col = vector_init(0.4, 0.4, 0.4);
    lights[2].position = vector_init(4,3,-10);
    lights[2].col = vector_init(0.5, 0.5, 0.5);

    num_lights=3;
  
    object_count = 0;
    material mat;

    mat.kd = vector_init(.5,.5,.5);
    mat.ks = vector_init(.5,.5,.5);
    mat.shininess = 20;
    mat.reflect_coef = 0.8;
    scene[object_count ++] = init_sphere(0,0,0,.3, mat);

    mat.kd = vector_init(0,0,1);
    mat.ks = vector_init(.5,.5,.5);
    mat.shininess = 20;
    mat.reflect_coef = 0.5;
    scene[object_count ++] = init_sphere(0,-.05,1,.20, mat);

    mat.ks = vector_init(.1,.1,.1);
    mat.kd = vector_init(0,1,0);
    mat.reflect_coef = 0.04;
    scene[object_count ++] = init_sphere(0,1,0,.15, mat);

    mat.kd = vector_init(1,0,0);
    mat.shininess = 500;
    scene[object_count ++] = init_sphere(1,-.05,0,.15, mat);

    mat.kd = vector_init(.5,1,.7);
    scene[object_count ++] = init_plane(0,1,0,0.2, mat);
}

void setup_scene_2(){
    theCamera = init_camera(vector_init(2,1,3), vector_init(0,0.3,0), vector_init(0,1,0), 60, (float)WIDTH/(float)HEIGHT);

    lights[0].position = vector_init(-2, 1.7, 3);
    lights[0].col = vector_init(1, 1, 1);
    lights[1].position = vector_init(3, 2, 3);
    lights[1].col = vector_init(0.4, 0.4, 0.4);
    lights[2].position = vector_init(4,3,-10);
    lights[2].col = vector_init(0.5, 0.5, 0.5);

    num_lights=3;
  
    object_count = 0;
    material mat;
    mat.shininess = 20;
    mat.kd = vector_init(1,1,1);
    mat.ks = vector_init(.5,.5,.5);
    mat.reflect_coef = 0.8;
    scene[object_count ++] = init_sphere(0,0,0,.3, mat);

    mat.reflect_coef = 0.5;
    mat.shininess = 20;
    mat.kd = vector_init(0,0,1);
    scene[object_count ++] = init_sphere(0,-.05,1,.20, mat);

    mat.reflect_coef = 0.04;
    mat.ks = vector_init(.1,.1,.1);
    mat.kd = vector_init(0,1,0);
    scene[object_count ++] = init_sphere(0,1,0,.15, mat);

    mat.kd = vector_init(1,0,0);
    mat.shininess = 500;
    scene[object_count ++] = init_sphere(1,-.05,0,.15, mat);

    mat.kd = vector_init(.5,1,.7);
    scene[object_count ++] = init_plane(0,1,0,0.2, mat);


    mat.reflect_coef = 0.0;
    mat.ks = vector_init(.1,.1,.1);
    mat.kd = vector_init(1,1,0);
    scene[object_count ++] = init_cylinder(vector_init(.0,.0,.0), vector_init(1,0,0), 1, .05, mat);
    scene[object_count ++] = init_cylinder(vector_init(.0,.0,.0), vector_init(0,1,0), 1, .05, mat);
    scene[object_count ++] = init_cylinder(vector_init(.0,.0,.0), vector_init(0,0,1), 1, .05, mat);

}

void setup_scene_3(){
    theCamera = init_camera(vector_init(2,1,3), vector_init(0,0.3,0), vector_init(0,1,0), 60, (float)WIDTH/(float)HEIGHT);

    lights[0].position = vector_init(-2, 1.7, 3);
    lights[0].col = vector_init(1, 1, 1);
    lights[1].position = vector_init(3, 2, 3);
    lights[1].col = vector_init(0.4, 0.4, 0.4);
    lights[2].position = vector_init(4,3,-10);
    lights[2].col = vector_init(0.5, 0.5, 0.5);

    num_lights=3;
  
    object_count = 0;
    material mat;
    mat.shininess = 20;
    mat.kd = vector_init(1,0,0);
    mat.ks = vector_init(.5,.5,.5);
    mat.reflect_coef = 0.8;
    for(int i=0; i<SCENE_SIZE; i++){
    	mat.kd = vector_init(((float)(rand()%255))/255.f,((float)(rand()%255))/255.f,((float)(rand()%255))/255.f);
    	scene[object_count ++] = init_sphere(((float)(rand()%8))/4.f,((float)(rand()%8))/4.f,((float)(rand()%8))/4.f,.1, mat);
    }
}

void setup_scene_4(){
  theCamera = init_camera(vector_init(2,1,3), vector_init(0,0.3,0), vector_init(0,1,0), 60, (float)WIDTH/(float)HEIGHT);
  lights[0].position = vector_init(0, 1.7, 1);
  lights[0].col = vector_init(1, 1, 1);
  lights[1].position = vector_init(3, 2, 3);
  lights[1].col = vector_init(0.4, 0.4, 0.4);
  lights[2].position = vector_init(4,3,-1);
  lights[2].col = vector_init(0.5, 0.5, 0.5);

  num_lights=3;
  
  object_count = 0;
  material mat;
  mat.kd = vector_init(.5, .5, .5);
  mat.ks = vector_init(.5, .5, .5);
  mat.shininess = 200;
  scene[object_count ++] = init_sphere(0,0,0,.3, mat);

  mat.kd = vector_init(.9,0,0);
  mat.ks = vector_init(.1,.1,.1);
  mat.shininess = 500;
  scene[object_count ++] = init_sphere(1,-.05,0,.15, mat);

  mat.kd = vector_init(0,1,0);
  mat.ks = vector_init(.0,.0,.0);
  mat.shininess = 0.01;
  scene[object_count ++] = init_sphere(0,1,0,.25, mat);

  mat.kd = vector_init(0,0,0.6);
  mat.ks = vector_init(.4,.4,.4);
  mat.shininess = 20;
  scene[object_count ++] = init_sphere(0,-.05,1,.20, mat);

  mat.kd = vector_init(.5,0.9,.7);
  mat.ks = vector_init(.01,.01,.01);
  mat.shininess = 50;
  scene[object_count ++] = init_plane(0,1,0,0.2, mat);

  mat.kd = vector_init(.8,0.09,.07);
  mat.ks = vector_init(.2,.2,.1);
  mat.shininess = 10;
  scene[object_count ++] = init_plane(1,0.0, -1.0, 2, mat);

  mat.kd = vector_init(0.1,0.3,.05);
  mat.ks = vector_init(.5,.5,.5);
  mat.shininess = 100;
  scene[object_count ++] = init_plane(0.3,-0.2, 1, 3, mat);
}


void setup_scene( int id){
    if(id == 0){ setup_scene_0(); return;}
    if(id == 1){ setup_scene_1(); return;}
    if(id == 2){ setup_scene_2(); return;}
    if(id == 3){ setup_scene_3(); return;}   
    if(id == 4){ setup_scene_4(); return;}   
}

// MAIN
int main(int argc, char ** argv){
    int scene_id = 0;
    bool use_octree = false;
    char basename[256]="";
    if(argc >1) scene_id = atoi(argv[1]);
    if(argc >2) strcpy(basename, argv[2]);

    // scene_id 0, 1, 2, 3 is for tp 1 tests;
    switch(scene_id){
    case 0: fill_red(output_image); break;
    case 1: 
	fill(output_image, vector_init(0.7,0.3,0.1)); 
	fill_rect(output_image, vector_init(0.2, 0.6, 0.7), vector_init(WIDTH/4, HEIGHT/5,0), vector_init(WIDTH/3, HEIGHT/4,0));
	break;
    case 2: horizontal_gradient(output_image, vector_init(0,0,1), vector_init(1,1,1)); break;
    case 3: 
	horizontal_gradient(output_image, vector_init(0,0,1), vector_init(1,1,1)); 
	fill_circle(output_image, WIDTH/20, vector_init(4*WIDTH/5, 4*HEIGHT/5, 0), vector_init(1, 1, 0)); 
	fill_rect(output_image, vector_init(0.1, 0.8, 0.4), vector_init(0, 0, 0), vector_init(WIDTH, HEIGHT/4,0));
	break;
    case 4:  break;
    default:
	setup_scene(scene_id-5);
	raytrace();
	break;
    }

#ifdef USE_FREEIMAGE
    FreeImage_Initialise();
    FIBITMAP *bitmap = FreeImage_Allocate(WIDTH, HEIGHT, 32, FI_RGBA_RED_MASK,
					  FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK); 
    if (bitmap) {
	// bitmap successfully created!
   
	// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
	color *ptr = output_image;
	int bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
	for(unsigned y = 0; y < FreeImage_GetHeight(bitmap); y++) {
	    BYTE *bits = FreeImage_GetScanLine(bitmap, y);
	    for(unsigned x = 0; x < FreeImage_GetWidth(bitmap); x++) {
		// Set pixel color to green with a transparency of 128
		*ptr = vector_clamp(*ptr, 0.f, 1.f);
		bits[FI_RGBA_RED] = ptr->x*255;
		bits[FI_RGBA_GREEN] = ptr->y*255;
		bits[FI_RGBA_BLUE] = ptr->z*255;
		bits[FI_RGBA_ALPHA] = 255;
		// jump to next pixel
		bits += bytespp;
		ptr++;
	    }
	}

	// FreeImage_Save(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, const char *filename, int flags FI_DEFAULT(0));
	char filename[256];
	strcpy(filename, basename);
	strcat(filename, ".png");
	if (FreeImage_Save(FIF_PNG, bitmap, filename, 0)) {
	    // bitmap successfully saved!
	}
	FreeImage_Unload(bitmap);
    }
  
    FreeImage_DeInitialise();
#endif
    // le code ci dessous crée et sauvegarde une image sous le nom output.png dans le repertoire d'execution
    { 
	FILE *fp = NULL;
	char filename[256];
	strcpy(filename, basename);
	strcat(filename, ".ppm");

	fp = fopen(filename, "w");

	if (fp) {
	    // création réussi
   
	    fprintf(fp, "P3\n");
	    fprintf(fp, "%d %d\n255\n", WIDTH, HEIGHT);
	    // La c'est pour faire la boucle

	    for(unsigned y = 0; y < HEIGHT; y++) {
		color *ptr = &(output_image[(HEIGHT-y-1)*WIDTH]);
		for(unsigned x = 0; x < WIDTH; x++) {
		    *ptr = vector_clamp(*ptr, 0.f, 1.f);
		    unsigned char r = ptr->x*255;
		    unsigned char g = ptr->y*255;
		    unsigned char b = ptr->z*255;
		    ptr++;
		    fprintf(fp, "%d %d %d  ", r, g, b);
		}
		fprintf(fp, "\n");
	    }
	    fclose(fp);
	}
    }



    return 0;
}
