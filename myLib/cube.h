#ifndef CUBE_3D
#define CUBE_3D

#include "math3D.h"

typedef struct {
	Point3D origin;
	Vector3D size;
	Face* faces;
	Edge* edges;
	Vertices vertices;
	int nVertices;
	int nEdges;
	int nFaces;
} _cube;

typedef _cube* Cube;

Cube translateCube(Cube cube, Point3D p);
Cube scaleCube(Cube cube, Vector3D scaleFactor);
Cube rotateCube(Cube cube, Vector3D rotation);
Vector3D alignCube(Cube cube, Vector3D rotation);

Cube createCube(Point3D origin, Vector3D size);

void updateCube(Cube cube);

void destroyCube(Cube cube);

#endif
