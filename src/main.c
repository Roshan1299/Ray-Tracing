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
#ifdef FS
    qsort(colors, numColors, sizeof(unsigned int), compareColor);
#endif
    fscanf(inputFile, "%d", &background_colorIndex);

#ifndef FS
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

        Sphere *newSphere = createSphere(radius, position, (Vec3){1.0, 1.0, 1.0});
        if (newSphere != NULL) {
            addSphere(&world, newSphere);
        }
    }
#endif
    viewportWidth = initialize_camera(viewportHeight, focalLength, imageWidth, imageHeight);

#ifdef MS1
    Vec3 bgColor = {0.0,0.0,0.0};
    Vec3 result;
    fprintf(outputFile, "(%0.1f, %0.1f, %0.1f) + (%0.1f, %0.1f, %0.1f) = (%0.1f, %0.1f, %0.1f)\n",
            bgColor.x, bgColor.y, bgColor.z, lightPosition.x, lightPosition.y, lightPosition.z,
            (add(bgColor, lightPosition)).x, (add(bgColor, lightPosition)).y, (add(bgColor, lightPosition)).z);

    fprintf(outputFile, "(%0.1f, %0.1f, %0.1f) - (%0.1f, %0.1f, %0.1f) = (%0.1f, %0.1f, %0.1f)\n",
            bgColor.x, bgColor.y, bgColor.z, lightPosition.x, lightPosition.y, lightPosition.z,
            (subtract(bgColor, lightPosition)).x, (subtract(bgColor, lightPosition)).y, (subtract(bgColor, lightPosition)).z);

    result = scalarMultiply(viewportWidth, lightPosition);
    fprintf(outputFile, "2.7 * (%0.1f, %0.1f, %0.1f) = (%0.1f, %0.1f, %0.1f)\n",
            lightPosition.x, lightPosition.y, lightPosition.z,
            result.x, result.y, result.z);

    fprintf(outputFile, "normalize(%0.1f, %0.1f, %0.1f) = (%0.1f, %0.1f, %0.1f)\n",
            lightPosition.x, lightPosition.y, lightPosition.z,
            (normalize(lightPosition)).x, (normalize(lightPosition)).y, (normalize(lightPosition)).z);

    fprintf(outputFile, "\n");

    for (int i = 0; i < numSpheres; i++) {
        Sphere *sphere = world.spheres[i];

        result = scalarDivide(sphere->color, sphere->r);
        fprintf(outputFile, "(%0.1f, %0.1f, %0.1f) / %0.1f = (%0.1f, %0.1f, %0.1f)\n",
                sphere->color.x, sphere->color.y, sphere->color.z, sphere->r,
                result.x, result.y, result.z);

        float dotResult = dot(lightPosition, sphere->pos);
        fprintf(outputFile, "dot((%0.1f, %0.1f, %0.1f), (%0.1f, %0.1f, %0.1f)) = %0.1f\n",
                lightPosition.x, lightPosition.y, lightPosition.z,
                sphere->pos.x, sphere->pos.y, sphere->pos.z,
                dotResult);

        float distanceResult = distance(lightPosition, sphere->pos);
        fprintf(outputFile, "distance((%0.1f, %0.1f, %0.1f), (%0.1f, %0.1f, %0.1f)) = %0.1f\n",
                lightPosition.x, lightPosition.y, lightPosition.z,
                sphere->pos.x, sphere->pos.y, sphere->pos.z,
                distanceResult);

        float lengthResult = length(sphere->pos);
        fprintf(outputFile, "length(%0.1f, %0.1f, %0.1f) = %0.1f\n",
                sphere->pos.x, sphere->pos.y, sphere->pos.z,
                lengthResult);


	if (i < numSpheres - 1) {
        	fprintf(outputFile, "\n");
    }

       
    }
    for (int i = 0; i < world.size; i++) {
        free(world.spheres[i]); // Free each sphere
    }
    
#endif


#ifdef MS2
    fprintf(outputFile, "P3\n%d %d\n255\n", (int)imageWidth, (int)imageHeight);
    Vec3 cameraPosition = {0.0, 0.0, 0.0};

    for (int y = imageHeight-1; y >= 0; y--) {
        for (int x = 0; x < imageWidth; x++) {
            Vec3 pixelColor = {0.0,0.0,0.0};
            
            float closestIntersectionT = INFINITY;
            Sphere *closestSphere = NULL;

            float u = (x + 0.5) / imageWidth * viewportWidth - (viewportWidth / 2);
            float v = (y + 0.5) / imageHeight * viewportHeight - (viewportHeight / 2);

            Vec3 rayDirection = normalize((Vec3){u, v, -focalLength});

            for (int i = 0; i < world.size; i++) {
                float t;
                if (doesIntersect(world.spheres[i], cameraPosition, rayDirection, &t) && t < closestIntersectionT) {
                    closestIntersectionT = t;
                    closestSphere = world.spheres[i];
                }
            }

            if (closestSphere != NULL) {
                Vec3 intersectionPoint = add(cameraPosition, scalarMultiply(closestIntersectionT, rayDirection));
                Vec3 normal = normalize(subtract(intersectionPoint, closestSphere->pos));
                Vec3 lightDirection = normalize(subtract(lightPosition, intersectionPoint));

                float lightIntensity = lightBrightness * fmax(0, dot(normal, lightDirection)) /
                                  pow(distance(lightPosition, intersectionPoint), 2);
                lightIntensity = fmin(1.0, lightIntensity);

                Vec3 shadowRayOrigin = add(intersectionPoint, scalarMultiply(0.001, normal));
                int isInShadowed = 0;
                for (int i = 0; i < world.size; i++) {
                    if (world.spheres[i] != closestSphere) {
                        float t;
                        if (doesIntersect(world.spheres[i], shadowRayOrigin, lightDirection, &t)) {
                            isInShadowed = 1;
                            break;
                        }
                    }
                }

                if (isInShadowed) {
                    lightIntensity *= 0.1; // Shadow factor
                }

                // Final color
                pixelColor = scalarMultiply(lightIntensity, closestSphere->color);
            }

            // Write the color to the file
            fprintf(outputFile, "%d %d %d ",
                (int)(pixelColor.x * 255),
                (int)(pixelColor.y * 255),
                (int)(pixelColor.z * 255));
        }
        fprintf(outputFile, "\n"); // Newline for each row
    }
    for (int i = 0; i < world.size; i++) {
        free(world.spheres[i]); // Free each sphere
    }
#endif

#ifdef FS
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


#endif
    
    free(world.spheres);
    free(colors);
    fclose(inputFile);
    fclose(outputFile);


    return 0;
}
