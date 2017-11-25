#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* compile: gcc exercise1.c -lm */
/* execute: ./a.out < input.txt */

typedef struct point {
	float x, y;
} Point;

/* Variables globales. En esta variables se almacenaran
   la cantidad de elementos leidos (size) y los valores
   leídos (points) */
Point *points;
int size;

/* Regresa la distancia entre dos puntos dados (p1, p2) */
float distance(const Point *p1, const Point *p2) {
	return sqrt( ((p2->x - p1->x) * (p2->x - p1->x)) +
		         ((p2->y - p1->y) * (p2->y - p1->y)) );
}

/* Carga los datos de entrada entandar (consola). Primero
   recibe la cantidad de elementos a leer (size), seguido por
   por 'size' pares de flotantes, los puntos a leer */
void load_data() {
	int i;
	float x, y;

	scanf("%i", &size);
	points = (Point*) malloc(size * sizeof(Point));

	for (i = 0; i < size; i++) {
		scanf("%f %f", &x, &y);
		points[i].x = x;
		points[i].y = y;
	}
}


/* Regresa el apuntador al punto leído (points) más
   cercano a p */
Point* get_near_from(const Point* p) {
	Point *near = &points[0];
	int i;

	for (i = 1; i < size; i++) {
		if (distance(p, &points[i]) < distance(p, near)) {
			near = &points[i];
		}
	}

	return near;
}


/* A IMPLEMENTAR
   Regresa un arreglo de puntos (array) conteniendo los
   últimos num elementos del arreglo points. */
Point* get_last_elements(int num) {
	Point* array = &points[size - num];

	return array;
}


/* Recibe un arreglo de puntos cualesquiera (array)y la cantidad
   de elementos a desplegar (num). Despliega los elementos en el
   formanto '(x, y)' */
void display_points(Point *array, int num) {
	int i;

	for (i = 0; i < num; i++) {
		printf("(%.2f, %.2f) ", array[i].x, array[i].y);
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	Point p = {0.0, 0.0};
	Point *lasts, *near;

	load_data();
	printf("first 20: ");
	display_points(points, 20);

	near = get_near_from(&p);
	printf("near = (%4.2f, %4.2f)\n", near->x, near->y);

	lasts = get_last_elements(20);
	printf("last 10: ");
	display_points(lasts, 10);

	free(points);
	return 0;
}
