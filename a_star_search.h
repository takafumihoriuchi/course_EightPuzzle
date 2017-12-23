#define GOALSTATE {{1,2,3},{8,0,4},{7,6,5}}

// REVISE : name conflicting with global varialble in IDS
OPENLIST *open_head = NULL;
OPENLIST *open_tail = NULL;
CLOSEDLIST *closed_head = NULL;
CLOSEDLIST *closed_tail = NULL;

// precondition : receives a map (randomly initialized)
// postcondition : return 1 if solution is found, if not, return 0
int a_star_search(int map[][3], int h)
{
	// needed depending on the method of calculating g()
	// int initial_map[3][3];
	// copy_array2_to_array1_2dim(initial_map, map);

	int cost;
	int depth = 0;

	cost = calc_cost(h, map, depth);
	insert_to_open(map, cost, depth+1);

	while (1) {

		int has_node = get_min_cost_open(map);
		if (!has_node) return 0;
		if (is_completed(map)) return 1;

		int child[4][3][3];
		// 0: up, 1:down, 2:left, 3:right
		int child_flg[4];
		// flg=1: expanded, flg=0: failed to expand
		expand_children_a_star(child, child_flg, map);
		insert_to_closed(map);

		

	}

}

void expand_children_a_star(int child[][3][3], int child_flg[], int map[3])
{
	// reset child_flg
	int child_clear[4] = {0,0,0,0};
	copy_array2_to_array1_1dim(child_flg, child_clear, 4);

	expand_node_a_star(child[0], 'e', 0, child_flg, map);  // up
	expand_node_a_star(child[1], 'd', 1, child_flg, map);  // down
	expand_node_a_star(child[2], 's', 2, child_flg, map);  // left
	expand_node_a_star(child[3], 'f', 3, child_flg, map);  // right
}

// postcondition : child_flg is set to 1 for those expanded
void expand_node_a_star(int child[][3], char direction, int num, int flg[], int map[][3])
{
	if (is_valid(map, direction)) {
		flg[num] = 1;
		copy_array2_to_array1_2dim(child, map);
		operate(child, direction);
	}
}

// minimal cost node is located at the head of open list
int get_min_cost_open()
{
	// similar to pop_from_open(), uses different global variable
	if (open_head == NULL) return 0;
	copy_array2_to_array1_2dim(map, (*open_head).map_data);
	OPENLIST *tmp = open_head;
	if ((*open_head).next == NULL) open_head = NULL;
	else open_head = (*open_head).next;
	free(tmp);
	return 1;
}

// insert a new node to open list, in sorted order
void insert_to_open(int map[][3], int cost, int depth)
{
	OPENLIST *node;
	node = (OPENLIST *)malloc(sizeof(OPENLIST));
	check_malloc_open_list(node);
	copy_array2_to_array1_2dim((*node).map_data, map);
	(*node).depth = depth;
	(*node).cost = cost;
	// when open list was empty
	if (open_head == NULL) {
		(*node).next = NULL;
		open_head = node;
		open_tail = node;
	} else {
		insertion_sort(node);
	}
}

// postcondition : open list is in ascending order
void insertion_sort(OPENLIST *node)
{
	if ((*node).cost <= (*open_head).cost) {
		insert_to_open_head(node);
		return;
	}
	if ((*node).cost > (*open_tail).cost) {
		insert_to_open_tail(node);
		return;
	}
	// when inserting between head and tail
	OPENLIST *point = open_head;
	while ((*point).next != NULL) {
		if ((*node).cost <= (*((*point).next)).cost) {
			(*node).next = (*point).next;
			(*point).next = node;
		}
		point = (*point).next;
	}
}

void insert_to_open_head(OPENLIST *node)
{
	(*node).next = open_head;
	open_head = node;
}

void insert_to_open_tail(OPENLIST *node)
{
	(*open_tail).next = node;
	open_tail = node;
	(*node).next = NULL;
}

void check_malloc_open_list(OPENLIST *node)
{
	if (node == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		exit(-1);
	}
}

// g() : cost from start to current state
// h() : cost from current state to goal
int calc_cost(int h, int map[][3], int depth)
{
	switch (h) {
		case 0: return g(depth) + h0();
		case 1: return g(depth) + h1(map);
		case 2: return g(depth) + h2(map);
	}
}

// cost from start to current state (equal to depth)
int g(int depth)
{
	return depth;
}

int h0()
{
	return 0;
}

int h1(int map[][3])
{
	int completed_map[][3] = GOALSTATE;

}

int h2(int map[][3])
{
	int completed_map[][3] = GOALSTATE;

}