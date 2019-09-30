#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> 

int matrix[1000][1000] = {{0}};
int ordered_summits[1000];
int colors[1000] = {0};
int temp_colors[1000] = {0};
int size, current_color, current_summit;
void print_matrix();
void print_degrees();
void print_colors();
void print_temp_colors();
void print_neighbours();
int incoherences();
void down_count_colors();
int recuit_simule();

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
			sscanf (buf, "p edge %d", &size);
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
	int summit_dsat, current_dsat = 0;
	int dsat_summit = 0;
	for (int summit=0; summit<size; summit++) {
		if (colors[summit] == 0) {
			current_dsat = dsat(summit);
			if (current_dsat > summit_dsat) {
				summit_dsat = current_dsat;
				dsat_summit = summit;
			} else if (current_dsat == summit_dsat) {
				if (nb_neighbours(dsat_summit) > nb_neighbours(summit)) {
					summit_dsat = current_dsat;
					dsat_summit = summit;
				}
			}
		}
	}
	return dsat_summit;
}

void write_solution(char input[]) {
	FILE *ofp;
	int l = strlen(input) + 4;
	char output[l];
    strcpy(output, "solutions/");
    strcat(output, input);
    memmove(output + 10, input + 6, l-5);
    output[l-3] = 's';
   	ofp = fopen(output, "w+");
   	for (int i=0; i<size; i++) {
   		fprintf(ofp, "%d\n", colors[i]);
   	}
   	fclose(ofp);
}


/* Main function */

int main() {

	srand(time(NULL));
    // time_t start = time(NULL);
    // time_t running_time = 60; // _s

	char input[] = "color/dsjc1000.9.col";
	if (readdata(input) == 1) {
		printf("Looks like the file didn't open \n");
		return 1;
	}
	get_ordered_degrees();
	current_summit = ordered_summits[0];
	while(highest_not_colored_summit() != -1) {
		colors[current_summit] = lowest_possible_color(current_summit);
		current_summit = dsat_max();
	}

	int chromatic_number = max(colors, size);

	printf("Chromatic number is %d \n", chromatic_number);
	write_solution(input);

	// while (time(NULL) - start < running_time) {
	// 	down_count_colors(chromatic_number);

	// 	int e = recuit_simule(chromatic_number);

	// 	if (e == 0) {
	// 		write_solution(input);
	// 		chromatic_number--;
	// 		printf("Found %d at %d _s\n", chromatic_number, time(NULL) - start);
	// 	}
	// }

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
		printf("%d ", colors[k]);
	}
	printf("\n");
}

void print_temp_colors() {
	for(int k=0; k<size; k++) {
		printf("%d ", temp_colors[k]);
	}
	printf("\n");
}

void print_neighbours(int summit) {
	int n, *_neighbours;
	_neighbours = neighbours(summit);
	n = nb_neighbours(summit);
	for(int k=0; k<n; k++) {
		printf("%d ", *(_neighbours + k));
	}
}


/* Optimization */

int incoherences() {
	int result = 0;
	for (int i=0; i<size; i++) {
		for (int j=0; j<i; j++) {
			if (matrix[i][j] == 1) {
				if (temp_colors[i] == temp_colors[j]) {
					result++;
				}
			}
		}
	}
	return result;
}

void down_count_colors(int color) {
	for (int i=0; i<size; i++) {
		if (colors[i] == color) {
			colors[i] = rand() % color;
		}
	}
}

void generate_coloration(int chromatic_number) {
	memcpy(temp_colors, colors, size*sizeof(int));
	int summit = rand() % size;
	temp_colors[summit] = rand() % chromatic_number;
}

float random() {
	float x = (float)rand()/(float)(RAND_MAX);
	return x;
}

/* 
s := s0
e := E(s)
k := 0
tant que k < kmax et e > emax
  sn := voisin(s)
  en := E(sn)
  si en < e ou aléatoire() < P(en - e, temp(k/kmax)) alors
    s := sn; e := en
  k := k + 1
retourne s
*/

int recuit_simule(int color) {
	int kmax = 500000;
	int en;
	memcpy(temp_colors, colors, size*sizeof(int));
	int e = incoherences();
	int k = 0;
	while (k<kmax && e>0) {
		generate_coloration(color);
		en = incoherences();
		if (en < e || random() < 0.001) { // exp(en-e/(k/kmax))
			memcpy(colors, temp_colors, size*sizeof(int));
			e = en; 
		}
		k++;
	}
	printf("Final e is %d\n", e);
	return e;
}