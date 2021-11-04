#include <stdlib.h>

struct THCObject;

// linked list implementation
// used for storing function arguments
// and fields in an object
typedef struct THCNode {
    struct THCObject * value;
    struct THCNode * next; 
} THCNode; 

typedef struct THCList{
    struct THCNode * head;
    size_t size;
} THCList;

typedef struct THCFunc {
    void (*f)(struct THCObject * cont, struct THCList * args);
    int arity;
    THCList args;
    struct THCObject * cont; // continuation
} THCFunc;

typedef union THCUnion {
    int i;
    THCList l;
    THCFunc f;
} THCUnion;

typedef struct THCObject {
    void (*evaluate)(struct THCObject *);
    THCUnion contents;
    // ignore these two for now
    int is_evaluated;
    struct THCObject * evaluated;

    int constructor;
} THCObject;

void initializeList(THCObject * cont, THCObject * /*NULL*/);
void prependListAtomic(THCObject * cont, THCObject * /* arity 2 */);

THCObject * constructFunction(void (*f)(THCObject *, THCList *), int arity);

void callCont(THCObject * cont, THCObject * arg);
void callFunction(THCObject * func);
void applyFunctionRaw(THCObject * cont, THCObject * f, THCObject * arg);

extern THCObject * THCprintResult;
extern THCObject * THCFuncApp;
extern THCObject * THCadd;
void initDefaults();
void nop(THCObject *);
void constructInt(THCObject * obj, int i);
void constructContinuation(THCObject *, THCObject *, THCObject *);
