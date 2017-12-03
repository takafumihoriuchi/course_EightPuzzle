/******************************/
/*                            */
/*   a game of eight puzzle   */
/*   - by Takafumi Horiuchi   */
/*                            */
/******************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "eight_puzzle.h"
#include "iterative_deepening_search.h"

int main(void)
{
	int map[3][3];
	init_map(map);

	/*
	while (!is_completed(map)) {
		print_map(map);
		player_input(map);
	}
	print_map(map);
	printf("completed\n");
	*/

	int ids;
	ids = iterative_deepening_search(map);
	if (ids) printf("puzzle solved\n");
	else printf("failed to solve puzzle\n");

	return 0;
}

// '0 (zero)' in 'map[][3]' represent the open space
void init_map(int map[][3])
{
	int flag[9] = {0,0,0,0,0,0,0,0,0};
	int i, j, random_number;
	srand(time(NULL));
	// NOTE: may not be the most efficient way of randomly initializing
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			while (1) {
				random_number=rand()%10;
				if (flag[random_number] == 0) break;
			}
			map[i][j] = random_number;
			flag[random_number] = 1;
		}
	}
}

// returns 1 if puzzle completed, if not, return 0
int is_completed(int map[][3])
{
	return (map[0][0]==1 && map[0][1]==2 && map[0][2]==3
		 && map[1][0]==8 && map[1][1]==0 && map[1][2]==4
		 && map[2][0]==7 && map[2][1]==6 && map[2][2]==5);
}

void print_map(int map[][3])
{
	int i, j;
	putchar('\n');
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			printf(" %d ", map[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
}

void player_input(int map[][3])
{
	char move_zero_dir;
	int valid;
	while (1) {
		printf("direction (e,d,s,f) : ");
		// up:'e', down:'d', left:'s', right:'f'
		scanf(" %c", &move_zero_dir);
		// space before %c is crucial
		if (move_zero_dir=='e' || move_zero_dir=='d' || 
			move_zero_dir=='s' || move_zero_dir=='f') {
			valid = is_valid(map, move_zero_dir);
			if (valid) {
				operate(map, move_zero_dir);
				return;
			}
		}
		printf("invalid input\n");
	}
}

// precondition : 'direction' to 'map' is valid
// move space/zero one step to 'direction'
void operate(int map[][3], char direction)
{
	int zero_row, zero_col;
	zero_row = get_zero_row(map);
	zero_col = get_zero_col(map);
	switch (direction) {
		case 'e' :
			map[zero_row][zero_col] = map[zero_row-1][zero_col];
			map[zero_row-1][zero_col] = 0;
			break;
		case 'd' :
			map[zero_row][zero_col] = map[zero_row+1][zero_col];
			map[zero_row+1][zero_col] = 0;
			break;
		case 's' :
			map[zero_row][zero_col] = map[zero_row][zero_col-1];
			map[zero_row][zero_col-1] = 0;
			break;
		case 'f' :
			map[zero_row][zero_col] = map[zero_row][zero_col+1];
			map[zero_row][zero_col+1] = 0;
			break;
		default :
			break;
	}
}

// returns '1' if the player input is valid
int is_valid(int map[][3], char direction)
{
	int zero_row, zero_col;
	zero_row = get_zero_row(map);
	zero_col = get_zero_col(map);
	if (zero_row==0 && direction=='e') return 0;
	if (zero_row==2 && direction=='d') return 0;
	if (zero_col==0 && direction=='s') return 0;
	if (zero_col==2 && direction=='f') return 0;
	return 1;
}

// postcondition : certain to return 'i', no default return
int get_zero_row(int map[][3])
{
	int i, j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (map[i][j]==0) return i;
}

// postcondition : certain to return 'i', no default return
int get_zero_col(int map[][3])
{
	int i, j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (map[i][j]==0) return j;
}
