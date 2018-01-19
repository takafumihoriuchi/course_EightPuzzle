#define N 4  // map row and col of fifteen puzzle

// count total number of states searched
unsigned long int cnt_state;


// common

void init_map(int map[][N]);
void print_map(int map[][N]);
int is_completed(int map[][N]);
int get_zero_row(int map[][N]);
int get_zero_col(int map[][N]);
void operate(int map[][N], char direction);
int is_valid(int map[][N], char direction);


// iterative deepening search

typedef struct node_open {
	struct node_open *next;
	int map_data[N][N];
	int depth;
} OPENSTACK;

typedef struct node_closed {
	struct node_closed *next;
	int map_data[N][N];
} CLOSEDLIST;

OPENSTACK *open_head = NULL;
CLOSEDLIST *closed_head = NULL;
CLOSEDLIST *closed_tail = NULL;

int iterative_deepening_search(int map[][N]);
int expand_children(int child[][N][N], int child_flg[], int map[][N]);
int expand_node(int child[][N], char direction, int child_num, int child_flg[], int map[][N]);
void push_to_open(int map[][N], int depth);
int pop_from_open(int map[][N]);
int get_open_head_depth();
void insert_to_closed(int map[][N]);
int is_in_closed(int child[][N]);
void copy_array2_to_array1_1dim(int array1[], int array2[], int elem_num);
void copy_array2_to_array1_2dim(int array1[][N], int array2[][N]);
int is_equal_array(int array1[][N], int array2[][N]);
void check_malloc_open_stack(OPENSTACK *node);
void check_malloc_closed(CLOSEDLIST *node);
void release_open_stack();
void release_closed_list();
void print_open_stack();  // funtion for debugging


// A* search

typedef struct open_list {
	struct open_list *next;
	int map_data[N][N];
	int depth;
	int cost;
} OPENLIST_A;

typedef struct closed_list {
	struct closed_list *next;
	int map_data[N][N];
	int cost;
} CLOSEDLIST_A;

OPENLIST_A *open_head_a = NULL;
OPENLIST_A *open_tail_a = NULL;
CLOSEDLIST_A *closed_head_a = NULL;
CLOSEDLIST_A *closed_tail_a = NULL;

int a_star_search(int map[][N], int h);
int calc_cost(int h, int map[][N], int depth);
int g(int depth);
int h0();
int h1(int map[][N]);
int h2(int map[][N]);
int get_distance(int tile_num, int i, int j);
void expand_children_a(int child[][N][N], int child_flg[], int map[][N]);
void expand_node_a_star(int child[][N], char direction, int num, int flg[], int map[][N]);
void delete_closed_insert_open(int child[][N], int cost, int depth);
int get_min_cost_open(int map[][N]);
int replace_open_node(int child[][N], int cost, int depth);
void insert_to_open(int map[][N], int cost, int depth);
void insertion_sort(OPENLIST_A *node);
void insert_to_open_head(OPENLIST_A *node);
void insert_to_open_tail(OPENLIST_A *node);
void delete_from_open(OPENLIST_A *target);
void insert_to_closed_a(int map[][N], int cost);
void delete_from_closed(CLOSEDLIST_A *target);
int is_in_closed_a(int map[][N]);
CLOSEDLIST_A* get_match_in_closed(int child[][N]);
void check_malloc_open_list(OPENLIST_A *node);
void check_malloc_closed_a(CLOSEDLIST_A *node);
void release_open_a();
void release_closed_a();


// player
char player_input(int map[][N]);
