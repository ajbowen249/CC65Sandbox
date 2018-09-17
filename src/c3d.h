#ifndef __C3D_H__
#define __C3D_H__

#include "float.h"
#include <stddef.h>

typedef float c3d_float;
typedef size_t c3d_size;
typedef unsigned char bool;

typedef struct  {
    c3d_float x;
    c3d_float y;
    c3d_float z;
} c3d_vector3;

typedef struct {
    c3d_size vertex1;
    c3d_size vertex2;
    c3d_size vertex3;
} c3d_face;

typedef struct {
    size_t numVertices;
    c3d_vector3* vertices;

    size_t numFaces;
    c3d_face* faces;
    c3d_vector3* normals;

    c3d_vector3 loc;
    c3d_vector3 rot;

    void* data;
} c3d_model;

typedef struct {
    c3d_size modelIndex;
    c3d_size faceIndex;

    c3d_vector3 vertex1;
    c3d_vector3 vertex2;
    c3d_vector3 vertex3;

    c3d_float distance;
} c3d_projectedTriangle;

typedef struct {
    c3d_vector3 loc;
    c3d_vector3 rot;
    c3d_float fov;
    c3d_float clipNear;
    c3d_float clipFar;
    c3d_size width;
    c3d_size height;
} c3d_camera;

typedef c3d_float c3d_4x4Matrix[4][4];

void c3d_project(c3d_camera* camera, const c3d_model models[], size_t numModels, void (*onProjectedTriangle)(c3d_projectedTriangle));

#endif // __C3D_H__
