// common
void init_map(int map[][3]);
void print_map(int map[][3]);
int is_completed(int map[][3]);
int get_zero_row(int map[][3]);
int get_zero_col(int map[][3]);
void operate(int map[][3], char direction);
int is_valid(int map[][3], char direction);


// player
void player_input(int map[][3]);


// iterative deepening search
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


