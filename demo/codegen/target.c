 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"
enum THCTypeListConstrs{
 E_TYPE_List_CONSTR_Node,
 E_TYPE_List_CONSTR_Empty,
};
THCObject * Constr_Node_obj;
THCObject * Constr_Empty_obj;
THCObject * Func_Colon_obj;
THCObject * Func_zipWith_obj;
THCObject * Func_add_obj;
THCObject * Func_tail_obj;
THCObject * Func_unsafePrintAll_obj;
THCObject * Func_fibs_obj;
THCObject * Func_main_obj;
THCObject * Func_zipWith_C0_obj;
THCObject * Func_zipWith_C1_obj;
THCObject * Func_zipWith_C2_obj;
THCObject * Func_zipWith_C3_obj;
THCObject * Func_zipWith_C4_obj;
THCObject * Func_zipWith_C5_obj;
THCObject * Func_zipWith_C6_obj;
THCObject * Func_zipWith_C7_obj;
THCObject * Func_zipWith_C8_obj;
THCObject * Func_zipWith_C9_obj;
THCObject * Func_zipWith_C10_obj;
THCObject * Func_add_C0_obj;
THCObject * Func_add_C1_obj;
THCObject * Func_tail_C0_obj;
THCObject * Func_tail_C1_obj;
THCObject * Func_unsafePrintAll_C0_obj;
THCObject * Func_unsafePrintAll_C1_obj;
THCObject * Func_unsafePrintAll_C2_obj;
THCObject * Func_unsafePrintAll_C3_obj;
THCObject * Func_unsafePrintAll_C4_obj;
THCObject * Func_unsafePrintAll_C5_obj;
THCObject * Func_fibs_C0_obj;
THCObject * Func_fibs_C1_obj;
THCObject * Func_fibs_C2_obj;
THCObject * Func_fibs_C3_obj;
THCObject * Func_fibs_C4_obj;
THCObject * Func_fibs_C5_obj;
THCObject * Func_fibs_C6_obj;
THCObject * Func_fibs_C7_obj;
THCObject * Func_main_C0_obj;
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
void Func_zipWith_C5(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = indexList(args, 4);

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_zipWith_C4_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * argp = indexList(args, 1);
THCObject * arg = indexList(&(argp->contents.l), 1);

            callCont(&funccont, arg);
            
        }
void Func_zipWith_C4(THCObject * this, THCList * args){
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
            
            THCObject * argp = indexList(args, 0);
THCObject * arg = indexList(&(argp->contents.l), 1);

            callCont(&funccont, arg);
            
        }
void Func_zipWith_C3(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Constr_Node_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_zipWith_C2_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_zipWith_C5_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_zipWith_C8(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_zipWith_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_zipWith_C7_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = indexList(args, 4);

            callCont(&funccont, arg);
            
        }
void Func_zipWith_C7(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_zipWith_C6_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * argp = indexList(args, 1);
THCObject * arg = indexList(&(argp->contents.l), 0);

            callCont(&funccont, arg);
            
        }
void Func_zipWith_C6(THCObject * this, THCList * args){
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
            
            THCObject * argp = indexList(args, 0);
THCObject * arg = indexList(&(argp->contents.l), 0);

            callCont(&funccont, arg);
            
        }
void Func_zipWith_C2(THCObject * this, THCList * args){
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
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_zipWith_C8_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_zipWith_C9(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * next_cont = cont;
            THCObject * v0;
            v0 = Constr_Empty_obj;

            callCont(next_cont, v0);
        }
void Func_zipWith_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * expr = indexList(args, 1);

            
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){
                
                case E_TYPE_List_CONSTR_Node:
                    constructContinuation(&next, Func_zipWith_C3_obj, cont);
                    break;

                case E_TYPE_List_CONSTR_Empty:
                    constructContinuation(&next, Func_zipWith_C9_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `zipWith_C1'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&next, args);
            callCont(&next, expr);
        }
void Func_zipWith_C10(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * next_cont = cont;
            THCObject * v0;
            v0 = Constr_Empty_obj;

            callCont(next_cont, v0);
        }
void Func_zipWith_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * expr = indexList(args, 1);

            
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){
                
                case E_TYPE_List_CONSTR_Node:
                    constructContinuation(&next, Func_zipWith_C1_obj, cont);
                    break;

                case E_TYPE_List_CONSTR_Empty:
                    constructContinuation(&next, Func_zipWith_C10_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `zipWith_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&next, args);
            callCont(&next, expr);
        }
void Func_add_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Default_add_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_add_C0_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = indexList(args, 1);

            callCont(&funccont, arg);
            
        }
void Func_add_C0(THCObject * this, THCList * args){
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
            
            THCObject * arg = indexList(args, 0);

            callCont(&funccont, arg);
            
        }
void Func_tail_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * next_cont = cont;
            THCObject * v0;
            THCObject * v0p = indexList(args, 0);
v0 = indexList(&(v0p->contents.l), 0);

            callCont(next_cont, v0);
        }
void Func_tail_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * expr = indexList(args, 0);

            
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){
                
                case E_TYPE_List_CONSTR_Node:
                    constructContinuation(&next, Func_tail_C1_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `tail_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&next, args);
            callCont(&next, expr);
        }
void Func_unsafePrintAll_C4(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Default_unsafePrint_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * argp = indexList(args, 0);
THCObject * arg = indexList(&(argp->contents.l), 1);

            callCont(&funccont, arg);
            
        }
void Func_unsafePrintAll_C3(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Default_seq_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_unsafePrintAll_C2_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_unsafePrintAll_C4_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_unsafePrintAll_C2(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_unsafePrintAll_C1_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = Func_unsafePrintAll_obj;

            callCont(&funccont, arg);
            
        }
void Func_unsafePrintAll_C1(THCObject * this, THCList * args){
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
            
            THCObject * argp = indexList(args, 0);
THCObject * arg = indexList(&(argp->contents.l), 0);

            callCont(&funccont, arg);
            
        }
void Func_unsafePrintAll_C5(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * next_cont = cont;

            THCObject object;
            constructInt(&object, 0);
            callCont(next_cont, &object);}
void Func_unsafePrintAll_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * expr = indexList(args, 0);

            
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){
                
                case E_TYPE_List_CONSTR_Node:
                    constructContinuation(&next, Func_unsafePrintAll_C3_obj, cont);
                    break;

                case E_TYPE_List_CONSTR_Empty:
                    constructContinuation(&next, Func_unsafePrintAll_C5_obj, cont);
                    break;


                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `safePrintAll_C0'.\n");
                    printf("%d\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\n", *x);
                    exit(-1);
            }
            setArgs(&next, args);
            callCont(&next, expr);
        }
void Func_fibs_C1(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_Colon_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_fibs_C0_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 0);

            callCont(&funccont, &lit);
            
        }
void Func_fibs_C3(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_Colon_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_fibs_C2_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject lit;
            constructInt(&lit, 1);

            callCont(&funccont, &lit);
            
        }
void Func_fibs_C6(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_zipWith_obj;

            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_fibs_C5_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = Func_add_obj;

            callCont(&funccont, arg);
            
        }
void Func_fibs_C5(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            
            
            
        THCObject nc0;
        constructContinuation(&nc0, Func_fibs_C4_obj, cont);
        setArgs(&nc0, args);
        THCObject * next_cont = &nc0;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = Func_fibs_obj;

            callCont(&funccont, arg);
            
        }
void Func_fibs_C7(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_tail_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = Func_fibs_obj;

            callCont(&funccont, arg);
            
        }
void Func_fibs_C4(THCObject * this, THCList * args){
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
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_fibs_C7_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_fibs_C2(THCObject * this, THCList * args){
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
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_fibs_C6_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_fibs_C0(THCObject * this, THCList * args){
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
            
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, Func_fibs_C3_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            
        }
void Func_main_C0(THCObject * this, THCList * args){
if(!checkStackSize()) THC_minor_gc(this);
THCObject * cont = this->contents.f.cont;

            THCObject * func = Func_unsafePrintAll_obj;

            
            THCObject * next_cont = cont;
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            
            THCObject * arg = Func_fibs_obj;

            callCont(&funccont, arg);
            
        }
void initSelf() {
Constr_Node_obj = constructFunction(Constr_Node, 2);

                Constr_Empty_obj = malloc(sizeof(THCObject));
                constructAlgebraic(Constr_Empty_obj, E_TYPE_List_CONSTR_Empty);
                Constr_Empty_obj->gcflag = HEAP;
                Constr_Empty_obj->contents.l.head = NULL;
                Constr_Empty_obj->contents.l.size = 0;

                Func_Colon_obj = Constr_Node_obj;
Func_zipWith_C0_obj = constructFunction(Func_zipWith_C0, 3);
Func_zipWith_C1_obj = constructFunction(Func_zipWith_C1, 4);
Func_zipWith_C2_obj = constructFunction(Func_zipWith_C2, 6);
Func_zipWith_C3_obj = constructFunction(Func_zipWith_C3, 5);
Func_zipWith_C4_obj = constructFunction(Func_zipWith_C4, 6);
Func_zipWith_C5_obj = constructFunction(Func_zipWith_C5, 5);
Func_zipWith_C6_obj = constructFunction(Func_zipWith_C6, 6);
Func_zipWith_C7_obj = constructFunction(Func_zipWith_C7, 6);
Func_zipWith_C8_obj = constructFunction(Func_zipWith_C8, 5);
Func_zipWith_C9_obj = constructFunction(Func_zipWith_C9, 5);
Func_zipWith_C10_obj = constructFunction(Func_zipWith_C10, 4);
Func_zipWith_obj = Func_zipWith_C0_obj;Func_add_C0_obj = constructFunction(Func_add_C0, 3);
Func_add_C1_obj = constructFunction(Func_add_C1, 2);
Func_add_obj = Func_add_C1_obj;Func_tail_C0_obj = constructFunction(Func_tail_C0, 1);
Func_tail_C1_obj = constructFunction(Func_tail_C1, 2);
Func_tail_obj = Func_tail_C0_obj;Func_unsafePrintAll_C0_obj = constructFunction(Func_unsafePrintAll_C0, 1);
Func_unsafePrintAll_C1_obj = constructFunction(Func_unsafePrintAll_C1, 3);
Func_unsafePrintAll_C2_obj = constructFunction(Func_unsafePrintAll_C2, 3);
Func_unsafePrintAll_C3_obj = constructFunction(Func_unsafePrintAll_C3, 2);
Func_unsafePrintAll_C4_obj = constructFunction(Func_unsafePrintAll_C4, 2);
Func_unsafePrintAll_C5_obj = constructFunction(Func_unsafePrintAll_C5, 2);
Func_unsafePrintAll_obj = Func_unsafePrintAll_C0_obj;Func_fibs_C0_obj = constructFunction(Func_fibs_C0, 1);
Func_fibs_C1_obj = constructFunction(Func_fibs_C1, 0);
Func_fibs_C2_obj = constructFunction(Func_fibs_C2, 1);
Func_fibs_C3_obj = constructFunction(Func_fibs_C3, 0);
Func_fibs_C4_obj = constructFunction(Func_fibs_C4, 1);
Func_fibs_C5_obj = constructFunction(Func_fibs_C5, 1);
Func_fibs_C6_obj = constructFunction(Func_fibs_C6, 0);
Func_fibs_C7_obj = constructFunction(Func_fibs_C7, 0);
Func_fibs_obj = Func_fibs_C1_obj;Func_main_C0_obj = constructFunction(Func_main_C0, 0);
Func_main_obj = Func_main_C0_obj;
}

int main(){
    initDefaults();
    initSelf();

    THCObject new_main;
    constructContinuation(&new_main, Func_main_obj, THCprintResult);
    THC_driver_loop(&new_main);
}
