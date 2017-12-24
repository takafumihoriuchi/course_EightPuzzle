// count total number of states searched
unsigned long int cnt_state;


// common

void init_map(int map[][3]);
void print_map(int map[][3]);
int is_completed(int map[][3]);
int get_zero_row(int map[][3]);
int get_zero_col(int map[][3]);
void operate(int map[][3], char direction);
int is_valid(int map[][3], char direction);


// iterative deepening search

typedef struct node_open {
	struct node_open *next;
	int map_data[3][3];
	int depth;
} OPENSTACK;

typedef struct node_closed {
	struct node_closed *next;
	int map_data[3][3];
} CLOSEDLIST;

OPENSTACK *open_head = NULL;
CLOSEDLIST *closed_head = NULL;
CLOSEDLIST *closed_tail = NULL;

int iterative_deepening_search(int map[][3]);
int expand_children(int child[][3][3], int child_flg[], int map[][3]);
int expand_node(int child[][3], char direction, int child_num, int child_flg[], int map[][3]);
void push_to_open(int map[][3], int depth);
int pop_from_open(int map[][3]);
int get_open_head_depth();
void insert_to_closed(int map[][3]);
int is_in_closed(int child[][3]);
void copy_array2_to_array1_1dim(int array1[], int array2[], int elem_num);
void copy_array2_to_array1_2dim(int array1[][3], int array2[][3]);
int is_equal_array(int array1[][3], int array2[][3]);
void check_malloc_open_stack(OPENSTACK *node);
void check_malloc_closed(CLOSEDLIST *node);
void release_open_stack();
void release_closed_list();
void print_open_stack();  // funtion for debugging


// A* search

typedef struct open_list {
	struct open_list *next;
	int map_data[3][3];
	int depth;
	int cost;
} OPENLIST_A;

typedef struct closed_list {
	struct closed_list *next;
	int map_data[3][3];
	int cost;
} CLOSEDLIST_A;

OPENLIST_A *open_head_a = NULL;
OPENLIST_A *open_tail_a = NULL;
CLOSEDLIST_A *closed_head_a = NULL;
CLOSEDLIST_A *closed_tail_a = NULL;

int a_star_search(int map[][3], int h);
int calc_cost(int h, int map[][3], int depth);
int g(int depth);
int h0();
int h1(int map[][3]);
int h2(int map[][3]);
int get_distance(int tile_num, int i, int j);
void expand_children_a(int child[][3][3], int child_flg[], int map[][3]);
void expand_node_a_star(int child[][3], char direction, int num, int flg[], int map[][3]);
void delete_closed_insert_open(int child[][3], int cost, int depth);
int get_min_cost_open(int map[][3]);
int replace_open_node(int child[][3], int cost, int depth);
void insert_to_open(int map[][3], int cost, int depth);
void insertion_sort(OPENLIST_A *node);
void insert_to_open_head(OPENLIST_A *node);
void insert_to_open_tail(OPENLIST_A *node);
void delete_from_open(OPENLIST_A *target);
void insert_to_closed_a(int map[][3], int cost);
void delete_from_closed(CLOSEDLIST_A *target);
int is_in_closed_a(int map[][3]);
CLOSEDLIST_A* get_match_in_closed(int child[][3]);
void check_malloc_open_list(OPENLIST_A *node);
void check_malloc_closed_a(CLOSEDLIST_A *node);
void release_open_a();
void release_closed_a();


// player
char player_input(int map[][3]);
