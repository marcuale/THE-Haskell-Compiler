#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <stddef.h>

#include "data.h"

// TODO: determine this in a better way
#define STACK_LIMIT (1<<19) // 512KiB
#define HEAP_INIT (1<<12) // 4KiB


struct THCHeap {
    THCObject * from;
    THCObject * to; // contains no valid references
    size_t cheneyStart;
    size_t size;
    size_t max;

    int need_major;
} heap;


void initHeap(){
    heap.size = 0;
    heap.cheneyStart = 0;
    heap.max = HEAP_INIT;
    heap.from = malloc(sizeof(THCObject) * heap.max);
    heap.to   = malloc(sizeof(THCObject) * heap.max); // contains no valid references 
}

THCObject * saved_cont_call;
jmp_buf empty_stack_state;
// We assume everything is stack allocated
size_t stack_top;


void THC_driver_loop(THCObject * call){
    int x=0;
    stack_top = (size_t) &x;
    if(setjmp(empty_stack_state))
        call = saved_cont_call;

    callFunction(call);
}


int checkStackSize(){
    int x=0;
    // give 512 byte buffer
    return stack_top - (size_t)&x + 512 < STACK_LIMIT;
}

inline void movePtr(THCObject * old, ptrdiff_t offset, THCObject ** ptr){
    if(*ptr != NULL && *ptr >= old && *ptr < old + heap.size) *ptr += offset;
}

void moveAfterRealloc(THCObject * old, ptrdiff_t offset){
    if(offset == 0)
        return;
    for(size_t i=0; i<heap.size; i++){
        THCObject * curr = heap.from + i;
        switch(curr->type){
            case ALGEBRAIC:
                movePtr(old, offset, &(curr->contents.l.head));
                break;
            case FUNCTION:
                movePtr(old, offset, &(curr->contents.f.cont));
                movePtr(old, offset, &(curr->contents.f.args.head));
                break;
            case NODE:
                movePtr(old, offset, &(curr->contents.n.value));
                movePtr(old, offset, &(curr->contents.n.next));
                break;
            case ATOMIC: break;
            default:
                printf("Something may have gone wrong during reallocation. %p %d\n", curr, curr->type);
                break;
        }
    }
}

THCObject * majorAlloc(){
    return heap.from + heap.size++;
}

THCObject * THC_major_gc_object(THCObject * obj){
    if(obj == NULL) return NULL;
    while(obj->gcflag == COPIED){
        obj = heap.from + obj->gc_forward;
    }

    // if object is in toheap
    if(obj->gcflag == HEAP && heap.from <= obj && obj < heap.from + heap.max)
        return obj;

    // global objects need not be copied
    if(obj->gcflag == GLOBAL)
        return obj;
    

    THCObject * newobj = majorAlloc();
    *newobj = *obj;
    newobj->gcflag = HEAP;
    obj->gcflag = COPIED;
    obj->gc_forward = newobj - heap.from;
    return newobj;
}

void THC_major_gc(){
    size_t start_max = heap.max;
    // switch tospace and fromspace. now we copy to fromspace from tospace
    THCObject * t = heap.to;
    heap.to = heap.from;
    heap.from = t;

    // copy saved continuation to new heap
    *(heap.from) = *saved_cont_call;
    
    heap.cheneyStart = 0;
    heap.size = 1;
    
    while(heap.cheneyStart < heap.size){
        if(heap.max - heap.size <= 4){
            heap.max *= 2;
            THCObject * old = heap.from;
            heap.from = realloc(heap.from, sizeof(THCObject) * heap.max);
            moveAfterRealloc(old, heap.from - old);
        }
        THCObject * curr = heap.from + heap.cheneyStart;
        switch(curr->type){
            case ATOMIC: 
                //printf("%d\n", curr->contents.i);
                break;
            case ALGEBRAIC:
                curr->contents.l.head = THC_major_gc_object(curr->contents.l.head);
                break;
            case FUNCTION: 
                curr->contents.f.cont = THC_major_gc_object(curr->contents.f.cont);
                curr->contents.f.args.head = THC_major_gc_object(curr->contents.f.args.head);
                break;
            case NODE: 
                curr->contents.n.next = THC_major_gc_object(curr->contents.n.next);
                curr->contents.n.value = THC_major_gc_object(curr->contents.n.value);
                break;
            default: 
                fprintf(stderr, "Something has gone horribly wrong during major GC! %d\n", curr->type);
                exit(-1);
        }
        heap.cheneyStart++;
    }
    if(heap.max != start_max){
        // heap has been reallocated
        free(heap.to);
        heap.to = malloc(sizeof(THCObject) * heap.max);
    }

    saved_cont_call = heap.from;
    longjmp(empty_stack_state, 1);
}

THCObject * allocHeap(){
    if(heap.size >= heap.max){
        printf("Switching to major GC!\n");
        THC_major_gc();
    }
    return heap.from + heap.size++;
}


THCObject * THC_minor_gc_object(THCObject * obj){
    if(obj == NULL) return NULL;
    if(obj->gcflag != STACK){
        while(obj->gcflag == COPIED)
            obj = heap.from + obj->gc_forward;
        return obj;
    }
    THCObject * newobj = allocHeap();
    *newobj = *obj;
    newobj->gcflag = HEAP;
    obj->gcflag = COPIED;
    obj->gc_forward = newobj - heap.from;
    return newobj;
}

void THC_minor_gc(THCObject* this){
    saved_cont_call = this;
    saved_cont_call = allocHeap();
    *saved_cont_call = *this;
    saved_cont_call->gcflag = HEAP;

    // TODO: fix this. always major gcing because evaluation causes heap objects
    // To point at stack objects
    THC_major_gc();
    heap.cheneyStart = heap.size-1;

    while(heap.cheneyStart < heap.size){
        if(heap.max - heap.size <= 4){
            THC_major_gc();
        }
        THCObject * curr = heap.from + heap.cheneyStart;
        switch(curr->type){
            case ATOMIC: 
                //printf("%d\n", curr->contents.i);
                break;
            case ALGEBRAIC:
                curr->contents.l.head = THC_minor_gc_object(curr->contents.l.head);
                break;
            case FUNCTION: 
                curr->contents.f.cont = THC_minor_gc_object(curr->contents.f.cont);
                curr->contents.f.args.head = THC_minor_gc_object(curr->contents.f.args.head);
                break;
            case NODE: 
                curr->contents.n.next = THC_minor_gc_object(curr->contents.n.next);
                curr->contents.n.value = THC_minor_gc_object(curr->contents.n.value);
                break;
            default:
                fprintf(stderr, "Something has gone horribly wrong during minor GC! %d\n", curr->type);
                exit(-1);
        }
        heap.cheneyStart++;
    }

    longjmp(empty_stack_state, 1);
}
