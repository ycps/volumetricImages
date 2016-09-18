#include <stdlib.h>
#include <stdio.h>

#include "../ex01/bitmap.h"

#include "ex03.h"

void drawStar() {
	int i, max, step = 10;

	Image2D image = newImage2D(500, 500);
	max = image->width / 2 / step;

	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step + i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step + i * 10, 0), 255);

	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, 0, 0), createPoint3D(0, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, 0, 0), createPoint3D(image->width, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, image->height, 0), createPoint3D(0, max * step + i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, image->height, 0), createPoint3D(image->width, max * step + i * 10, 0), 255);

	saveImage("out/star", image->img, image->width, image->height);
	freeImage2D(image);
}

void drawSquare(Image2D image, Vertex a, Vertex b, Vertex c, Vertex d) {
	// printf("%f\t%f\n", a.x, a.y);
	// printf("%f\t%f\n", b.x, b.y);
	// printf("%f\t%f\n", c.x, c.y);
	// printf("%f\t%f\n", d.x, d.y);
	drawLine(image, a, b, 255);
	drawLine(image, b, c, 255);
	drawLine(image, c, d, 255);
	drawLine(image, d, a, 255);
}

void drawFace(Image2D image, Vertices vertices, int face, Vector3D planeRotation) {
	Point3D origin = createPoint3D(100, 100, 100);
	switch (face) {
		case 0:
			drawSquare(image, project(planeRotation, vertices[0], 100), project(planeRotation, vertices[1], 100), project(planeRotation, vertices[2], 100), project(planeRotation, vertices[3], 100));
			break;
		case 1:
			drawSquare(image, project(planeRotation, vertices[4], 100), project(planeRotation, vertices[5], 100), project(planeRotation, vertices[6], 100), project(planeRotation, vertices[7], 100));
			break;
		case 2:
			drawSquare(image, project(planeRotation, vertices[0], 100), project(planeRotation, vertices[4], 100), project(planeRotation, vertices[7], 100), project(planeRotation, vertices[3], 100));
			break;
		case 3:
			drawSquare(image, project(planeRotation, vertices[1], 100), project(planeRotation, vertices[5], 100), project(planeRotation, vertices[6], 100), project(planeRotation, vertices[2], 100));
			break;
		case 4:
			drawSquare(image, project(planeRotation, vertices[0], 100), project(planeRotation, vertices[1], 100), project(planeRotation, vertices[5], 100), project(planeRotation, vertices[4], 100));
			break;
		case 5:
			drawSquare(image, project(planeRotation, vertices[2], 100), project(planeRotation, vertices[3], 100), project(planeRotation, vertices[7], 100), project(planeRotation, vertices[6], 100));
			break;
	}
}

void testVisibleFaces(char* name, Vector3D planeRotation, Vector3D* normals, Vertices vertices) {
	int i;
	bool* visible = (bool*) malloc(sizeof(bool) * 6);
	visibleFaces(planeRotation, normals, visible, 6);

	Image2D image = newImage2D(500, 500);
	for (i = 0; i < 6; i++) {
		printf("%d\t", visible[i]);
	}
	printf("\n");
	for (i = 0; i < 6; i++) {
		if (visible[i]) {
			// printf("%d\n", i);
			drawFace(image, vertices, i, planeRotation);
		}
	}

	saveImage(name, image->img, image->width, image->height);
	freeImage2D(image);
	free(visible);
}

void drawCube(Vector3D* normals, Vertices vertices) {
	char nome[200];
	float i;
	for (i = 0; i < 2 * PI; i += 0.1) {
		sprintf(nome, "out/cube_%f", i);
		testVisibleFaces(nome, createVector3D(i, i, 0), normals, vertices);
	}
}

int main(int argc, char* argv[]) {
	Vector3D* normals = (Vector3D*) malloc(sizeof(Vector3D) * 6);
	normals[0] = createVector3D( 0,  0, -1);
	normals[1] = createVector3D( 0,  0,  1);
	normals[2] = createVector3D(-1,  0,  0);
	normals[3] = createVector3D( 1,  0,  0);
	normals[4] = createVector3D( 0,  1,  0);
	normals[5] = createVector3D( 0, -1,  0);

	printf("samePoint: 1 == %d\n", samePoint(normals[0], normals[0]));
	printf("samePoint: 0 == %d\n", samePoint(normals[1], normals[0]));

	Vertices vertices = (Vertices) malloc(sizeof(Vertex) * 8);
	vertices[0] = createVertex( 50,  50,  50);
	vertices[1] = createVertex(150,  50,  50);
	vertices[2] = createVertex(150, 150,  50);
	vertices[3] = createVertex( 50, 150,  50);
	vertices[4] = createVertex( 50,  50, 150);
	vertices[5] = createVertex(150,  50, 150);
	vertices[6] = createVertex(150, 150, 150);
	vertices[7] = createVertex( 50, 150, 150);

	//drawStar();

	drawCube(normals, vertices);

	return 0;
}
