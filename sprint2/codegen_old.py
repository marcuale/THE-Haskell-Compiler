import sys

from parser import Parser
from thcast import *

constr_enums = {}

funcs = {}

headers = """ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"


"""

footers = """

int main(){
    initDefaults();
    initSelf();

    THCObject new_main;
    constructContinuation(&new_main, THCFuncmainobj, THCprintResult);
    THC_driver_loop(&new_main);
}
"""

def escape(name):
    # TODO: make this actually work
    return name.replace(':', 'colon').replace('+', 'add')

def funcname(name):
    return "THCFunc" + escape(str(name))

def funcheader(name):
    return f"void {funcname(name)}(THCObject * cont, THCList * args)"

def contname(name, i):
    return "THCFunc" + name + str(i)

def contheader(name, i):
    return f"void {contname(name, i)}(THCObject * cont, THCList * args)"


def genenum(s):
    name = s.type_constr.constr.name
    enum = f"enum THCType{name}Constrs"+ "{\n"

    funcdefs = ""
    funcs = ""
    init = ""

    for constr in s.constructors:
        constrname = constr.constr.name
        if constrname in constr_enums:
            raise NameError(f"Duplicate constructors named {constrname}")

        constr_enums[constrname] = f"THCConstr{name}{constrname}"
        enum += f" {constr_enums[constrname]},\n"

        arity = len(constr.args);
        funcdefs += f"THCObject * {funcname(constrname)}obj;\n"
        
        if arity == 0:
            init += f"""
            {funcname(constrname)}obj = malloc(sizeof(THCObject));
            constructInt({funcname(constrname)}obj, 0);
            {funcname(constrname)}obj->constructor = {constr_enums[constrname]};
            {funcname(constrname)}obj->type = ALGEBRAIC;
            {funcname(constrname)}obj->gcflag = HEAP;
            {funcname(constrname)}obj->contents.l.head = NULL;
            {funcname(constrname)}obj->contents.l.size = 0;\n
            """
            continue

        funcs += f"{funcheader(constrname)}" + "{" + f"""
            if(!checkStackSize()) THC_minor_gc({funcname(constrname)}, cont, args);
            THCObject object;
            constructInt(&object, 0);
            object.constructor = {constr_enums[constrname]};
            object.contents.l.head = args->head;
            object.contents.l.size = args->size;
            object.type = ALGEBRAIC;
            callCont(cont, &object);""" + "}\n"

        init += f"{funcname(constrname)}obj = constructFunction({funcname(constrname)}, {arity});\n" 


    enum += "};\n"
    return enum, funcdefs, funcs, init


def get_variable(args, varname, name=None):
    if name is None: name = escape(varname)
    decl = f'THCObject * {name} = '
    for n, arg in enumerate(args):
        if isinstance(arg, list) and varname in arg:
            par = f'THCObject * {name}p = indexList(args, {n});\n'
            pos = arg.index(varname)
            decl += f'indexList(&({name}p->contents.l), {pos});\n'
            return par + decl
        elif isinstance(arg, str) and varname == arg:
            pos = args.index(varname)
            return decl + f'indexList(args, {pos});\n'
    else:
        # variable is global
        return decl + f'THCFunc{escape(varname)}obj;\n'

def gencont(app, args, name, num, nextcont=None):
    defs = f'THCObject * {name}cont{num}obj;\n'
    init = f'{name}cont{num}obj = constructFunction({name}cont{num}, {len(args)});\n'
    funcs = ''
    header = f'void {name}cont{num}(THCObject * cont, THCList * args)' + '{\n'
    header += f"if(!checkStackSize()) THC_minor_gc({name}cont{num}, cont, args);\n"

    if nextcont is None:
        nextcontcode = 'THCObject * next_cont = cont;'
    else:
        nextcontcode = f"""
            THCObject next_cont_obj;
            constructContinuation(&next_cont_obj, {nextcont}, cont);
            THCObject * next_cont = &next_cont_obj;
            setArgs(&next_cont_obj, args);
            """

    if isinstance(app, Variable): 
        varname = app.name
        funcs =  header + f"""
            // {str(app)}
            {nextcontcode}
            {get_variable(args, varname, 'v0')}
            callCont(next_cont, v0);
        """ + "}\n"
        
        return defs, funcs, init, num+1, f'{name}cont{num}obj'
    
    elif isinstance(app, Literal):
        i = int(app.value)
        funcs = header + f"""
            // {str(app)}
            {nextcontcode}
            THCObject object;
            constructInt(&object, {i});
            callCont(cont, &object);""" + "}\n"
        
        return defs, funcs, init, num+1, f'{name}cont{num}obj'

    elif isinstance(app, CaseExpr): 
        currnum = num
        init = f'{name}cont{num}obj = constructFunction({name}cont{num}, {len(args)+1});\n'
        nd, nf, ni, num, entry = gencont(app.expr, args, name, num+1, nextcont=f'{name}cont{num}obj')
        defs += nd
        funcs += nf
        init += ni
        cases = []
        for case in app.cases:
            enum = constr_enums[case.pattern.constr.name]
            scope_args = [x.name for x in reversed(case.pattern.args)]
            new_args = [scope_args] + args
            
            nd, nf, ni, num, nxt = gencont(case.body, new_args, name, num, nextcont=nextcont)
            defs += nd
            funcs += nf
            init += ni
            
            code = f"""
                case {enum}:
                    constructContinuation(&case_cont, {nxt}, cont);
                    break;
            """
            cases.append(code)


        funcs += header + f"""
            // {str(app)}
            THCObject * arg = args->head->contents.n.value;
            THCObject case_cont;
            switch(arg->constructor){{
                {''.join(cases)}\n
                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `{name.lstrip('THCFunc')}'.\\n");
                    printf("%d\\n", arg->constructor);
                    int *x=NULL;
                    printf("%d\\n", *x);
                    exit(-1);
            }}
            setArgs(&case_cont, args);
            callFunction(&case_cont);
        """ + "}\n"
        return defs, funcs, init, num, entry
    elif isinstance(app, FuncApp):
        init = f'{name}cont{num}obj = constructFunction({name}cont{num}, {len(args)+1});\n'
        nd, nf, ni, num, entry = gencont(app.func, args, name, num+1, nextcont=f'{name}cont{num}obj')
        defs += nd
        funcs += nf
        init += ni
        nd, nf, ni, num, nxt = gencont(app.value, args, name, num)
        defs += nd
        funcs += nf
        init += ni

        funcs += header + f"""
            // {str(app)}
            THCObject * func = args->head->contents.n.value;
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            {nextcontcode}
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            THCObject argcont;
            constructContinuation(&argcont, {nxt}, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
        """ + "}\n"

        return defs, funcs, init, num, entry

def genfuncs(s):
    arity = len(s.args)

    funcdefs = funcs = init = ''
    funcdefs, funcs, init, _, entry = gencont(s.body, [x.name for x in reversed(s.args)], funcname(s.name), 0)

    funcdefs += f"THCObject * {funcname(s.name)}obj;\n"
    init += f"{funcname(s.name)}obj = {entry};\n"
    
    return funcdefs, funcs, init

def codegen(statements):
    enums = ""
    funcdefs = ""
    funcs = ""
    init = ""

    for s in reversed(statements):
        if isinstance(s, DataDef):
            ne, nfd, nf, ni = genenum(s)
            enums += ne
            funcdefs += nfd
            funcs += nf
            init += ni
        else:
            new_fds, new_fs, new_init = genfuncs(s)
            funcdefs += new_fds
            funcs += new_fs
            init += new_init


    init = "void initSelf() {\n" + init + "\n}"
    

    return headers + enums + funcdefs + funcs + init + footers

def main():
    m = Parser()
    m.build()
    parsed = m.parse(open(sys.argv[1]).read())
    out = codegen(parsed)
    print(out)

if __name__ == '__main__':
    main()
