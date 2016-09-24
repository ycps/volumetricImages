#ifndef EX_03
#define EX_03

#include "../myLib/math3D.h"
#include "../myLib/cube.h"
#include "../myLib/myCommon.h"
#include "../myLib/bitmap.h"

void render(char* name, Vector3D planeRotation, Cube cube);

void drawLine2D(Image2D image, Point3D start, Point3D end, int intensity);

void drawLine(Image2D image, Point3D start, Point3D end, int intensity, bool ignoreZ);

void visibleFaces(Vector3D planeRotation, Face* faces, bool* visibleFaces, int nFaces);

#endif
