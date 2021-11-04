#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "data.h"

void nop(THCObject * n){}

THCObject * constructFunction(void (*func)(THCObject *, THCList *), int arity){
    THCObject * obj = malloc(sizeof(THCObject));
    obj->evaluated = NULL;
    obj->is_evaluated = 0;
    obj->evaluate = NULL;
    
    THCFunc * f =&(obj->contents.f);
    f->arity = arity;
    f->f = func;
    f->cont = NULL;
    f->args.head = NULL;
    f->args.size = 0;


    return obj;
}

inline void constructInt(THCObject * obj, int i){
    obj->contents.i = i;
    obj->constructor = 0;
    obj->evaluated = obj;
    obj->is_evaluated = 1;
    obj->evaluate = nop;
}

inline void constructContinuation(THCObject * dest, THCObject * src, THCObject * cont){
    memcpy(dest, src, sizeof(THCObject));
    dest->contents.f.cont = cont;
}

void callCont(THCObject * cont, THCObject * arg){
    THCNode new_node;
    new_node.value = arg;
    new_node.next = cont->contents.f.args.head;
    
    THCList new_list;
    new_list.size = cont->contents.f.args.size+1;
    new_list.head = &new_node;

    
    THCFunc * f = &(cont->contents.f);
    f->f(f->cont, &new_list);
}

void callFunction(THCObject * func){
    THCFunc * f = &(func->contents.f);
    f->f(f->cont, &(f->args));
}

void applyFunctionRaw(THCObject * cont, THCObject * f, THCObject * arg){
    // initialize new function
    THCObject new_obj;
    THCNode new_node;

    memcpy(&new_obj, f, sizeof(THCObject)); 
    
    THCFunc * new_func = &(new_obj.contents.f);

    new_node.value = arg;
    new_node.next = new_func->args.head;
    new_func->args.head = &new_node;
    new_func->args.size++;

    if(new_func->args.size >= new_func->arity){
        // eager evaluation
        new_func->cont = cont;
        callFunction(&new_obj);
    } else {
        // add function to continuation and call
        callCont(cont, &new_obj);
    }

}


void applyFunction(THCObject * cont, THCList * args){
    applyFunctionRaw(cont, args->head->next->value, args->head->value);
}

void printAndExit(THCObject * cont, THCList * args){
    int i = args->head->value->contents.i;
    printf("%d\n", i);
    exit(0);

}

void add_func(THCObject * cont, THCList * args){
    int x = args->head->value->contents.i;
    int y = args->head->next->value->contents.i;
    THCObject ret;
    constructInt(&ret, x+y);
    callCont(cont, &ret);
}

THCObject * THCFuncApp;
THCObject * THCprintResult;
THCObject * THCadd;

void initDefaults(){
    THCFuncApp = constructFunction(applyFunction, 2);
    THCadd = constructFunction(add_func, 2);
    THCprintResult = constructFunction(printAndExit, 1);
}


