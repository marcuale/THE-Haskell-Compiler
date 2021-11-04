#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "data.h"

// TODO: determine this in a better way
#define STACK_LIMIT (1<<19) // 512KiB
#define HEAP_INIT (1<<12) // 4KiB

THCObject * THCprintResult;
THCObject * Default_add_obj;
THCObject * Default_mult_obj;
THCObject * THCcallExcessArgs;
THCObject * THCevalAndContinue;

THCObject * constructFunction(void (*func)(THCObject *, THCList *), int arity){
    THCObject * obj = malloc(sizeof(THCObject));
    obj->type = FUNCTION;
    
    THCFunc * f =&(obj->contents.f);
    f->arity = arity;
    f->f = func;
    f->cont = NULL;
    f->args.head = NULL;
    f->args.size = 0;
    f->shouldeval = 0;

    obj->gcflag = GLOBAL;
    obj->gc_forward = 0;
    return obj;
}

inline int is_evaluated(THCObject * o){
    // if we are not a function, or only a partial function
    return o->type != FUNCTION || o->contents.f.arity > o->contents.f.args.size;
}

inline void constructInt(THCObject * obj, int i){
    obj->contents.i = i;
    obj->constructor = 0;
    obj->type = ATOMIC;
    obj->gcflag = STACK;
    obj->gc_forward = 0;
}

inline void constructAlgebraic(THCObject * obj, int constructor){
    obj->constructor = constructor;
    obj->type = ALGEBRAIC;
    obj->gcflag = STACK;
    obj->gc_forward = 0;
}

inline void constructNode(THCObject * obj){
    obj->constructor = 0;
    obj->type = NODE;
    obj->gcflag = STACK;
    obj->gc_forward = 0;
}

inline void constructContinuation(THCObject * dest, THCObject * src, THCObject * cont){
    *dest = *src;
    //memcpy(dest, src, sizeof(THCObject));
    dest->contents.f.cont = cont;
    dest->gcflag = STACK;
}

void callExcessArgs(THCObject * this, THCList * args){
    THCObject next_func;
    THCObject * cont = this->contents.f.cont;
    constructContinuation(&next_func, args->head->contents.n.value, cont);
    THCList excess_args = args->head->contents.n.next->contents.n.value->contents.l; 
    setArgs(&next_func, &excess_args);
    callFunction(&next_func); 
}

void callCont(THCObject * cont, THCObject * arg){
    THCObject new_cont;
    new_cont = *cont;

    THCObject new_node;
    constructNode(&new_node);
    new_node.contents.n.value = arg;
    new_node.contents.n.next = cont->contents.f.args.head;

    new_cont.contents.f.args.head = &new_node;
    new_cont.contents.f.args.size++;
    callFunction(&new_cont);
}

void evalAndContinue(THCObject * this, THCList * args){
    THCObject * cont = this->contents.f.cont;
    THCObject * new = args->head->contents.n.value;
    THCObject * orig = args->head->contents.n.next->contents.n.value;
    
    if(new->type == FUNCTION){
        new->contents.f.cont = NULL;
    }

    GCFlag gcflag = orig->gcflag;
    *orig = *new;
    orig->gcflag = gcflag;
    /*
    THCObject * n = cont->contents.f.args.head;
    while(n != NULL){
        if(n->contents.n.value == orig)
            n->contents.n.value = new;
        n = n->contents.n.next;
    }*/
    cont->contents.f.shouldeval = 1;
    callFunction(cont);
}

void evaluate(THCObject * current, THCObject * obj){
    if(is_evaluated(obj)) return;
    obj->contents.f.shouldeval = 1;
    
    THCObject new_curr = *current;
    new_curr.gcflag = STACK;

    THCObject evaluator;
    constructContinuation(&evaluator, THCevalAndContinue, &new_curr);

    THCObject orig_arg;
    constructNode(&orig_arg);
    orig_arg.contents.n.next = NULL;
    orig_arg.contents.n.value = obj;

    evaluator.contents.f.args.head = &orig_arg;
    evaluator.contents.f.args.size = 1;

    obj->contents.f.cont = &evaluator;
    callFunction(obj);
}

void callFunction(THCObject * func){
    THCFunc * f = &(func->contents.f);
    THCList * args = &(f->args);
    if(!f->shouldeval || is_evaluated(func)){
        THCObject * next_cont = f->cont;
        f->cont = NULL;
        callCont(next_cont, func);
    } else if(f->arity < args->size){
        // partial list: arguments to give to f
        // new_list: arguments to give to return of f
        THCList required_args;
        THCObject * n = args->head;
        for(int i=0; i<args->size - f->arity-1; i++){
            n = n->contents.n.next;
        }
        required_args.head = n->contents.n.next;
        required_args.size = f->arity; 
        n->contents.n.next = NULL;

        THCList remaining_args = *args;
        remaining_args.size = args->size - f->arity;

        f->args = required_args;
        THCObject excess_arg_cont;
        constructContinuation(&excess_arg_cont, THCcallExcessArgs, f->cont);
        
        THCObject excess_args;
        constructAlgebraic(&excess_args, 0);
        excess_args.contents.l = remaining_args;

        THCObject excess_arg_wrapper_elem;
        constructNode(&excess_arg_wrapper_elem);
        excess_arg_wrapper_elem.contents.n.next = NULL;
        excess_arg_wrapper_elem.contents.n.value = &excess_args;
        THCList excess_arg_wrapper;
        excess_arg_wrapper.head = &excess_arg_wrapper_elem;
        excess_arg_wrapper.size = 1;

        setArgs(&excess_arg_cont, &excess_arg_wrapper);

        THCObject funccopy;
        constructContinuation(&funccopy, func, &excess_arg_cont);

        //f->cont = &excess_arg_cont;
        f->f(&funccopy, &required_args);
    }else{
        THCList new_args = f->args;
        f->f(func, &new_args);
    }
}

THCObject * indexList(THCList * l, size_t ind){
    THCObject * n = l->head;
    for(int i=0; i<ind; i++){
        n = n->contents.n.next;
    }
    return n->contents.n.value;
}
void setArgs(THCObject * f, THCList * args){
    // Note: may have to copy args. ugh, nontrivial
    if(args->size == 0) return;
    f->contents.f.args.size += args->size;
    THCObject * n = args->head;
    while(n->contents.n.next != NULL){
        n = n->contents.n.next;
    }
    n->contents.n.next = f->contents.f.args.head;
    f->contents.f.args.head = args->head;
}

void printAndExit(THCObject * this, THCList * args){
    evaluate(this, args->head->contents.n.value);
    int i = args->head->contents.n.value->contents.i;
    printf("final result: %d\n", i);
    exit(0);
}

void add_func(THCObject * this, THCList * args){
    evaluate(this, args->head->contents.n.value);
    evaluate(this, args->head->contents.n.next->contents.n.value);

    THCObject * cont = this->contents.f.cont;

    int x = args->head->contents.n.value->contents.i;
    int y = args->head->contents.n.next->contents.n.value->contents.i;

    GCFlag gcflag = this->gcflag;
    constructInt(this, x+y);
    this->gcflag = gcflag;
    callCont(cont, this);
}

void mult_func(THCObject * this, THCList * args){
    evaluate(this, args->head->contents.n.value);
    evaluate(this, args->head->contents.n.next->contents.n.value);

    THCObject * cont = this->contents.f.cont;

    int x = args->head->contents.n.value->contents.i;
    int y = args->head->contents.n.next->contents.n.value->contents.i;

    GCFlag gcflag = this->gcflag;
    constructInt(this, x*y);
    this->gcflag = gcflag;
    callCont(cont, this);
}

void initDefaults(){
    Default_add_obj = constructFunction(add_func, 2);
    Default_mult_obj = constructFunction(mult_func, 2);
    THCprintResult = constructFunction(printAndExit, 1);
    THCcallExcessArgs = constructFunction(callExcessArgs, 2);
    THCprintResult->contents.f.shouldeval = 1;
    
    THCevalAndContinue = constructFunction(evalAndContinue, 2);
    THCevalAndContinue->contents.f.shouldeval = 1;

    initHeap();
}


