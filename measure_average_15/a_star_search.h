// precondition : receives a map, and 'h' indicating which heuristic function to use
// postcondition : return 1 if solution is found, if not, return 0
int a_star_search(int map[][N], int h)
{
	// initialize global variable 'cnt_state'
	cnt_state = 0;

	int cost, depth;

	depth = 0;
	cost = calc_cost(h, map, depth);
	insert_to_open(map, cost, depth);
	while (1) {

		depth = (*open_head_a).depth;
		cost = (*open_head_a).cost;
		// node at the head of open list has minimum cost
		int got_node = get_min_cost_open(map);
		if (!got_node) {
			release_closed_a();
			// closed_head_a, closed_tail_a set back to NULL
			return 0;
		}
		if (is_completed(map)) {
			release_open_a();
			// open_head_a, open_tail_a set back to NULL
			release_closed_a();
			// closed_head_a, closed_tail_a set back to NULL
			return 1;
		}

		int child[4][N][N];
		// 0: up, 1:down, 2:left, 3:right
		int child_flg[4];
		// flg=1: expanded, flg=0: failed to expand
		expand_children_a(child, child_flg, map);
		insert_to_closed_a(map, cost);
		cnt_state++;

		int i;
		for (i=0; i<4; i++) {
			if (child_flg[i] == 0) continue;
			cost = calc_cost(h, child[i], depth+1);
			if (!(is_in_closed_a(child[i]))) {
				// child is not in closed list
				int is_in_open = replace_open_node(child[i], cost, depth+1);
				if (is_in_open) continue;
				// child node is new to both open list and closed list
				insert_to_open(child[i], cost, depth+1);
			} else {
				// child is in closed list
				delete_closed_insert_open(child[i], cost, depth+1);
				// high-cost deleted in closed, low-cost inserted in open
			}
		}

	} // end of while-loop

}

// g() : cost from start to current state
// h() : cost from current state to goal
int calc_cost(int h, int map[][N], int depth)
{
	switch (h) {
		case 0: return g(depth) + h0();
		case 1: return g(depth) + h1(map);
		case 2: return g(depth) + h2(map);
	}
}

// equal to depth
int g(int depth) { return depth; }

// ignore cost from current state to goal
int h0() { return 0; }

// returns the number of tiles at incorrect places
int h1(int map[][N])
{
	int completed_map[][N] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
	int count = 0;
	int i, j;
	for (i=0; i<N; i++) {
		for (j=0; j<N; j++) {
			if (map[i][j] == 0) continue;
			if (map[i][j] != completed_map[i][j]) count++;
		}
	}
	return count;
}

// returns Manhattan distance
// that is, sum of distance from goal state for each tile
int h2(int map[][N])
{
	int manhattan = 0;
	int i, j;
	for (i=0; i<N; i++) {
		for (j=0; j<N; j++) {
			if (map[i][j] == 0) continue;
			manhattan += get_distance(map[i][j], i, j);
		}
	}
	return manhattan;
}

int get_distance(int tile_num, int i, int j)
{
	int goal_idx[][2] = {
	//	{i,j},
		{3,3}, // blank
		{0,0}, // 1
		{0,1}, // 2
		{0,2}, // 3
		{0,3}, // 4
		{1,0}, // 5
		{1,1}, // 6
		{1,2}, // 7
		{1,3}, // 8
		{2,0}, // 9
		{2,1}, // 10
		{2,2}, // 11
		{2,3}, // 12
		{3,0}, // 13
		{3,1}, // 14
		{3,2}, // 15
	};
	int vertical_distance = abs(i - goal_idx[tile_num][0]);
	int horizontal_destance = abs(j - goal_idx[tile_num][1]);
	return vertical_distance + horizontal_destance;
}

void expand_children_a(int child[][N][N], int child_flg[], int map[][N])
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
void expand_node_a_star(int child[][N], char direction, int num, int flg[], int map[][N])
{
	if (is_valid(map, direction)) {
		flg[num] = 1;
		copy_array2_to_array1_2dim(child, map);
		operate(child, direction);
	}
}

void delete_closed_insert_open(int child[][N], int cost, int depth)
{
	CLOSEDLIST_A *matching_point = get_match_in_closed(child);
	if (cost < (*matching_point).cost) {
		delete_from_closed(matching_point);
		insert_to_open(child, cost, depth);
	}
}

// minimal cost node is located at the head of open list
// postcondition : corresponding map_data is copied to map[][]
int get_min_cost_open(int map[][N])
{
	// identical to pop operation of stack
	if (open_head_a == NULL) return 0;
	copy_array2_to_array1_2dim(map, (*open_head_a).map_data);
	OPENLIST_A *tmp = open_head_a;
	if ((*open_head_a).next == NULL) open_head_a = NULL;
	else open_head_a = (*open_head_a).next;
	free(tmp);
	return 1;
}

// open list is manipulated (replace -> delete then insert)
// postcondition : return 1 if node matches another node in open list
int replace_open_node(int child[][N], int cost, int depth)
{
	OPENLIST_A *point = open_head_a;
	while (point != NULL) {
		if (is_equal_array(child, (*point).map_data)) {
			if (cost < (*point).cost) {
				delete_from_open(point);
				// inserted node will be sorted upon cost
				insert_to_open(child, cost, depth);
			}
			return 1;
		}
		point = (*point).next;
	}
	// matching node not found in open list
	return 0;
}

// insert a new node to open list, in sorted order
void insert_to_open(int map[][N], int cost, int depth)
{
	OPENLIST_A *node;
	node = (OPENLIST_A *)malloc(sizeof(OPENLIST_A));
	check_malloc_open_list(node);
	copy_array2_to_array1_2dim((*node).map_data, map);
	(*node).depth = depth;
	(*node).cost = cost;
	// when open list was empty
	if (open_head_a == NULL) {
		(*node).next = NULL;
		open_head_a = node;
		open_tail_a = node;
	} else {
		// insert in sorted order of cost (ascending)
		insertion_sort(node);
	}
}

// postcondition : open list is in ascending order
void insertion_sort(OPENLIST_A *node)
{
	if ((*node).cost <= (*open_head_a).cost) {
		insert_to_open_head(node);
		return;
	}
	if ((*node).cost > (*open_tail_a).cost) {
		insert_to_open_tail(node);
		return;
	}
	// when inserting between head and tail
	OPENLIST_A *point = open_head_a;
	// compares cost of *node and cost of node after *point
	while ((*point).next != NULL) {
		if ((*node).cost <= (*((*point).next)).cost) {
			(*node).next = (*point).next;
			(*point).next = node;
			break;
		}
		point = (*point).next;
	}

}

void insert_to_open_head(OPENLIST_A *node)
{
	(*node).next = open_head_a;
	open_head_a = node;
}

void insert_to_open_tail(OPENLIST_A *node)
{
	(*open_tail_a).next = node;
	open_tail_a = node;
	(*node).next = NULL;
}

// delete the node at (*target) in open list
// simple implementation, but not the most time efficient method (linear time)
void delete_from_open(OPENLIST_A *target)
{
	// delete node at head of open list
	if (target == open_head_a) {
		open_head_a = (*open_head_a).next;
		free(target);
		return;
	}
	// deleting node located after head
	OPENLIST_A *point = open_head_a;
	while ((*point).next != NULL) {
		if ((*point).next == target) {
			if (target == open_tail_a) {
				(*point).next = NULL;
				open_tail_a = point;
				free(target);
				return;
			}
			(*point).next = (*target).next;
			free(target);
			return;
		}
		point = (*point).next;
	}
}

void insert_to_closed_a(int map[][N], int cost)
{
	CLOSEDLIST_A *node;
	node = (CLOSEDLIST_A *)malloc(sizeof(CLOSEDLIST_A));
	check_malloc_closed_a(node);
	(*node).cost = cost;
	copy_array2_to_array1_2dim((*node).map_data, map);
	if (closed_head_a == NULL) closed_tail_a = node;
	(*node).next = closed_head_a;
	closed_head_a = node;
}

// precondition : *target is a pointer to a node in closed list, which to delete
// simple implementation, but not the most time efficient method (linear time)
void delete_from_closed(CLOSEDLIST_A *target)
{
	// when deleting node at head of closed list
	if (target == closed_head_a) {
		closed_head_a = (*closed_head_a).next;
		free(target);
		return;
	}
	// deleting node located after head
	CLOSEDLIST_A *point = closed_head_a;
	while ((*point).next != NULL) {
		if ((*point).next == target) {
			// when target is at the tail 
			if (target == closed_tail_a) {
				(*point).next = NULL;
				closed_tail_a = point;
				free(target);
				return;
			}
			(*point).next = (*target).next;
			free(target);
			return;
		}
		point = (*point).next;
	}
}

// postcondition : return 1 if map is in closed list, if not, return 0
int is_in_closed_a(int map[][N])
{
	CLOSEDLIST_A *point;
	point = closed_head_a;
	while (point != NULL) {
		if (is_equal_array((*point).map_data, map)) return 1;
		point = (*point).next;
	}
	return 0;
}

CLOSEDLIST_A* get_match_in_closed(int child[][N])
{
	CLOSEDLIST_A *point = closed_head_a;
	while (point != NULL) {
		if (is_equal_array(child, (*point).map_data)) break;
		point = (*point).next;
	}
	return point;
}

void check_malloc_open_list(OPENLIST_A *node)
{
	if (node == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		exit(-1);
	}
}

void check_malloc_closed_a(CLOSEDLIST_A *node)
{
	if (node == NULL) {
		fprintf(stderr, "failed to allocate memory\n");
		exit(-1);
	}
}

void release_open_a()
{
	OPENLIST_A *tmp;
	while (open_head_a != NULL) {
		tmp = open_head_a;
		open_head_a = (*open_head_a).next;
		free(tmp);
	}
	open_head_a = NULL;
}

void release_closed_a()
{
	CLOSEDLIST_A *tmp;
	while (closed_head_a != NULL) {
		tmp = closed_head_a;
		closed_head_a = (*closed_head_a).next;
		free(tmp);
	}
	closed_tail_a = NULL;
}
