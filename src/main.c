#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "spheres.h"
#include <math.h>
#include "color.h"

float initialize_camera(float viewportHeight, float focalLength, int imageWidth, int imageHeight) {
    float aspect_ratio = (float)imageWidth / (float)imageHeight;
    return viewportHeight * aspect_ratio;
}

int main(int argc, char *argv[]) {
    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");
    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "Error opening files: %s or %s\n", argv[1], argv[2]);
        if (inputFile) fclose(inputFile);
        if (outputFile) fclose(outputFile);
        return 1;
    }

    int numColors, numSpheres, background_colorIndex;
    float imageWidth, imageHeight, viewportHeight,viewportWidth ,focalLength;
    Vec3 lightPosition;
    float lightBrightness;

    if (fscanf(inputFile, "%f %f", &imageWidth, &imageHeight) != 2) {
        fprintf(stderr, "Error: Failed to read image width and height.\n");
        fclose(inputFile);
        return 1;
    }

    if (fscanf(inputFile, "%f", &viewportHeight) != 1) {
        fprintf(stderr, "Error: Failed to read viewport height.\n");
        fclose(inputFile);
        return 1;
    }

    if (fscanf(inputFile, "%f", &focalLength) != 1) {
        fprintf(stderr, "Error: Failed to read focal length.\n");
        fclose(inputFile);
        return 1;
    }

    if (fscanf(inputFile, "%f %f %f %f", &lightPosition.x, &lightPosition.y, &lightPosition.z, &lightBrightness) != 4) {
        fprintf(stderr, "Error: Failed to read light position and brightness.\n");
        fclose(inputFile);
        return 1;
    }

    if (fscanf(inputFile, "%d", &numColors) != 1) {
        fprintf(stderr, "Error: Failed to read number of colors.\n");
        fclose(inputFile);
        return 1;
    }
	
    unsigned int *colors = malloc(numColors * sizeof(unsigned int));
    if (!colors) {
        fprintf(stderr, "Error: Could not allocate memory for colors.\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    for (int i = 0; i < numColors; i++) {
        if (fscanf(inputFile, "%x", &colors[i]) != 1) {
            fprintf(stderr, "Error reading colors.\n");
            fclose(inputFile);
            return 1;
        }
    }

    qsort(colors, numColors, sizeof(unsigned int), compareColor);

    fscanf(inputFile, "%d", &background_colorIndex);

    viewportWidth = initialize_camera(viewportHeight, focalLength, imageWidth, imageHeight);

    World world;
    worldInit(&world);
    fscanf(inputFile, "%d", &numSpheres);

    for (int i = 0; i < numSpheres; i++) {
        Vec3 position;
        float radius;
        int colorIndex;
        if (fscanf(inputFile, "%f %f %f %f %d", &position.x, &position.y, &position.z, &radius, &colorIndex) != 5) {
        fprintf(stderr, "Error: Failed to read sphere data (position, radius, color index).\n");
        fclose(inputFile);
        return 1;
        }
	
	Vec3 sphereColor = unpackRGB(colors[colorIndex]);

        Sphere *newSphere = createSphere(radius, position,sphereColor);
	if (newSphere != NULL) {
            addSphere(&world, newSphere);
        }
    }

    Vec3 backgroundColor = unpackRGB(colors[background_colorIndex]);

    fprintf(outputFile, "P3\n%d %d\n255\n", (int)imageWidth, (int)imageHeight);
    Vec3 cameraPosition = {0.0, 0.0, 0.0};
    
    for (int y = 0; y < imageHeight; y++){
        for (int x = 0; x < imageWidth; x++) {
            Vec3 total_color = {0, 0, 0};
            
            for (int dy = 0; dy < 3; dy++) {
                for (int dx = 0; dx < 3; dx++) {
                    float pixel_x = x * 3 + dx;
                    float pixel_y = y * 3 + dy;
                    
                    float u = (pixel_x + 0.5f) * (viewportWidth / (imageWidth*3)) - (viewportWidth / 2);
                    float v = (viewportHeight/2) - (pixel_y + 0.5f) * (viewportHeight/(imageHeight*3));

                    Vec3 rayDirection = normalize((Vec3){u, v, -focalLength});

                    float closestIntersectionT = 1e30f;
                    Sphere *closest_sphere = NULL;
                    
                    for (int i = 0; i < world.size; i++) {
                        float t;
                        if (doesIntersect(world.spheres[i], cameraPosition, rayDirection, &t)) {
                            if (t < closestIntersectionT) {
                                closestIntersectionT = t;
                                closest_sphere = world.spheres[i];
                            }
                        }
                    }
                    
                    Vec3 sampleColor;
                    if (closest_sphere) {
                        Vec3 intersectionPoint = add(cameraPosition, scalarMultiply(closestIntersectionT, rayDirection));
                        Vec3 normal = normalize(subtract(intersectionPoint, closest_sphere->pos));
                        Vec3 lightDirection = normalize(subtract(lightPosition, intersectionPoint));
                        
                        float dotProduct = fmax(dot(lightDirection, normal), 0.0f);
                        float distSquared = distance2(lightPosition, intersectionPoint);
                        float intensity = lightBrightness * dotProduct / distSquared;
                        intensity = fmin(1.0f, intensity);
                        
                        Vec3 shadowRayPos = add(intersectionPoint, scalarMultiply(0.001f, normal));
                        for (int i = 0; i < world.size; i++) {
                            float shadowT;
                            if (doesIntersect(world.spheres[i], shadowRayPos, lightDirection, &shadowT)) {
                                intensity *= 0.1f;
                                break;
                            }
                        }
                        
                        sampleColor = (Vec3){
                            intensity * closest_sphere->color.x,
                            intensity * closest_sphere->color.y,
                            intensity * closest_sphere->color.z
                        };
                    } else {
                        sampleColor = backgroundColor;
                    }
                    
                    total_color = add(total_color, sampleColor);
                }
            }
            
            Vec3 pixelColor = scalarDivide(total_color, 9.0f);
            writeColour(outputFile, pixelColor);
        }
	fprintf(outputFile, "\n");
    }
    for (int i = 0; i < world.size; i++) {
        free(world.spheres[i]); // Free each sphere
    }
    
    free(world.spheres);
    free(colors);
    fclose(inputFile);
    fclose(outputFile);


    return 0;
}
