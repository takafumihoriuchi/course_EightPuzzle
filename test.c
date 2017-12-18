/******************************/
/*                            */
/*   a game of eight puzzle   */
/*   - by Takafumi Horiuchi   */
/*                            */
/******************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node_open {
   struct node_open *next;
   int depth;
   int map_data[][3];
} OPENSTACK;

typedef struct node_closed {
   struct node_closed *next;
   int map_data[][3];
} CLOSEDLIST;

OPENSTACK *open_head = NULL;
CLOSEDLIST *closed_head = NULL;
CLOSEDLIST *closed_tail = NULL;

void init_map(int map[][3]);
void print_map(int map[][3]);
int is_completed(int map[][3]);
int get_zero_row(int map[][3]);
int get_zero_col(int map[][3]);
void operate(int map[][3], char direction);
int is_valid(int map[][3], char direction);
void player_input(int map[][3]);
int iterative_deepening_search(int map[][3]);
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

// precondition : receives a map (randomly initialized)
// postcondition : return 1 if solution is found(, if not, return 0)
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
         // map is initial state when limit=0
         if (closed_tail != NULL)
            copy_array2_to_array1_2dim(map, (*closed_tail).map_data);
            // initial map data is stored at the tail of closed list
         release_closed_list();
         push_to_open(map, 0);
         limit++;
         continue;  // add commands here to set limit to 'limit' / if (limit > ~) return 0; continue;
      }

      // depth limited search
      if (node_depth >= limit) continue;

      copy_array2_to_array1_1dim(child_flg, child_clear, 4);
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

// postcondition-1 : if valid, renew the child node, if not, leave the child node as it is
// postcondition-2 : return 1 if child node is in goal state, return 0 if not
int expand_node(int child[][3], char direction, int child_num, int child_flg[], int map[][3])
{
   if (is_valid(map, direction)) {
      child_flg[child_num] = 1;
      copy_array2_to_array1_2dim(child, map);
      operate(child, direction);
      // end if one of the child node is in goal state
      if (is_completed(child)) return 1;
   }
   return 0;
}

void push_to_open(int map[][3], int depth)
{
   OPENSTACK *node;
   node = malloc(sizeof(OPENSTACK));   // BUG HERE!!
   // printf("node : %d\n", node);
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
   node = malloc(sizeof(CLOSEDLIST));
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
   while (1) {
      if (is_equal_array((*point).map_data, child))
         return 1;
      if ((*point).next==NULL)
         return 0;
      point = (*point).next;
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