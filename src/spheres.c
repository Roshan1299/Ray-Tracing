#include "spheres.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void worldInit(World *world) {
    world->spheres = malloc(sizeof(Sphere*));  
    if (world->spheres == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    world->size = 0;        
    world->capacity = 0;    
}

void freeWorld(World *world) {
    for (int i = 0; i < world->size; i++) {
        free(world->spheres[i]);  
    }
    free(world->spheres);  
    world->spheres = NULL;  
    world->size = 0;
    world->capacity = 0;
}

Sphere* createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *sphere = (Sphere*)malloc(sizeof(Sphere)); 
    if (!sphere) {
        fprintf(stderr, "Memory allocation failed for Sphere\n");
        return NULL;  
    }
    sphere->r = radius;
    sphere->pos = position;
    sphere->color = color;
    return sphere;
}

void addSphere(World *world, Sphere *newSphere) {
    world->spheres = realloc(world->spheres, (world->size + 1) * sizeof(Sphere*));
    if (!world->spheres) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }
    world->spheres[world->size] = newSphere;
    world->size++;
}


int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
    Vec3 sphereToRay = subtract(rayPos, sphere->pos);

    float a = dot(rayDir, rayDir);
    float b = 2 * dot(rayDir, sphereToRay);
    float c = dot(sphereToRay, sphereToRay) - sphere->r * sphere->r;

    float discriminant = b * b - (4 * a * c);

    if (discriminant < 0) {
        return 0;
    }

    float t1 = (-b - sqrt(discriminant)) / (2 * a);
    float t2 = (-b + sqrt(discriminant)) / (2 * a);

    if (t1 > 0 && t2 > 0) {
        *t = fmin(t1, t2);
    } else if (t1 > 0) {
        *t = t1;
    } else if (t2 > 0) {
        *t = t2;
    } else {
        return 0;
    }

    return 1;
}


