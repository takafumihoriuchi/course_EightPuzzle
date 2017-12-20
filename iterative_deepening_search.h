typedef struct node_open {
	struct node_open *next;
	int depth;
	int map_data[3][3];
} OPENSTACK;

typedef struct node_closed {
	struct node_closed *next;
	int map_data[3][3];
} CLOSEDLIST;

int iterative_deepening_search(int map[][3]);
int expand_children(int child[][3][3], int child_flg[], int map[][3]);
int expand_node(int child[][3], char direction, int child_num, int child_flg[], int map[][3]);
void push_to_open(int map[][3], int depth);
int pop_from_open(int map[][3]);
int get_open_head_depth();
void insert_to_closed(int map[][3]);
int is_in_closed(int child[][3]);
void release_closed_list();
void copy_array2_to_array1_1dim(int array1[], int array2[], int elem_num);
void copy_array2_to_array1_2dim(int array1[][3], int array2[][3]);
int is_equal_array(int array1[][3], int array2[][3]);
void check_malloc_open(OPENSTACK *node);
void check_malloc_closed(CLOSEDLIST *node);
void print_open_stack();

OPENSTACK *open_head = NULL;
CLOSEDLIST *closed_head = NULL;
CLOSEDLIST *closed_tail = NULL;

// precondition : receives a map (randomly initialized)
// postcondition : return 1 if solution is found(, if not, return 0)
int iterative_deepening_search(int map[][3])
{
	int limit = 0;
	int i, pop_success, node_depth, match_goal_state;
	// 0: up, 1:down, 2:left, 3:right
	int child[4][3][3];
	int child_flg[4];

	// end if the initialized map is already completed
	if (is_completed(map)) return 1;
	// push first state to open stack
	push_to_open(map, 0);

	while (1) {

		node_depth = get_open_head_depth();

		// pop top of stack and store in map
		pop_success = pop_from_open(map);
		if (!pop_success) {
			// map is initial state when limit=0
			if (closed_tail != NULL)
				copy_array2_to_array1_2dim(map, (*closed_tail).map_data);
				// initial map data is stored at the tail of closed list
			release_closed_list();
			push_to_open(map, 0);
			limit++;
			printf("proceeding to limit : %d\n", limit);
			continue;  // add commands here to set limit to 'limit' / if (limit > ~) return 0; continue;
		}

		// key of depth limited search
		if (node_depth >= limit) continue;

		match_goal_state = expand_children(child, child_flg, map);
		if (match_goal_state) return 1;
		
		insert_to_closed(map);
		
		for (i=0; i<4; i++) {
			if (!child_flg[i]) continue;  // skip non-updated child
			if (!(is_in_closed(child[i])))
				push_to_open(child[i], node_depth+1);  // child_depth is parent_depth + 1
		}

	}

}

int expand_children(int child[][3][3], int child_flg[], int map[][3])
{
	// reset child_flg
	int child_clear[4] = {0,0,0,0};
	copy_array2_to_array1_1dim(child_flg, child_clear, 4);

	if (expand_node(child[0], 'e', 0, child_flg, map)) return 1;  // up
	if (expand_node(child[1], 'd', 1, child_flg, map)) return 1;  // down
	if (expand_node(child[2], 's', 2, child_flg, map)) return 1;  // left
	if (expand_node(child[3], 'f', 3, child_flg, map)) return 1;  // right
}

// postcondition-1 : if valid, renew the child node, if not, leave the child node as it is
// postcondition-2 : return 1 if child node is in goal state, return 0 if not
int expand_node(int child[][3], char direction, int child_num, int child_flg[], int map[][3])
{
	if (is_valid(map, direction)) {
		child_flg[child_num] = 1;
		copy_array2_to_array1_2dim(child, map);
		operate(child, direction);
		if (is_completed(child)) return 1;
	}
	return 0;
}

void push_to_open(int map[][3], int depth)
{
	OPENSTACK *node;
	node = (OPENSTACK *)malloc(sizeof(OPENSTACK));
	check_malloc_open(node);
	copy_array2_to_array1_2dim((*node).map_data, map);
	(*node).depth = depth;
	(*node).next = open_head;
	open_head = node;
}

int pop_from_open(int map[][3])
{
	if (open_head == NULL) return 0;
	copy_array2_to_array1_2dim(map, (*open_head).map_data);
	OPENSTACK *tmp = open_head;
	if ((*open_head).next == NULL) open_head = NULL;
	else open_head = (*open_head).next;
	free(tmp);
	return 1;
}

int get_open_head_depth()
{
	if (open_head == NULL) return 0;
	return (*open_head).depth;
}

void insert_to_closed(int map[][3])
{
	CLOSEDLIST *node;
	node = (CLOSEDLIST *)malloc(sizeof(CLOSEDLIST));
	check_malloc_closed(node);
	copy_array2_to_array1_2dim((*node).map_data, map);
	if (closed_head == NULL) closed_tail = node;
	(*node).next = closed_head;
	closed_head = node;
}

// postcondition : return 1 if given child is in closed list, if not, return 0
int is_in_closed(int child[][3])
{
	CLOSEDLIST *point;
	point = closed_head;
	while (point != NULL) {
		if (is_equal_array((*point).map_data, child)) return 1;
		point = (*point).next;
	}
	return 0;
}

void release_closed_list()
{
	CLOSEDLIST *tmp;
	while (closed_head != NULL) {
		tmp = closed_head;
		closed_head = (*closed_head).next;
		free(tmp);
	}
	closed_tail = NULL;
}

// restricted to one-dimension array
void copy_array2_to_array1_1dim(int array1[], int array2[], int elem_num)
{
	int i;
	for (i=0; i<elem_num; i++)
		array1[i] = array2[i];
}

// restricted to three-three-element-two-dimension array
void copy_array2_to_array1_2dim(int array1[][3], int array2[][3])
{
	int i, j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			array1[i][j] = array2[i][j];
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

// function used for debugging
void print_open_stack()
{
	OPENSTACK *point;
	point = open_head;
	int i=0;
	printf("open stack:\n");
	if (point==NULL) printf("no entry\n");
	while (point!=NULL) {
		printf("No.%d : %d\n", i++, point);
		point = (*point).next;
	}
}