#include "c3d.h"

#include <stdlib.h>
#include "math.h"

void c3d_zeroMatrix(c3d_4x4Matrix* matrix) {
    c3d_size i;
    for (i = 0; i < 4; i++) {
        c3d_size j;
        for (j = 0; j < 4; j++) {
            (*matrix)[i][j] = 0;
        }
    }
}

void c3d_multiplyMatricies(c3d_4x4Matrix* m1, c3d_4x4Matrix* m2, c3d_4x4Matrix* mOut) {
    c3d_size i;
    c3d_zeroMatrix(mOut);

    for (i = 0; i < 4; i++) {
        c3d_size j;
        for (j = 0; j < 4; j++) {
            c3d_size k;
            (*mOut)[i][j] = 0;

            for (k = 0; k < 4; k++) {
                (*mOut)[i][j] = (*mOut)[i][j] + (*m1)[i][k] * (*m2)[k][j];
            }
        }
    }
}

signed char fcompare(float f1, float f2) {
    return (signed char)fcmp(f1, f2);
}

c3d_4x4Matrix xrot;
c3d_4x4Matrix yrot;
c3d_4x4Matrix zrot;
c3d_4x4Matrix temp;

void c3d_makeRotationMatrix(c3d_vector3* rotation, c3d_4x4Matrix* receiver) {
    xrot[0][0] = itof(1); xrot[0][1] =           itof(0); xrot[0][2] =                           itof(0); xrot[0][3] = itof(0);
    xrot[1][0] = itof(0); xrot[1][1] = fcos(rotation->x); xrot[1][2] = fmul(itof(-1), fsin(rotation->x)); xrot[1][3] = itof(0);
    xrot[2][0] = itof(0); xrot[2][1] = fsin(rotation->x); xrot[2][2] =                 fcos(rotation->x); xrot[2][3] = itof(0);
    xrot[3][0] = itof(0); xrot[3][1] =           itof(0); xrot[3][2] =                           itof(0); xrot[3][3] = itof(1);

    yrot[0][0] =                 fcos(rotation->y); yrot[0][1] = itof(0); yrot[0][2] = fsin(rotation->y); yrot[0][3] = itof(0);
    yrot[1][0] =                           itof(0); yrot[1][1] = itof(1); yrot[1][2] =           itof(0); yrot[1][3] = itof(0);
    yrot[2][0] = fmul(itof(-1), fsin(rotation->y)); yrot[2][1] = itof(0); yrot[2][2] = fcos(rotation->y); yrot[2][3] = itof(0);
    yrot[3][0] =                           itof(0); yrot[3][1] = itof(0); yrot[3][2] =           itof(0); yrot[3][3] = itof(1);

    zrot[0][0] = fcos(rotation->z); zrot[0][1] = fmul(itof(-1), fsin(rotation->z)); zrot[0][2] = itof(0); zrot[0][3] = itof(0);
    zrot[1][0] = fsin(rotation->z); zrot[1][1] =                 fcos(rotation->z); zrot[1][2] = itof(0); zrot[1][3] = itof(0);
    zrot[2][0] =           itof(0); zrot[2][1] =                           itof(0); zrot[2][2] = itof(1); zrot[2][3] = itof(0);
    zrot[3][0] =           itof(0); zrot[3][1] =                           itof(0); zrot[3][2] = itof(0); zrot[3][3] = itof(1);

    c3d_multiplyMatricies(&xrot, &yrot, &temp);
    c3d_multiplyMatricies(&temp, &zrot, receiver);
}

// assumes vector is homogeneous with a
// w-value of 1
void c3d_multiplyVectorByMatrix(c3d_vector3* vector, c3d_4x4Matrix* matrix, c3d_vector3* vOut) {
    c3d_float w;
    c3d_vector3 backupVector;
    backupVector.x = vector->x;
    backupVector.y = vector->y;
    backupVector.z = vector->z;

    w = fadd(fadd(fadd(fmul(backupVector.x, (*matrix)[0][3]), fmul(backupVector.y, (*matrix)[1][3])), fmul(backupVector.z, (*matrix)[2][3])), (*matrix)[3][3]);
    vOut->x = fadd(fadd(fadd(fmul(backupVector.x, (*matrix)[0][0]), fmul(backupVector.y, (*matrix)[1][0])), fmul(backupVector.z, (*matrix)[2][0])), (*matrix)[3][0]);
    vOut->y = fadd(fadd(fadd(fmul(backupVector.x, (*matrix)[0][1]), fmul(backupVector.y, (*matrix)[1][1])), fmul(backupVector.z, (*matrix)[2][1])), (*matrix)[3][1]);
    vOut->z = fadd(fadd(fadd(fmul(backupVector.x, (*matrix)[0][2]), fmul(backupVector.y, (*matrix)[1][2])), fmul(backupVector.z, (*matrix)[2][2])), (*matrix)[3][2]);

    if (fcompare(w, itof(1)) != 0 && fcompare(w, itof(0)) != 0) {
       vOut->x = fdiv(vOut->x, w);
       vOut->y = fdiv(vOut->y, w);
       vOut->z = fdiv(vOut->z, w);
    }
}

void c3d_addVectors(c3d_vector3* v1, c3d_vector3* v2, c3d_vector3* vOut) {
    vOut->x = fadd(v1->x, v2->x);
    vOut->y = fadd(v1->y, v2->y);
    vOut->z = fadd(v1->z, v2->z);
}

void c3d_subtractVectors(c3d_vector3* v1, c3d_vector3* v2, c3d_vector3* vOut) {
    vOut->x = fsub(v1->x, v2->x);
    vOut->y = fsub(v1->y, v2->y);
    vOut->z = fsub(v1->z, v2->z);
}

bool isVertexVisible(c3d_vector3* vertex, c3d_float perspectiveScaler, c3d_float nearClip, c3d_float farClip) {
    c3d_float max = vertex->z * perspectiveScaler;

    return fcompare(vertex->z, nearClip) > 0
           && fcompare(vertex->z, farClip) < 0
           && fcompare(_fabs(vertex->x), max) < 0
           && fcompare(_fabs(vertex->y), max) < 0;
}

c3d_4x4Matrix cameraRotationMatrix;
c3d_vector3 cameraTranslation;
int modelIndex;
c3d_4x4Matrix modelRotationMatrix;
c3d_size vertexIndex;
c3d_model model;
c3d_vector3* projectedVertices;
c3d_vector3 modelloc;
c3d_size faceIndex;

void c3d_project(c3d_camera* camera, const c3d_model models[], size_t numModels, void (*onProjectedTriangle)(c3d_projectedTriangle)) {
    c3d_float tanfov = _fabs(ftan(fdiv(camera->fov, itof(2))));
    c3d_float nearPlaneWidth = fmul(tanfov, camera->clipNear);
    c3d_float farPlaneWidth = fmul(tanfov, camera->clipFar);
    c3d_float perspectiveScaleFactor = fdiv(fsub(farPlaneWidth, nearPlaneWidth), fsub(camera->clipFar, camera->clipNear));
    c3d_float halfPerspectiveScale = fdiv(perspectiveScaleFactor, itof(40));
    c3d_float halfWidth = fdiv(camera->width, itof(2));
    c3d_float halfHeight = fdiv(camera->height, itof(2));
    c3d_float pixelScale = fdiv(camera->width, fmul(nearPlaneWidth, itof(2)));
    c3d_makeRotationMatrix(&camera->rot, &cameraRotationMatrix);

    cameraTranslation.x = fmul(camera->loc.x, itof(-1));
    cameraTranslation.y = fmul(camera->loc.y, itof(-1));
    cameraTranslation.z = fmul(camera->loc.z, itof(-1));

    for (modelIndex = 0; modelIndex < numModels; modelIndex++) {
        model = models[modelIndex];
        projectedVertices = malloc(sizeof(c3d_vector3) * model.numVertices);
        c3d_addVectors(&cameraTranslation, &model.loc, &modelloc);

        c3d_makeRotationMatrix(&model.rot, &modelRotationMatrix);

        for (vertexIndex = 0; vertexIndex < model.numVertices; vertexIndex++) {
            c3d_vector3* vert;
            c3d_vector3 vertex;
            c3d_float scalefactor;
            c3d_float z;

            vert = &model.vertices[vertexIndex];

            // rotate relative to model center
            c3d_multiplyVectorByMatrix(vert, &modelRotationMatrix, &vertex);

            // translate relative to camera
            c3d_addVectors(&modelloc, &vertex, &vertex);

            // rotate relative to camera
            c3d_multiplyVectorByMatrix(&vertex, &cameraRotationMatrix, &vertex);

            // perspective scaling
            scalefactor = fmul(vertex.y, perspectiveScaleFactor);
            vertex.x = fdiv(vertex.x, scalefactor);
            vertex.z = fdiv(vertex.z, scalefactor);

            // flip z and y for convenience later
            z = vertex.z;
            vertex.z = vertex.y;
            vertex.y = z;

            projectedVertices[vertexIndex] = vertex;
        }

        for (faceIndex = 0; faceIndex < model.numFaces; faceIndex++) {
            c3d_face* face;
            c3d_vector3 v1;
            c3d_vector3 v2;
            c3d_vector3 v3;
            c3d_vector3 normal;
            bool allVerticesVisible;

            face = &model.faces[faceIndex];
            v1 = projectedVertices[face->vertex1];
            v2 = projectedVertices[face->vertex2];
            v3 = projectedVertices[face->vertex3];

            // note: Normals here are not really correct.
            //       They are just being rotated with the rest of the model.
            //       This does not take into account any perspective skewing
            //       or, in the future, vertex transformations. This should
            //       be good enough for back-face culling, though. The main
            //       problem it has is flat planes facing cardinal directions.

            c3d_multiplyVectorByMatrix(&model.normals[faceIndex], &modelRotationMatrix, &normal);
            c3d_multiplyVectorByMatrix(&normal, &cameraRotationMatrix, &normal);

            allVerticesVisible = isVertexVisible(&v1, halfPerspectiveScale, camera->clipNear, camera->clipFar)
                || isVertexVisible(&v2, halfPerspectiveScale, camera->clipNear, camera->clipFar)
                || isVertexVisible(&v3, halfPerspectiveScale, camera->clipNear, camera->clipFar);

            if (allVerticesVisible) {
                c3d_float midx = fdiv((fadd(fadd(v1.x, v2.x), v3.x)), itof(3));
                c3d_float midy = fdiv((fadd(fadd(v1.y, v2.y), v3.y)), itof(3));
                c3d_float midz = fdiv((fadd(fadd(v1.z, v2.z), v3.z)), itof(3));

                c3d_float screen_v1_x = fadd(fmul(v1.x, pixelScale), halfWidth);
                c3d_float screen_v2_x = fadd(fmul(v2.x, pixelScale), halfWidth);
                c3d_float screen_v3_x = fadd(fmul(v3.x, pixelScale), halfWidth);

                c3d_float screen_v1_y = fsub(camera->height, fadd(fmul(v1.y, pixelScale), halfHeight));
                c3d_float screen_v2_y = fsub(camera->height, fadd(fmul(v2.y, pixelScale), halfHeight));
                c3d_float screen_v3_y = fsub(camera->height, fadd(fmul(v3.y, pixelScale), halfHeight));

                c3d_projectedTriangle projectedTriangle;
                projectedTriangle.modelIndex = modelIndex;
                projectedTriangle.faceIndex = faceIndex;
                projectedTriangle.distance = fadd(fadd(fmul(midx, midx), fmul(midy, midy)), fmul(midz, midz));

                projectedTriangle.vertex1.x = screen_v1_x;
                projectedTriangle.vertex1.y = screen_v1_y;
                projectedTriangle.vertex1.z = v1.z;

                projectedTriangle.vertex2.x = screen_v2_x;
                projectedTriangle.vertex2.y = screen_v2_y;
                projectedTriangle.vertex2.z = v2.z;

                projectedTriangle.vertex3.x = screen_v3_x;
                projectedTriangle.vertex3.y = screen_v3_y;
                projectedTriangle.vertex3.z = v3.z;

                onProjectedTriangle(projectedTriangle);
            }
        }

        free(projectedVertices);
    }
}
