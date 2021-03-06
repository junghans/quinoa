#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H

extern int maxObj;

//#include "converse.h"

//#define CpvAccess(x) (x)
#define _MEMCHECK(x) (x)
#define CmiMyPe() (mype)
#define CrnRand() rand()
#define CmiPrintf printf
#define CldLoad() (myqlength(queue))

#define CmiNumPes() (maxObj)
#define CkNumPes() (maxObj)

typedef struct {int node1, node2;} Edge;

typedef struct {int next; 
	 Edge * edges;} EdgeListType;

typedef struct {
  int degree; 
  int next; /* temporary count needed to tell where to insert the next entry */
  int adjListInd; /* where in the big array does its adj list begin */
  /*  int available;*/ /* number of connections still available. 12/2/97 */
} Vertex;

typedef struct {
  int numVertices;
  Vertex * vertexArray; /* ptr to an array of records, one for each vertex */
  int * adjArray; /* ptr to an array in which adjacency sub-arrays for each 
		     vertex are stored contiguosly */
} VerticesListType;

#define connections(i) (graph.vertexArray[i].degree)

/* -----*/
typedef struct {
  int size, head, tail;
  int numElements;
  int * buf; }
 Q;

#endif
