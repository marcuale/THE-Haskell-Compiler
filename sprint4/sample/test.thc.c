 
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
THCObject * Constr_Node_obj;
THCObject * Constr_Empty_obj;
THCObject * Constr_Nothing_obj;
THCObject * Constr_Just_obj;
THCObject * Func_Colon_obj;
THCObject * Func_square_obj;
THCObject * Func_three_obj;
THCObject * Func_main_obj;
THCObject * Func_len_obj;
THCObject * Func_head_obj;
THCObject * Func_square_C0_obj;
THCObject * Func_square_C1_obj;
THCObject * Func_three_C0_obj;
THCObject * Func_three_C1_obj;
THCObject * Func_three_C2_obj;
THCObject * Func_three_C3_obj;
THCObject * Func_main_C0_obj;
THCObject * Func_main_C1_obj;
THCObject * Func_main_C2_obj;
THCObject * Func_main_C3_obj;
THCObject * Func_main_C4_obj;
THCObject * Func_main_C5_obj;
THCObject * Func_main_C6_obj;
THCObject * Func_main_C7_obj;
THCObject * Func_main_C8_obj;
THCObject * Func_len_C0_obj;
THCObject * Func_len_C1_obj;
THCObject * Func_len_C2_obj;
THCObject * Func_len_C3_obj;
THCObject * Func_len_C4_obj;
THCObject * Func_head_C0_obj;
THCObject * Func_head_C1_obj;
void Constr_Node(THCObject * cont, THCList * args){
                if(!checkStackSize()) THC_minor_gc(Constr_Node, cont, args);
                THCObject object;
                constructAlgebraic(&object, E_TYPE_List_CONSTR_Node);
                object.contents.l.head = args->head;
                assert(args->size == 2);
                object.contents.l.size = 2;
                callCont(cont, &object);
}
void Constr_Just(THCObject * cont, THCList * args){
                if(!checkStackSize()) THC_minor_gc(Constr_Just, cont, args);
                THCObject object;
                constructAlgebraic(&object, E_TYPE_Maybe_CONSTR_Just);
                object.contents.l.head = args->head;
                assert(args->size == 1);
                object.contents.l.size = 1;
                callCont(cont, &object);
}
void Func_square_C1(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_square_C1, cont, args);

            THCObject * func = Default_mult_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_square_C0_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = indexList(args, 0);

            callCont(&funccont, arg);
            
        }
void Func_square_C0(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_square_C0, cont, args);

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = indexList(args, 0);

            callCont(&funccont, arg);
            
        }
void Func_three_C3(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_three_C3, cont, args);

            THCObject * func = Default_add_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_three_C2_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 1);

            callCont(&funccont, &lit);
            
        }
void Func_three_C2(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_three_C2, cont, args);

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 1);

            callCont(&funccont, &lit);
            
        }
void Func_three_C1(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_three_C1, cont, args);

            THCObject * func = Default_add_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_three_C0_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject argcont;
            constructContinuation(&argcont, Func_three_C3_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_three_C0(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_three_C0, cont, args);

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 1);

            callCont(&funccont, &lit);
            
        }
void Func_main_C5(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C5, cont, args);

            THCObject * func = Func_Colon_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_main_C4_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 2);

            callCont(&funccont, &lit);
            
        }
void Func_main_C4(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C4, cont, args);

            
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
void Func_main_C3(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C3, cont, args);

            THCObject * func = Func_head_obj;

            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C5_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C2(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C2, cont, args);

            THCObject * func = Func_square_obj;

            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C3_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C1(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C1, cont, args);

            THCObject * func = Default_add_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_main_C0_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C2_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C8(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C8, cont, args);

            THCObject * func = Func_Colon_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_main_C7_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 2);

            callCont(&funccont, &lit);
            
        }
void Func_main_C7(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C7, cont, args);

            
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
void Func_main_C6(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C6, cont, args);

            THCObject * func = Func_len_obj;

            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C8_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C0(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_main_C0, cont, args);

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject argcont;
            constructContinuation(&argcont, Func_main_C6_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_len_C2(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_len_C2, cont, args);

            THCObject * func = Default_add_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_len_C1_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 1);

            callCont(&funccont, &lit);
            
        }
void Func_len_C3(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_len_C3, cont, args);

            THCObject * func = Func_len_obj;

            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * argp = indexList(args, 0);
THCObject * arg = indexList(&(argp->contents.l), 0);

            callCont(&funccont, arg);
            
        }
void Func_len_C1(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_len_C1, cont, args);

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            THCObject * next_cont = cont;
            
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject argcont;
            constructContinuation(&argcont, Func_len_C3_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_len_C4(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_len_C4, cont, args);

            THCObject * next_cont = cont;
            THCObject object;
            constructInt(&object, 0);
            callCont(next_cont, &object);}
void Func_len_C0(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_len_C0, cont, args);

            
            THCObject * expr = indexList(args, 0);

            
            THCObject case_cont;
            switch(expr->constructor){
                
                case E_TYPE_List_CONSTR_Node:
                    constructContinuation(&case_cont, Func_len_C2_obj, cont);
                    break;

                case E_TYPE_List_CONSTR_Empty:
                    constructContinuation(&case_cont, Func_len_C4_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `len_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&case_cont, args);
            callCont(&case_cont, expr);
        }
void Func_head_C1(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_head_C1, cont, args);

            THCObject * next_cont = cont;
            THCObject * v0;
            THCObject * v0p = indexList(args, 0);
v0 = indexList(&(v0p->contents.l), 1);

            callCont(next_cont, v0);
        }
void Func_head_C0(THCObject * cont, THCList * args){
if(!checkStackSize()) THC_minor_gc(Func_head_C0, cont, args);

            
            THCObject * expr = indexList(args, 0);

            
            THCObject case_cont;
            switch(expr->constructor){
                
                case E_TYPE_List_CONSTR_Node:
                    constructContinuation(&case_cont, Func_head_C1_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `head_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&case_cont, args);
            callCont(&case_cont, expr);
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
Func_Colon_obj = Constr_Node_obj;
Func_square_C0_obj = constructFunction(Func_square_C0, 2);
Func_square_C1_obj = constructFunction(Func_square_C1, 1);
Func_square_obj = Func_square_C1_obj;Func_three_C0_obj = constructFunction(Func_three_C0, 1);
Func_three_C1_obj = constructFunction(Func_three_C1, 0);
Func_three_C2_obj = constructFunction(Func_three_C2, 1);
Func_three_C3_obj = constructFunction(Func_three_C3, 0);
Func_three_obj = Func_three_C1_obj;Func_main_C0_obj = constructFunction(Func_main_C0, 1);
Func_main_C1_obj = constructFunction(Func_main_C1, 0);
Func_main_C2_obj = constructFunction(Func_main_C2, 0);
Func_main_C3_obj = constructFunction(Func_main_C3, 0);
Func_main_C4_obj = constructFunction(Func_main_C4, 1);
Func_main_C5_obj = constructFunction(Func_main_C5, 0);
Func_main_C6_obj = constructFunction(Func_main_C6, 0);
Func_main_C7_obj = constructFunction(Func_main_C7, 1);
Func_main_C8_obj = constructFunction(Func_main_C8, 0);
Func_main_obj = Func_main_C1_obj;Func_len_C0_obj = constructFunction(Func_len_C0, 1);
Func_len_C1_obj = constructFunction(Func_len_C1, 3);
Func_len_C2_obj = constructFunction(Func_len_C2, 2);
Func_len_C3_obj = constructFunction(Func_len_C3, 2);
Func_len_C4_obj = constructFunction(Func_len_C4, 2);
Func_len_obj = Func_len_C0_obj;Func_head_C0_obj = constructFunction(Func_head_C0, 1);
Func_head_C1_obj = constructFunction(Func_head_C1, 2);
Func_head_obj = Func_head_C0_obj;
}

int main(){
    initDefaults();
    initSelf();

    THCObject new_main;
    constructContinuation(&new_main, Func_main_obj, THCprintResult);
    THC_driver_loop(&new_main);
}
