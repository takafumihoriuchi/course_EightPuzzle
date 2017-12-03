// #include "eight_puzzle.h" // needed?

void iterative_deepening_search(int map[][3]);

typedef struct node {
	struct node *next;
	int map_data[][3];
} OPENSTACK;

typedef struct node {
	struct node *next;
	int map_data[][3];
} CLOSEDLIST;

OPENSTACK *open_head = NULL;
CLOSEDLIST *closed_head = NULL;

// precondition : receives a map (randomly initialized)
// postcondition : return 1 if solution is found, if not, return 0
void iterative_deepening_search(int map[][3])
{
	int limit = 0;
	int i, pop_success;
	int child[4][3][3];
	int child_flg[4];  // 0: up, 1:down, 2:left, 3:right
	int child_clear[4] = {0,0,0,0};

	// end if the initialized map is already completed
	if (is_completed(map)) return 1;
	// push first state to open stack
	push_to_open(map);

	while (1) {

		// pop top of stack and store in map
		pop_success = pop_from_open(map);
		if (!pop_success) return 0;

		copy_array2_to_array1(child_flg, child_clear);
		expand_node(child[0], 'e', 0, child_flg, map);  // up
		expand_node(child[1], 'd', 1, child_flg, map);  // down
		expand_node(child[2], 's', 2, child_flg, map);  // left
		expand_node(child[3], 'f', 3, child_flg, map);  // right
		// new child node is marked 1 in child_flg[]
		insert_to_closed(map);

		for (i=0; i<4; i++) {
			if (!child_flg[i]) continue;
			if (!(is_in_closed(child[i]))) push_to_open(child[i]);
		}

	}

}

// postcondition : return 1 if given child is in closed list, if not, return 0
int is_in_closed(int child[][3])
{
	CLOSEDLIST *point;
	point = closed_head;
	while (1) {
		if (is_equal_array((*point).map_data, child))
			return 1;
		if ((*point).next==NULL)
			return 0;
		point = (*point).next;
	}
}

// postcondition : return 1 if two arrays are equal, if not, return 0
int is_equal_array(int array1[][3], int array2[][3])
{
	int i, j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (array1[i][j]!=array2[i][j]) return 0;
	return 1;
}

void insert_to_closed(int map[][3])
{
	CLOSEDLIST *node;
	node = malloc(sizeof(CLOSEDLIST));
	check_malloc(node);
	copy_array2_to_array1((*node).map_data, map);
	(*node).next = closed_head;
	closed_head = node;
}

// postcondition : if valid, renew the child node, if not, leave the child node as it is
void expand_node(int child[][3], char direction, int child_num, int child_flg[], int map[][3])
{
	if (is_valid(map, direction)) {
		child_flg[child_num] = 1;
		copy_array2_to_array1(child, map);
		operate(child, direction);
		// end if one of the child node is in goal state
		if (is_completed(child)) return 1;
	}
}

void check_malloc(LINKEDLIST *node)
{
	if (node == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		exit(-1);
	}
}

void push_to_open(int map[][3])
{
	OPENSTACK *node;
	node = malloc(sizeof(OPENSTACK));
	check_malloc(node);
	copy_array2_to_array1((*node).map_data, map);
	(*node).next = open_head;
	open_head = node;
}

int pop_from_open(int map[][3])
{
	if (open_head == NULL) return 0;
	copy_array2_to_array1(map, (*open_head).map_data);
	OPENSTACK *tmp = open_head;
	if ((*open_head).next == NULL) open_head = NULL;
	else open_head = (*open_head).next;
	free(tmp);
	return 1;
}

void copy_array2_to_array1(int array1[][3], int array2[][3])
{
	int i, j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			array1[i][j] = array2[i][j];
}
