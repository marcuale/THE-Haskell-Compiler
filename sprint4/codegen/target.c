 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"
enum THCTypeListConstrs{
 E_TYPE_List_CONSTR_Node,
 E_TYPE_List_CONSTR_Empty,
};
enum THCTypeMaybeConstrs{
 E_TYPE_Maybe_CONSTR_Nothing,
 E_TYPE_Maybe_CONSTR_Just,
};
enum THCTypeMaybeTConstrs{
 E_TYPE_MaybeT_CONSTR_MaybeT,
};
THCObject * Constr_Node_obj;
THCObject * Constr_Empty_obj;
THCObject * Constr_Nothing_obj;
THCObject * Constr_Just_obj;
THCObject * Constr_MaybeT_obj;
THCObject * Func_Colon_obj;
THCObject * Func_main_obj;
THCObject * Func_head_obj;
THCObject * Func_fromJust_obj;
THCObject * Func_fromMaybeT_obj;
THCObject * Func_main_C0_obj;
THCObject * Func_main_C1_obj;
THCObject * Func_main_C2_obj;
THCObject * Func_main_C3_obj;
THCObject * Func_main_C4_obj;
THCObject * Func_main_C5_obj;
THCObject * Func_main_C6_obj;
THCObject * Func_head_C0_obj;
THCObject * Func_head_C1_obj;
THCObject * Func_fromJust_C0_obj;
THCObject * Func_fromJust_C1_obj;
THCObject * Func_fromMaybeT_C0_obj;
THCObject * Func_fromMaybeT_C1_obj;
void Constr_Node(THCObject * this, THCList * args){
                if(!checkStackSize()) THC_minor_gc(this);
                THCObject * cont = this->contents.f.cont;
                THCObject value;
                constructAlgebraic(&value, E_TYPE_List_CONSTR_Node);
                value.contents.l.head = args->head;
                assert(args->size == 2);
                value.contents.l.size = 2;
                callCont(cont, &value);
}
void Constr_Just(THCObject * this, THCList * args){
                if(!checkStackSize()) THC_minor_gc(this);
                THCObject * cont = this->contents.f.cont;
                THCObject value;
                constructAlgebraic(&value, E_TYPE_Maybe_CONSTR_Just);
                value.contents.l.head = args->head;
                assert(args->size == 1);
                value.contents.l.size = 1;
                callCont(cont, &value);
}
void Constr_MaybeT(THCObject * this, THCList * args){
                if(!checkStackSize()) THC_minor_gc(this);
                THCObject * cont = this->contents.f.cont;
                THCObject value;
                constructAlgebraic(&value, E_TYPE_MaybeT_CONSTR_MaybeT);
                value.contents.l.head = args->head;
                assert(args->size == 1);
                value.contents.l.size = 1;
                callCont(cont, &value);
}
void Func_main_C6(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Constr_Just_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 2);

            callCont(&funccont, &lit);
            
        }
void Func_main_C5(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_Colon_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_main_C4_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C6_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C4(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = Constr_Empty_obj;

            callCont(&funccont, arg);
            
        }
void Func_main_C3(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Constr_MaybeT_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C5_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C2(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_fromMaybeT_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C3_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_head_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C2_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_fromJust_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C1_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_head_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * next_cont = cont;
            THCObject * v0;
            THCObject * v0p = indexList(args, 0);
v0 = indexList(&(v0p->contents.l), 1);

            callCont(next_cont, v0);
        }
void Func_head_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * expr = indexList(args, 0);

            
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){
                
                case E_TYPE_List_CONSTR_Node:
                    constructContinuation(&next, Func_head_C1_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `head_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&next, args);
            callCont(&next, expr);
        }
void Func_fromJust_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * next_cont = cont;
            THCObject * v0;
            THCObject * v0p = indexList(args, 0);
v0 = indexList(&(v0p->contents.l), 0);

            callCont(next_cont, v0);
        }
void Func_fromJust_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * expr = indexList(args, 0);

            
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){
                
                case E_TYPE_Maybe_CONSTR_Just:
                    constructContinuation(&next, Func_fromJust_C1_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `fromJust_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&next, args);
            callCont(&next, expr);
        }
void Func_fromMaybeT_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * next_cont = cont;
            THCObject * v0;
            THCObject * v0p = indexList(args, 0);
v0 = indexList(&(v0p->contents.l), 0);

            callCont(next_cont, v0);
        }
void Func_fromMaybeT_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * expr = indexList(args, 0);

            
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){
                
                case E_TYPE_MaybeT_CONSTR_MaybeT:
                    constructContinuation(&next, Func_fromMaybeT_C1_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `fromMaybeT_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&next, args);
            callCont(&next, expr);
        }
void initSelf() {
Constr_Node_obj = constructFunction(Constr_Node, 2);

                Constr_Empty_obj = malloc(sizeof(THCObject));
                constructAlgebraic(Constr_Empty_obj, E_TYPE_List_CONSTR_Empty);
                Constr_Empty_obj->gcflag = HEAP;
                Constr_Empty_obj->contents.l.head = NULL;
                Constr_Empty_obj->contents.l.size = 0;

                
                Constr_Nothing_obj = malloc(sizeof(THCObject));
                constructAlgebraic(Constr_Nothing_obj, E_TYPE_Maybe_CONSTR_Nothing);
                Constr_Nothing_obj->gcflag = HEAP;
                Constr_Nothing_obj->contents.l.head = NULL;
                Constr_Nothing_obj->contents.l.size = 0;

                Constr_Just_obj = constructFunction(Constr_Just, 1);
Constr_MaybeT_obj = constructFunction(Constr_MaybeT, 1);
Func_Colon_obj = Constr_Node_obj;
Func_main_C0_obj = constructFunction(Func_main_C0, 0);
Func_main_C1_obj = constructFunction(Func_main_C1, 0);
Func_main_C2_obj = constructFunction(Func_main_C2, 0);
Func_main_C3_obj = constructFunction(Func_main_C3, 0);
Func_main_C4_obj = constructFunction(Func_main_C4, 1);
Func_main_C5_obj = constructFunction(Func_main_C5, 0);
Func_main_C6_obj = constructFunction(Func_main_C6, 0);
Func_main_obj = Func_main_C0_obj;Func_head_C0_obj = constructFunction(Func_head_C0, 1);
Func_head_C1_obj = constructFunction(Func_head_C1, 2);
Func_head_obj = Func_head_C0_obj;Func_fromJust_C0_obj = constructFunction(Func_fromJust_C0, 1);
Func_fromJust_C1_obj = constructFunction(Func_fromJust_C1, 2);
Func_fromJust_obj = Func_fromJust_C0_obj;Func_fromMaybeT_C0_obj = constructFunction(Func_fromMaybeT_C0, 1);
Func_fromMaybeT_C1_obj = constructFunction(Func_fromMaybeT_C1, 2);
Func_fromMaybeT_obj = Func_fromMaybeT_C0_obj;
}

int main(){
    initDefaults();
    initSelf();

    THCObject new_main;
    constructContinuation(&new_main, Func_main_obj, THCprintResult);
    THC_driver_loop(&new_main);
}
