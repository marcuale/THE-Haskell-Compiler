import sys

from thcir import *
from irgen import *

DEFAULT_SYMBOLS = {
    '+': 'Default_add',
    '*': 'Default_mult',
}

headers = """ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"
"""

footers = """

int main(){
    initDefaults();
    initSelf();

    THCObject new_main;
    constructContinuation(&new_main, Func_main_obj, THCprintResult);
    THC_driver_loop(&new_main);
}
"""

def funcname(name):
    return "THCFunc" + escape(str(name))

def funcheader(name):
    return f"void {funcname(name)}(THCObject * cont, THCList * args)"

def contname(name, i):
    return "THCFunc" + name + str(i)

def contheader(name, i):
    return f"void {contname(name, i)}(THCObject * cont, THCList * args)"

def codegen(decls, funcs):
    enums = ""
    declrs = ""
    funcdefs = ""
    top_level_vars = ""
    init = ""
    
    enum_table = {}
    symbol_table = {}
    symbol_table.update(DEFAULT_SYMBOLS)
    for decl in decls:
        ne, nfd, nf, ni = decl.gencode(symbol_table, enum_table)
        enums += ne
        declrs += nfd
        funcdefs += nf
        init += ni

    for name, _ in funcs:
        symbol_table[name] = f'Func_{escape(name)}'
        declrs += f'THCObject * {symbol_table[name]}_obj;\n'

    for name, s in funcs:
        cname = symbol_table[name]
        if isinstance(s, Variable):
            init += s.do_assignment(symbol_table, cname+'_obj');
        elif isinstance(s, Literal):
            init += f'{cname}_obj = malloc(sizeof(THCObject));\n'
            init += s.do_assignment(symbol_table, cname+'_obj');
        else:
            nd, nfd, ni, _, entry = s.gencode(symbol_table, enum_table, cname, 0)
            declrs += nd
            funcdefs += nfd
            init += ni
            init += f'{symbol_table[name]}_obj = {entry}_obj;'

    init = "void initSelf() {\n" + init + "\n}"
    

    return headers + enums + declrs + funcdefs + init + footers

def main():
    m = Parser()
    m.build()
    parsed = m.parse(open(sys.argv[1]).read())
    out = irgen(parsed)
    code = codegen(*out)

    print(code)

if __name__ == '__main__':
    main()
