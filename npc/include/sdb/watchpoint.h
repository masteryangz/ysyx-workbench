#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char *expr; // Expression to watch
  word_t value; // Value of the expression at the time of creation
} WP;

extern WP *wp_head;
extern WP *free_;

void init_wp_pool();
WP* new_wp(char *expr, word_t value);
void free_wp(WP *wp);
void set_value(WP *wp, word_t value);