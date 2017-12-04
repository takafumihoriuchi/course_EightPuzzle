// #include "eight_puzzle.h" // needed?

int iterative_deepening_search(int map[][3]);

typedef struct node_open {
	struct node_open *next;
	int map_data[][3];
	int depth;
} OPENSTACK;

typedef struct node_closed {
	struct node_closed *next;
	int map_data[][3];
} CLOSEDLIST;

OPENSTACK *open_head = NULL;
CLOSEDLIST *closed_head = NULL;
CLOSEDLIST *closed_tail = NULL;

// precondition : receives a map (randomly initialized)
// postcondition : return 1 if solution is found, if not, return 0
int iterative_deepening_search(int map[][3])
{
	int limit = 0;
	int i, pop_success, node_depth;
	int child[4][3][3];
	int child_flg[4];  // 0: up, 1:down, 2:left, 3:right
	int child_clear[4] = {0,0,0,0};

	// end if the initialized map is already completed
	if (is_completed(map)) return 1;
	// push first state to open stack
	push_to_open(map, 0);

	while (1) {

		node_depth = get_open_head_depth();
		// pop top of stack and store in map
		pop_success = pop_from_open(map);
		if (!pop_success) {
			// initial map data is stored at the tail of closed list
			copy_array2_to_array1(map, (*closed_tail).map_data);
			release_closed_list();
			push_to_open(map, 0);
			limit++;
			continue;  // add commands here to set limit to 'limit' / if (limit > ~) return 0; continue;
		}

		// depth limited search
		if (node_depth >= limit) continue;

		copy_array2_to_array1(child_flg, child_clear);
		// expand node, then check if child node is in goal state; return 1 if so
		if (expand_node(child[0], 'e', 0, child_flg, map)) return 1;  // up
		if (expand_node(child[1], 'd', 1, child_flg, map)) return 1;  // down
		if (expand_node(child[2], 's', 2, child_flg, map)) return 1;  // left
		if (expand_node(child[3], 'f', 3, child_flg, map)) return 1;  // right
		insert_to_closed(map);

		for (i=0; i<4; i++) {
			if (!child_flg[i]) continue;  // guard clause  // add 1 to parent depth
			if (!(is_in_closed(child[i]))) push_to_open(child[i], node_depth+1);
		}

	}

}

void release_closed_list()
{
	CLOSEDLIST *tmp;
	while (closed_head != NULL) {
		tmp = closed_head;
		closed_head = (*closed_head).next;
		free(tmp);
	}
	closed_tail = NULL;  // not necessary, but more feels clean to do so
}

int get_open_head_depth()
{
	return (*open_head).depth;
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
	check_malloc_closed(node);
	copy_array2_to_array1((*node).map_data, map);
	if (closed_head == NULL) closed_tail = node;
	(*node).next = closed_head;
	closed_head = node;
}

// postcondition-1 : if valid, renew the child node, if not, leave the child node as it is
// postcondition-2 : return 1 if child node is in goal state, return 0 if not
int expand_node(int child[][3], char direction, int child_num, int child_flg[], int map[][3])
{
	if (is_valid(map, direction)) {
		child_flg[child_num] = 1;
		copy_array2_to_array1(child, map);
		operate(child, direction);
		// end if one of the child node is in goal state
		if (is_completed(child)) return 1;
	}
	return 0;
}

void check_malloc_open(OPENSTACK *node)
{
	if (node == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		exit(-1);
	}
}

void check_malloc_closed(CLOSEDLIST *node)
{
	if (node == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		exit(-1);
	}
}

void push_to_open(int map[][3], int depth)
{
	OPENSTACK *node;
	node = malloc(sizeof(OPENSTACK));
	check_malloc_open(node);
	copy_array2_to_array1((*node).map_data, map);
	(*node).depth = depth;
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
