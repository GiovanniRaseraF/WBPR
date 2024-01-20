#include<stdio.h>
#include<math.h>

/* A BUG IN PREVIOUS VERSION HAS BEEN FIXED */ 

/*-----------------------------------------------------------------*/
/*  Generates random undirected graphs with $m$ edges, in          */ 
/*  Challenge format.  Instances have random uniform edge          */
/*  costs.                                                         */
/*  C. McGeoch at DIMACS, July 1991  */

/*  You may need to insert your own random number generators.     */
/*     double drand48();  returns doubles from (0.0, 1.0]         */
/*     void srand48(seed);  initializes RNG with seed             */
/*     long seed;                                                 */

/*  Example input:         */
/*  nodes 1000             */
/*  edges  2500            */
/*  maxcost 1000           */ 
/*  seed  818182717        */

/*  Seed command is optional.                           */
/*  nodes  N  : specifies N nodes                       */ 
/*  edges  A  : specifies A edges                       */
/*  maxcost X : specifies maximum edge weight           */ 
/*  seed   S  : RNG seed; default system timer          */ 
/*------------------------------------------------------*/  

#define Assert( cond , msg ) if ( ! (cond) ) {printf("msg\n"); exit(); } ; 
#define MAXNODES 10000
#define MAXCOST 100000

#define TRUE 1
#define FALSE 0

typedef char string[50];

/* BST for Set searches */ 
typedef struct node_type {
  int val;
  struct node_type *left;
  struct node_type *right;
} treenode; 

int arc_size; 
treenode *root; 

/* Random Number Functions */ 
double drand48();
void srand48(); 

/* Global Parameters */
long seed;
int nodes,edges; 
int rand_seed;        /*boolean*/ 
int seed; 
int maxcost; 
int maxarcs, maxedges; 

string cmdtable[10];
int cmdtable_size; 
int nodelist[MAXNODES]; 
int listsize; 

/*----------------- Initialize tables and data  */ 
void init()
{ 
  int i; 
  
  cmdtable_size = 4; 
  strcpy(cmdtable[0], "sentinel.spot");
  strcpy(cmdtable[1], "nodes" );   /* required */ 
  strcpy(cmdtable[2], "edges"  );
  strcpy(cmdtable[3], "seed");
  strcpy(cmdtable[4], "maxcost");
  
  nodes = 100; 
  edges = 100; 
  maxcost = MAXCOST; 
  rand_seed = TRUE;

  root = NULL; 
  arc_size = 0; 

}   

/* ---------------- Random number utilities */

/* Initialize rng */  
void init_rand(seed) 
int seed; 
{
  srand48(seed);
    }

/* Return an integer from [1..max] */  
int rand_int(max) 
int max;
{
  double x; 
  int i; 
  
  x = drand48();
  i = (double) x * max + 1.0;
  return(i);
}

double rand_d()
{
   double x;
   x = drand48();

   return(x);
 }


/*-------------------------bst for selection wo replacement */ 

int inset(arc, ptr)
int arc;
{
  treenode *current;

  current = root; 
  while(current != NULL) {
    if (current->val > arc) { 
      current = current->left;
    }
    else if (current->val < arc) {
      current = current->right;
    }
    else 
      return(TRUE);
  }/*while*/

  return(FALSE);

}/*inset*/

/* insert arc into tree--assumes its not already in */ 

void insert(arc)
int arc;
{
  treenode *current;
  treenode *parent;

  if ( arc_size ==  0) {  /* first insertion */
    root = (treenode *) malloc(1*sizeof(treenode));
    root->val = arc;
    root->left = NULL;
    root->right= NULL; 
  }else{
    current = root;

    while (current != NULL) {
      parent = current; 
      /* search for the empty spot, saving parent */
      if (arc < parent->val) { 
	current = parent->left;
	}
      else if (arc > parent->val) {
	current = parent->right;
      }

    }/* while */ 
  
    current = (treenode *) malloc(1*sizeof(treenode));
    current->val = arc;
    current->left = NULL;
    current->right = NULL; 

    if (arc < (parent->val)) parent->left = current;
    else parent->right = current;
  }/*else not new  */

    arc_size++; 
} /* insert */

/*  generate edges in fairly sparse graphs */ 
void gen_edges1() 
{
   int i; 
   int out; 
   int src,dst; 
   int a; 
   int cost; 

  for (i=0; i< edges; i++) { 

    /* loop until an unused edge is found */ 
    /* and dont allow the upper diagonal */ 
    do {   
       a = rand_int(maxarcs);
       src = ((a-1) / (nodes-1)) + 1;        /* row number in 1..n */
       dst = (a % (nodes-1)) + 1;          /* col number in 1..n-1 */
       if (src == dst) {                /* move arc [i,i] to [i,n] */ 
	dst = nodes; 
      }
/* printf("a %d  src  %d  dst  %d \n", a, src,dst);  */

    } while ((src > dst) || (inset(a) == TRUE ));
 
    /* found one */ 
    insert(a); 
    cost  = rand_int(maxcost); 
    printf("e \t %d \t %d \t %d\n", src, dst, cost); 
  
  }/* for i */ 

 }/*gen_edges1 */ 


/*   generate edges in fairly dense graphs  */ 
/*  See Knuth V2, 3.4.2, Algorithm S        */

void gen_edges2() 
{
    int howmany; 
    double have; 
    double seen; 
    double need; 
    double total; 
    double x; 
    int src, dst; 
    int cost; 
  
    need = (double) edges;
    total = (double) maxedges;
    have = 0.0;    /* increment each time one is printed */
    seen = 0.0;    /* increment each time one is examined */ 
 
    /* consider each edge in turn and decide whether to use it */ 

    for (src = 1; src <= nodes; src++) {
      for (dst = 1; dst < src; dst++) {

           if (have >= need) break; 
 
           x = rand_d();
           /* choose current edge with prob =  */
           /* (number left to choose)/(number left in set) */ 

           if ((total - seen) *x < (need - have) ) {

	     cost = rand_int(maxcost);
	     printf("e  %d  %d  %d \n", src, dst, cost);
             have++;
 
	   }/* if */
           seen++;

	 }/*for dst*/
    }/*for src*/
  }/*gen_edges2*/

/*------------------Command input routines  */ 
int lookup(cmd)
{
 int i;
 int stop;

 strcpy( cmdtable[0], cmd);  /* sentinel */ 
 stop = 1;
 for (i = cmdtable_size; stop != 0; i--) stop = strcmp(cmdtable[i], cmd);

 return ( i + 1 ); 
}/*lookup*/

/*---------------------- Read command lines */ 
void get_input()
{
char cmd[50], buf[50];
int index;
int i; 

  while (scanf("%s", cmd ) != EOF) {
    fgets(buf, sizeof(buf), stdin);
    index = lookup(cmd);
    switch(index) {

    case 0:  { fprintf(stderr, "%s: Unknown command. Ignored.\n", cmd);
	       break;
	     }
    case 1:  {sscanf( buf , "%d", &nodes); 
	      Assert( 0<=nodes && nodes<=MAXNODES , Nodes out of range. );
	      maxarcs =  ((nodes*nodes) - nodes); 
              maxedges = maxarcs/2; 
	      break;
	    }
    case  2: {sscanf (buf , "%d", &edges);
	      break;
	    }
    case 3: { sscanf( buf, "%d", &seed);
	       rand_seed  = FALSE;
	       break;
	    }
    case 4: { sscanf( buf, "%d", &maxcost);
              Assert( maxcost <= MAXCOST , Maxcost too high--recompile. ); 
              break;
            }
    }/*switch*/
  }/*while*/

  /* Do sanity checks */ 
  Assert( 0 <= edges && edges <= maxedges , Bad number of edges value. ); 

}/*input*/
/* ---------------------------Report parameters  */

void report_params()
{
  printf("c nodes %d\n", nodes);
  printf("c edges %d \n", edges);
  printf("c maxcost %d \n", maxcost); 
  if (rand_seed == TRUE) printf("c random seed\n");
  else printf("c seed %d\n", seed);
}

/*--------------------------- Generate and print out network  */ 
void generate_graph()
{
 int n, a, i;
 int cap; 
 int src, dst; 
 int t, s; 

  if (rand_seed == TRUE) init_rand((int) time(0));
  else init_rand(seed); 

  /* Print first line and some comments  */

  printf("p edge  \t %d \t %d \n", nodes, edges);
  printf("c Matching problem with %d nodes and %d edges\n", 
          nodes, edges);
  printf("c Randomly generated---very little structure \n");
  printf("c Max edge weight %d\n", maxcost);

  report_params();

  /*  use different generation routines depending on density */

  if (edges < maxarcs / 8) gen_edges1(); 
  else gen_edges2();  
 

}/*generate_graph*/

main()
{

    init(); 

    get_input();

    generate_graph(); 

  } 