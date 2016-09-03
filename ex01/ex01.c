#include "mo815-3dvis.h"
#include "bitmap.h"

#define RADIOLOGIST 'R'
#define NEUROSURGEON 'N'
#define AXIAL 'A'
#define SAGITTAL 'S'
#define CORONAL 'C'

typedef enum {
	X,
	Y,
	Z
} AxisType;

typedef enum {
	false,
	true
} bool;

typedef struct {
	AxisType axis;
	bool inverted;
} AxisOrientation;

typedef struct {
	AxisOrientation fixedAxis;
	AxisOrientation firstAxis;
	AxisOrientation secondAxis;
	int start;
} Axis;

Axis getAxis(AxisType fixedAxis, AxisType firstAxis, bool invertedFixedAxis, bool invertedFirstAxis, bool invertedSecondAxis, int start) {
	Axis axis;

	axis.start = start;
	AxisOrientation fixed;
	fixed.axis = fixedAxis;
	fixed.inverted = invertedFixedAxis;
	axis.fixedAxis = fixed;

	AxisOrientation first;
	first.axis = firstAxis;
	first.inverted = invertedFirstAxis;
	axis.firstAxis = first;

	AxisOrientation second;
	if (fixedAxis == X) {
		second.axis = firstAxis == Y ? Z : Y;
	} else if (fixedAxis == Y) {
		second.axis = firstAxis == X ? Z : X;
	} else {
		second.axis = firstAxis == X ? Z : X;
	}
	second.inverted = invertedSecondAxis;
	axis.secondAxis = second;

	return axis;
}

Axis getAxisForModeAndCut(char mode, char cut, int start) {
	if (cut == 'C') {
		return getAxis(Y, X, false, false, true, start);
	}

	if (mode == 'R') {
		if (cut == 'A') {
			return getAxis(Z, X, false, false, false, start);
		}
		return getAxis(X, Y, false, true, true, start);
	}

	if (cut == 'A') {
		return getAxis(Z, X, true, true, false, start);
	}
	return getAxis(X, Y, true, false, true, start);
}

int getVoxelX(Image *img, Axis axis, int row, int col) {
	if (axis.fixedAxis.axis == X) {
		return axis.fixedAxis.inverted ? img->xsize - axis.start : axis.start;
	}

	if (axis.firstAxis.axis == X) {
		return axis.firstAxis.inverted ? img->xsize - col : col;
	}

	return axis.secondAxis.inverted ? img->xsize - row : row;
}

int getVoxelY(Image *img, Axis axis, int row, int col) {
	if (axis.fixedAxis.axis == Y) {
		return axis.fixedAxis.inverted ? img->ysize - axis.start : axis.start;
	}

	if (axis.firstAxis.axis == Y) {
		return axis.firstAxis.inverted ? img->ysize - col : col;
	}

	return axis.secondAxis.inverted ? img->ysize - row : row;
}

int getVoxelZ(Image *img, Axis axis, int row, int col) {
	if (axis.fixedAxis.axis == Z) {
		return axis.fixedAxis.inverted ? img->zsize - axis.start : axis.start;
	}

	if (axis.firstAxis.axis == Z) {
		return axis.firstAxis.inverted ? img->zsize - col : col;
	}

	return axis.secondAxis.inverted ? img->zsize - row : row;
}

Voxel getVoxel(Image *img, Axis axis, int row, int col) {
	Voxel v;

	v.x = getVoxelX(img, axis, row, col);
	v.y = getVoxelY(img, axis, row, col);
	v.z = getVoxelZ(img, axis, row, col);

	return v;
}

int getColumns(Image *img, Axis axis) {
	switch (axis.firstAxis.axis) {
		case X:
			return img->xsize;
		case Y:
			return img->ysize;
		default:
			return img->zsize;
	}
}

int getLines(Image *img, Axis axis) {
	switch (axis.secondAxis.axis) {
		case X:
			return img->xsize;
		case Y:
			return img->ysize;
		default:
			return img->zsize;
	}
}

void drawCut(Image *img, Axis axis, char* outputFileName) {
	int columns = getColumns(img, axis);
	int lines = getLines(img, axis);
	int row, col;
	Voxel v;

	float **cut = (float**) malloc(sizeof(float*) * lines);
	for (row = 0; row < lines; row++) {
		cut[row] = (float*) malloc(sizeof(float) * columns);
		for (col = 0; col < columns; col++) {
			v = getVoxel(img, axis, row, col);
			cut[row][col] = img->val[v.z * img->ysize * img->xsize + v.y * img->xsize + v.x];
		}
	}

	saveImage(outputFileName, cut, columns, lines);

	for (row = 0; row < lines; row++) {
		free(cut[row]);
	}
	free(cut);
}

void test(Image *img, char mode, char cut, int start, char* outputFileNamePattern) {
	char outputFileName[200];
	sprintf(outputFileName, "%s_%c_%c_%04d", outputFileNamePattern, mode, cut, start);
	drawCut(img, getAxisForModeAndCut(mode, cut, start), outputFileName);
}

int main(int argc, char *argv[]) {
	Image *img;
	timer *t1, *t2;
	int i;

	/*--------------------------------------------------------*/
	void *trash = malloc(1);
	struct mallinfo info;
	int MemDinInicial, MemDinFinal;
	free(trash);
	info = mallinfo();
	MemDinInicial = info.uordblks;

	/*--------------------------------------------------------*/

	if (argc != 4) {
		Error("Usage: ex01.o <input.scn> <outputFileNamePattern> <start>", "ex01.c - main");
	}

	t1 = Tic();

	img = ReadImage(argv[1]);

	printf("Image size: %d x %d x %d\n", img->xsize, img->ysize, img->zsize);

	for (i = 0; i < img->zsize; i++) {
		test(img, RADIOLOGIST, AXIAL, i, argv[2]);
		test(img, NEUROSURGEON, AXIAL, i, argv[2]);
	}

	for (i = 0; i < img->xsize; i++) {
		test(img, RADIOLOGIST, SAGITTAL, i, argv[2]);
		test(img, NEUROSURGEON, SAGITTAL, i, argv[2]);
	}

	for (i = 0; i < img->ysize; i++) {
		test(img, RADIOLOGIST, CORONAL, i, argv[2]);
		test(img, NEUROSURGEON, CORONAL, i, argv[2]);
	}

	t2 = Toc();
	fprintf(stdout, "Done in %f ms\n",CompTime(t1,t2));

	DestroyImage(img);

	/* ------------------------------------------------------ */

	info = mallinfo();
	MemDinFinal = info.uordblks;
	if (MemDinInicial != MemDinFinal) {
		printf("\n\nDinamic memory was not completely deallocated (%d, %d)\n", MemDinInicial, MemDinFinal);
	}

	return 0;
}