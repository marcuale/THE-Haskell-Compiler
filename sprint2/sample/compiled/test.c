#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

enum THCTypeListConstrs {
    THCTypeListEmpty,
    THCTypeListNode,
};


THCObject * THCsquare;
THCObject * THClen;
THCObject * THClencont0;
THCObject * THCConstrListNode;
THCObject * THChead;

THCObject * THCUnitListEmpty;

THCObject * THCmain;
THCObject * THCmaincont0;
THCObject * THCmaincont1;
THCObject * THCmaincont2;
THCObject * THCmaincont3;
THCObject * THCmaincont4;
THCObject * THCmaincont5;
THCObject * THCmaincont6;
THCObject * THCmaincont7;

void square_func(THCObject * cont, THCList * args){
    int x = args->head->value->contents.i;
    THCObject ret;
    constructInt(&ret, x*x);
    callCont(cont, &ret);
}

void len_cont0(THCObject * cont, THCList * args){
    int x = args->head->value->contents.i;
    THCObject ret;
    constructInt(&ret, x+1);
    callCont(cont, &ret);
}

void head_func(THCObject * cont, THCList * args){
    THCObject * l = args->head->value;
    if(l->constructor == THCTypeListEmpty){
        fprintf(stderr, "EXPLOSION OH NOOO ASLKFDJSLK\n");
        exit(-1);
    }else if (l->constructor == THCTypeListNode){
        THCNode * b = l->contents.l.head;
        THCNode * c = b->next;
        
        callCont(cont, c->value);
    }
}

void len_func(THCObject * cont, THCList * args){
    THCObject * l = args->head->value;
    if(l->constructor == THCTypeListEmpty){
        THCObject ret;
        constructInt(&ret, 0);
        callCont(cont, &ret);
    }else if (l->constructor == THCTypeListNode){
        THCNode * b = l->contents.l.head;
        THCNode * c = b->next;
        
        THCObject cont0;
        constructContinuation(&cont0, THClencont0, cont);
        applyFunctionRaw(&cont0, THClen, b->value);
    }
}

void funcListNode(THCObject * cont, THCList * args){
    THCObject ret;
    constructInt(&ret, 0);
    ret.constructor = THCTypeListNode;
    ret.contents.l.head = args->head;
    ret.contents.l.size = args->size;
    callCont(cont, &ret);
}

void main_cont7(THCObject * cont, THCList * args){
    applyFunctionRaw(cont, THClen, args->head->value);
}

void main_cont6(THCObject * cont, THCList * args){
    THCObject cont7;
    constructContinuation(&cont7, THCmaincont7, cont);
    
    applyFunctionRaw(&cont7, args->head->value, THCUnitListEmpty);
}

void main_cont5(THCObject * cont, THCList * args){
    THCObject o;
    constructInt(&o, 2);
    
    THCObject cont6;
    constructContinuation(&cont6, THCmaincont6, cont);
    
    applyFunctionRaw(&cont6, THCConstrListNode, &o);
}

void main_cont4(THCObject * cont, THCList * args){
    THCObject cont5;
    constructContinuation(&cont5, args->head->value, cont);
    
    // TODO: make this one not a hack
    THCObject cont_next;
    constructContinuation(&cont_next, THCmaincont5, &cont5);
    callFunction(&cont_next);
}

void main_cont3(THCObject * cont, THCList * args){
    THCObject cont4;
    constructContinuation(&cont4, THCmaincont4, cont);
    
    applyFunctionRaw(&cont4, THCadd, args->head->value);
}

void main_cont2(THCObject * cont, THCList * args){
    THCObject cont3;
    constructContinuation(&cont3, THCmaincont3, cont);
    
    applyFunctionRaw(&cont3, THCsquare, args->head->value);
}

void main_cont1(THCObject * cont, THCList * args){
    THCObject cont2;
    constructContinuation(&cont2, THCmaincont2, cont);
    
    applyFunctionRaw(&cont2, THChead, args->head->value);
}

void main_cont0(THCObject * cont, THCList * args){
    
    THCObject cont1;
    constructContinuation(&cont1, THCmaincont1, cont);
    
    applyFunctionRaw(&cont1, args->head->value, THCUnitListEmpty);
}

void main_func(THCObject * cont, THCList * args){
    THCObject o;
    constructInt(&o, 2);
    
    THCObject cont0;
    constructContinuation(&cont0, THCmaincont0, cont);
    
    applyFunctionRaw(&cont0, THCConstrListNode, &o);
}

void THCTopLevel(){

    THCObject new_main;
    memcpy(&new_main, THCmain, sizeof(THCObject));

    new_main.contents.f.cont = THCprintResult;
    callFunction(&new_main);
}


void initTest(){
    THCmain = constructFunction(main_func, 0);
    THCsquare = constructFunction(square_func, 1);
    THClen = constructFunction(len_func, 1);
    THClencont0 = constructFunction(len_cont0, 1);
    THCConstrListNode = constructFunction(funcListNode, 2);
    THChead = constructFunction(head_func, 1);
    
    THCmaincont0 = constructFunction(main_cont0, 1);
    THCmaincont1 = constructFunction(main_cont1, 1);
    THCmaincont2 = constructFunction(main_cont2, 1);
    THCmaincont3 = constructFunction(main_cont3, 1);
    THCmaincont4 = constructFunction(main_cont4, 1);
    THCmaincont5 = constructFunction(main_cont5, 0);
    THCmaincont6 = constructFunction(main_cont6, 1);
    THCmaincont7 = constructFunction(main_cont7, 1);

    THCUnitListEmpty = malloc(sizeof(THCObject));
    constructInt(THCUnitListEmpty, 0);
    THCUnitListEmpty->constructor = THCTypeListEmpty;
}

int main(){
    initDefaults();
    initTest();
    THCTopLevel();
}
