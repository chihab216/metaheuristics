#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int matrix[1000][1000] = {{0}};
int ordered_summits[1000];
int colors[1000] = {0};
int size, second;
int current_color, current_summit;
void print_matrix();
void print_degrees();
void print_colors();
void print_neighbours();

int sum(int *arr, int n) { 
    int sum = 0;
    for (int i = 0; i < n; i++) {
    	sum += arr[i]; 
    }
    return sum; 
}

int max(int *arr, int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
    	if (max < arr[i]) {
    		max = arr[i];
    	} 
    }
    return max; 
}

int nb_neighbours(int summit) {
	int nb_neighbours;
	nb_neighbours = sum(matrix[summit], 1000);
	return nb_neighbours;
}

int compare (const void * a, const void * b) {
  return nb_neighbours(*(int*)b) - nb_neighbours(*(int*)a);
}

int highest_not_colored_summit() {
	for(int k=0; k<size; k++) {
		if (colors[ordered_summits[k]] == 0) {
			return ordered_summits[k];
		}
	}
	return -1;
}

int get_ordered_degrees() {
	for(int i=0; i<size; i++) {
		ordered_summits[i] = i;
	}
	qsort (ordered_summits, size, sizeof(int), compare);
	return 0;
}

int readdata (char* fic) {
	int i,j;
	char buf[1024];
	FILE * f = fopen (fic, "r");
	if (f == NULL) return 1;
	while (fgets (buf, 1024, f) != NULL) {
		if (buf[0] == 'e') {
			sscanf (buf, "e %d %d", &i, &j);
			matrix[j-1][i-1] = 1;
			matrix[i-1][j-1] = 1;
		} else if (buf[0] == 'p') {
			sscanf (buf, "p edge %d %d", &size, &second);
		}
	}
	fclose(f);
	return 0;
}

int * neighbours(int summit) {
	int current_neighbour = 0;
	int n = nb_neighbours(summit);
	int *neighbours = (int*) malloc(n * sizeof(int));
	for (int i=0; i<size; i++) {
		if (matrix[summit][i] == 1) {
			*(neighbours + current_neighbour) = i;
			current_neighbour++;
		}
	}
	return neighbours;
}

int lowest_possible_color(int summit) {
	int number_of_neighbours = nb_neighbours(summit);
	int *my_neighbours = neighbours(summit);
	int color = 1;
	while(color<1000) {
		int ok = 1;
		for (int i=0; i<number_of_neighbours; i++) {
			if (colors[*(my_neighbours + i)] == color) {
				ok = 0;
			}
		}
		if (ok == 1) {
			return color;
		}
		color++;
	}
	return 1000;
}

/* nombre de couleurs différentes dans les sommets adjacents */
int dsat(int summit) {
	int nneigh = nb_neighbours(summit);
	int neighbouring_colors[nneigh];

	memset(neighbouring_colors, 0, nneigh*sizeof(int));
	int *my_neighbours = neighbours(summit);
	for (int k=0; k<nneigh; k++) {
		int color = colors[*(my_neighbours + k)];
		if (color != 0) {
			neighbouring_colors[color] = 1;
		}
	}
	return sum(neighbouring_colors, nneigh);
}

int dsat_max() {
	int number_neighbours, summit_dsat = 0;
	int dsat_summit = highest_not_colored_summit();
	summit_dsat = dsat(summit_dsat);
	for (int summit=0; summit<size; summit++) {
		if (colors[summit] == 0) {
			number_neighbours = nb_neighbours(summit);
			if (number_neighbours > summit_dsat) {
				summit_dsat = dsat(summit);
				dsat_summit = summit;
			} else if (number_neighbours == summit_dsat) {
				if (nb_neighbours(dsat_summit) > nb_neighbours(summit)) {
					summit_dsat = dsat(summit);
					dsat_summit = summit;
				}
			}
		}
	}
	return dsat_summit;
}

/*int main() {
	int a[] = {1, 9, 3, 4};
	printf("%d\n", max(a, 4));
}

/* Main function */

int main() {
	readdata("C:/Users/admin/Desktop/dsjc125.1.col");
	get_ordered_degrees();
	
	current_summit = ordered_summits[0];

	while(highest_not_colored_summit() != -1) {
		colors[current_summit] = lowest_possible_color(current_summit);
		current_summit = dsat_max();
	}
	printf("Chromatic number is %d \n", max(colors, size));

	// printf("Current summit : %d \n", current_summit);
	// colors[83] = 1;
	// printf("Next summit : %d \n", highest_not_colored_summit());
	// printf("Its lowest possible color : %d \n", lowest_possible_color(current_summit));
	// printf("And the dsat summit : %d \n", dsat_max());


	return 0;
}




/* Other debugging functions */

void print_matrix() {
	for(int k=0; k<size; k++) {
		for(int l=0; l<size; l++) {
			printf("%d", matrix[k][l]);
		}
		printf("\n");
	}
}

void print_degrees() {
	for(int k=0; k<size; k++) {
		printf("%d %d \n", ordered_summits[k], nb_neighbours(ordered_summits[k]));
	}
}

void print_colors() {
	for(int k=0; k<size; k++) {
		printf("%d \n", colors[k]);
	}
}

void print_neighbours(int summit) {
	int n, *_neighbours;
	_neighbours = neighbours(summit);
	n = nb_neighbours(summit);
	for(int k=0; k<n; k++) {
		printf("%d ", *(_neighbours + k));
	}
}