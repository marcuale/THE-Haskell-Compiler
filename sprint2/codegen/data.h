#include <stdlib.h>
#include <stddef.h>

struct THCObject;
typedef enum ObjType{
    ATOMIC, // only integer atm
    FUNCTION,
    ALGEBRAIC,
    NODE, 
} ObjType;

typedef enum GCFlag{
    STACK,
    HEAP,
    GLOBAL,
    COPIED,
} GCFlag;

// linked list implementation
// used for storing function arguments
// and fields in an object
typedef struct THCNode {
    struct THCObject * value;
    struct THCObject * next; 
} THCNode; 

typedef struct THCList{
    struct THCObject * head;
    size_t size;
} THCList;

typedef struct THCFunc {
    void (*f)(struct THCObject * cont, struct THCList * args);
    int arity;
    THCList args;
    struct THCObject * cont; // continuation
    int shouldeval;
} THCFunc;

typedef union THCUnion {
    int i;
    THCList l;
    THCFunc f;
    THCNode n;
} THCUnion;

typedef struct THCObject {
    THCUnion contents;
    int constructor;
    ObjType type;
    GCFlag gcflag;
    size_t gc_forward; 
} THCObject;

void initializeList(THCObject * cont, THCObject * /*NULL*/);
void prependListAtomic(THCObject * cont, THCObject * /* arity 2 */);

THCObject * constructFunction(void (*f)(THCObject *, THCList *), int arity);

void callCont(THCObject * cont, THCObject * arg);
void callFunction(THCObject * func);

extern THCObject * THCprintResult;
extern THCObject * Default_add_obj;
extern THCObject * Default_mult_obj;
void initDefaults();
void constructInt(THCObject * obj, int i);
void constructNode(THCObject * obj);
void constructAlgebraic(THCObject * obj, int);
void constructContinuation(THCObject *, THCObject *, THCObject *);
int is_evaluated(THCObject *);

void evaluate(THCObject * current, THCObject * obj);
void setArgs(THCObject *, THCList * args);

THCObject * indexList(THCList * l, size_t ind);

void THC_driver_loop(THCObject *);
void THC_minor_gc(THCObject* curr);

int checkStackSize();
void initHeap();
void movePtr(THCObject*, ptrdiff_t, THCObject**);
