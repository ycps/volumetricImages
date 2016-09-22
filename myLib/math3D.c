#include <stdio.h>
#include <stdlib.h>

#include "math3D.h"

Point3D copy(Point3D p, Point3D q) {
	p->x = q->x;
	p->y = q->y;
	p->z = q->z;

	return p;
}

Point3D project(Vector3D planeRotation, Point3D p, int D) {
	// Point3D p2 = rotateX(p, createPoint3D(D, D, D), planeRotation->x, false);
	// p2 = rotateY(p2, createPoint3D(D, D, D), planeRotation->y, false);
	// p2->x += 0;
	// p2->y += 0;
	// p2->z = 0;

	return p;
}

Vector3D vectorProduct(Vector3D a, Vector3D b) {
	return createVector3D(
		a->y * b->z - a->z * b->y,
		a->z * b->x - a->x * b->z,
		a->x * b->y - a->y * b->x);
}

Vector3D calculateNormal(Vertices vertices, int vCount) {
	if (vCount <= 2) {
		return createVector3D(0, 0, 0);
	}
	Vector3D v0 = copy(createVector3D(0, 0, 0), vertices[0]);
	Vector3D v1 = copy(createVector3D(0, 0, 0), vertices[1]);
	Vector3D v2 = copy(createVector3D(0, 0, 0), vertices[2]);

	Vector3D a = translate(v1, v0, true);
	Vector3D b = translate(v2, v0, true);

	return normalizedVector3D(vectorProduct(a, b));
}

Vector3D calculateFaceNormal(Face face) {
	face->normal = calculateNormal(face->vertices, 4);
	if (face->invertedNormal) {
		scale(face->normal, createVector3D(-1, -1, -1), false);
	}

	return face->normal;
}

float innerProduct(Vector3D a, Vector3D b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

float moduleOfVector3D(Vector3D a) {
	return sqrt(innerProduct(a, a));
}

Vector3D normalizedVector3D(Vector3D a) {
	float module = moduleOfVector3D(a);
	if (-0.00001 < module && module < 0.00001) {
		a->x = 0;
		a->y = 0;
		a->z = 0;

		return a;
	}

	a->x /= module,
	a->y /= module,
	a->z /= module;

	return a;
}

Point3D translate(Point3D p, Vector3D inc, bool inverse) {
	p->x += (inverse ? -1.0 : 1.0) * inc->x,
	p->y += (inverse ? -1.0 : 1.0) * inc->y,
	p->z += (inverse ? -1.0 : 1.0) * inc->z;

	return p;
}

Point3D scale(Point3D p, Vector3D factor, bool inverse) {
	p->x *= (inverse ? 1.0 / factor->x : factor->x),
	p->y *= (inverse ? 1.0 / factor->y : factor->y),
	p->z *= (inverse ? 1.0 / factor->z : factor->z);

	return p;
}

Point3D rotateZ(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;

	translate(p, origin, true);

	Point3D q = createPoint3D(
		p->x * COS(theta) + p->y * SIN(theta),
		-p->x * SIN(theta) + p->y * COS(theta),
		p->z);

	translate(q, origin, false);

	return copy(p, q);
}

Point3D rotateY(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;

	translate(p, origin, true);

	Point3D q = createPoint3D(
		p->x * COS(theta) + p->z * SIN(theta),
		p->y,
		-p->x * SIN(theta) + p->z * COS(theta));

	translate(q, origin, false);

	return copy(p, q);
}

Point3D rotateX(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;

	translate(p, origin, true);

	Point3D q = createPoint3D(
		p->x,
		p->y * COS(theta) - p->z * SIN(theta),
		p->y * SIN(theta) + p->z * COS(theta));

	translate(q, origin, false);

	return copy(p, q);
}

bool samePoint(Point3D a, Point3D b) {
	return a->x == b->x && a->y == b->y && a->z == b->z;
}

Point3D createPoint3D(float x, float y, float z) {
	Point3D p = (Point3D) malloc(sizeof(_point3D));

	p->x = x;
	p->y = y;
	p->z = z;

	return p;
}

Vertex createVertex(float x, float y, float z) {
	return createPoint3D(x, y, z);
}

Vector3D createVector3D(float x, float y, float z) {
	return createPoint3D(x, y, z);
}

Edge createEdge(Vertex a, Vertex b) {
	Edge e = (Edge) malloc(sizeof(_edge));

	e->a = a;
	e->b = b;

	return e;
}

Face createFace(Vertices vertices, bool invertedNormal) {
	int i;

	Face f = (Face) malloc(sizeof(_face));

	f->invertedNormal = invertedNormal;
	f->vertices = (Vertices) malloc(sizeof(Vertex) * 4);

	for (i = 0; i < 4; i++) {
		f->vertices[i] = vertices[i];
	}

	f->normal = calculateFaceNormal(f);

	return f;
}

char* toStringPoint3D(Point3D p) {
	char* str = (char*) malloc(sizeof(char) * (2 + 2 + 2 + 3 * (1 + 4 + 1 + 4)));
	sprintf(str, "(%.4f, %.4f, %.4f)", p->x, p->y, p->z);

	return str;
}
