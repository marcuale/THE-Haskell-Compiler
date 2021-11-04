import sys
from thcast import *

ARROW = TypeConstrVar('->')
INT = TypeConstrVar('Int')
INT_T = Type(INT, [])

DEFAULT_TYPES = {
    '->': DataDef(Type(ARROW, ['_', '_']), []),
    'Int': DataDef(Type(INT, []), []),
}

DEFAULT_FUNCS = {
    '+': Type(ARROW, [INT_T, Type(ARROW, [INT_T, INT_T])]), 
    '*': Type(ARROW, [INT_T, Type(ARROW, [INT_T, INT_T])]), 
}


def check_type(typedefs, typescope, typ):
    typ = normalize(typ)        
    if len(typ.args) == 0:
        success = typ.constr.name in typedefs or typ.constr.name in typescope \
                or typ.constr.name[0].islower()
        if not success:
            print(f"Unrecognized type {typ.constr.name}")
            sys.exit(-1)
        return

    success = typ.constr.name in typedefs
    if not success:
        print(f"Unrecognized type {typ.constr}")
        sys.exit(-1)

    ref = typedefs[typ.constr.name]
    success = len(ref.type_constr.args) == len(typ.args)
    if not success:
        print(f"{ref.type_constr.constr} expected {len(ref.type_constr.args)} args got {len(typ.args)}")
        sys.exit(-1)

    for arg in typ.args:
        check_type(typedefs, typescope, arg)



def check_data(datadefs, funcdefs, data):
    args = [x.name for x in data.type_constr.args]
    
    for constr in data.constructors:
        typ = data.type_constr
        for arg in constr.args:
            check_type(datadefs, args, arg)
            typ = Type(TypeConstrVar('->'), [arg, typ])
        
        func = FuncDef(typ, [Variable(constr.constr.name)], None)
        funcdefs[constr.constr.name] = typ


def get_vars(typ):
    v = set()
    if isinstance(typ, TypeConstrVar):
        if typ.name[0].islower():
            return {typ.name}
    elif isinstance(typ, Type):
        for arg in typ.args:
            v |= get_vars(arg)
    else:
        print("Something went wrong during function type detection")
        print(type(typ))
        sys.exit(-1)
    
    return v

def normalize(typ):
    if isinstance(typ, TypeConstrVar) or isinstance(typ, Variable):
        typ = Type(TypeConstrVar(typ.name), [])
    if isinstance(typ.constr, str):
        typ.constr = TypeConstrVar(typ.constr)

    return typ

# needs work, hack works for now
def unify(ltype, rtype, tdict):
    rtype = normalize(rtype)
    ltype = normalize(ltype)
    
    is_var = ltype.constr.name[0].islower()
    varname = ltype.constr.name
    if isinstance(rtype, TypeConstrVar) or isinstance(rtype, Variable):
        rtype = Type(TypeConstrVar(rtype.name), [])

    if is_var and varname in tdict:
        if rtype != tdict[varname]:
            print(f"Could not match type {rtype} with {tdict[varname]}")
        else:
            return tdict[varname]
    elif is_var:
        tdict[varname] = rtype
        return rtype

    is_var = rtype.constr.name[0].islower()
    varname = rtype.constr.name
    if is_var and varname in tdict:
        if ltype != tdict[varname]:
            print(f"Could not match type {ltype} with {ldict[varname]}")
        else:
            return tdict[varname]
    elif is_var:
        tdict[varname] = ltype
        return ltype

    elif ltype.constr.name != rtype.constr.name or len(ltype.args) != len(rtype.args):
        print(f"Could not match type {rtype} with {ltype}")
        sys.exit(-1)
    else:
        args = [unify(l, r, tdict) for l, r in zip(ltype.args, rtype.args)]
        return Type(TypeConstrVar(ltype.constr.name), args)

# needs work, hack works for now
def unify_left(ltype, rtype, tdict):
    rtype = normalize(rtype)
    ltype = normalize(ltype)
    
    is_var = ltype.constr.name[0].islower()
    varname = ltype.constr.name
    if isinstance(rtype, TypeConstrVar) or isinstance(rtype, Variable):
        rtype = Type(TypeConstrVar(rtype.name), [])

    if is_var and varname in tdict:
        if rtype != tdict[varname]:
            print(f"Could not match type {rtype} with {tdict[varname]}")
            sys.exit(-1)
        else:
            return tdict[varname]
    elif is_var:
        tdict[varname] = rtype
        return rtype

    elif ltype.constr.name != rtype.constr.name or len(ltype.args) != len(rtype.args):
        print(f"Could not match type {rtype} with {ltype}")
        sys.exit(-1)
    else:
        args = [unify(l, r, tdict) for l, r in zip(ltype.args, rtype.args)]
        return Type(TypeConstrVar(ltype.constr.name), args)

def substitute(typ, tdict):
    typ = normalize(typ)
    name = typ.constr.name
    if name in tdict: 
        # TODO: support type constructor variables
        return tdict[name]
    else:
        args = [substitute(t, tdict) for t in typ.args]
        return Type(typ.constr, args)

def unify_function(ftype, vtype, tdict):
    ltype = unify(ftype.args[0], vtype, tdict)
    return substitute(ftype.args[1], tdict)

def number_type(typ, n):
    typ = normalize(typ)
    if isinstance(typ, Type):
        name = typ.constr.name 
        if name[0].islower(): name += f'-{n}'
        args = [number_type(t, n) for t in typ.args]
        return Type(TypeConstrVar(name), args)
    else:
        print("Invalid type detected")
        print(type(typ))
        sys.exit(-1)

def fix_type(typ):
    typ = normalize(typ)
    if isinstance(typ, Type):
        name = typ.constr.name 
        if name[0].islower(): name = '-'+name
        args = [fix_type(t) for t in typ.args]
        return Type(TypeConstrVar(name), args)
    else:
        print("Invalid type detected")
        print(type(typ))
        sys.exit(-1)

def get_pattern_scope(datadefs, typ, pattern):
    if typ.constr.name not in datadefs:
        print("Expression of invalid type")
        sys.exit(-1)
    
    data = datadefs[typ.constr.name]

    if len(data.type_constr.args) != len(typ.args):
        print("Invalid type")
        sys.exit(-1)

    subdict = {k.name:normalize(v) for k,v in zip(data.type_constr.args, typ.args)}
    constr = None
    for c in data.constructors:
        if c.constr.name == pattern.constr.name:
            constr = c
            break

    if constr is None:
        print(f"{pattern} is not constructor of {typ}")
        sys.exit(-1)

    if len(pattern.args) != len(constr.args):
        print(f"{constr.constr} expected {len(constr.args)} fields, got {len(pattern.args)}")
        sys.exit(-1)
    
    d = {k.name:substitute(v, subdict) for k, v in zip(pattern.args, reversed(constr.args))}
    return d
    
def get_body_type(datadefs, funcdefs, scope, body, subs, type_int=0):
    if isinstance(body, Variable):
        name = body.name
        if name in funcdefs:
            return number_type(funcdefs[name], type_int), type_int + 1
        elif name in scope:
            #return number_type(scope[name], type_int), type_int + 1
            return scope[name], type_int
        else:
            print(f"Name not found {name}")
            sys.exit(-1)
    elif isinstance(body, Literal):
        return INT_T, type_int
    elif isinstance(body, FuncApp):
        # TODO: update the scope with any tightend types
        func_typ, type_int = get_body_type(datadefs, funcdefs, scope, body.func, subs, type_int)
        valu_typ, type_int = get_body_type(datadefs, funcdefs, scope, body.value, subs, type_int)
        res = unify_function(func_typ, valu_typ, subs)
        return res, type_int

    elif isinstance(body, CaseExpr):
        # TODO: carry up any restrictions made on the expression type
        expr_typ, type_int = get_body_type(datadefs, funcdefs, scope, body.expr, subs, type_int)
        ret_type = None
        for case in body.cases:
            new_scope = get_pattern_scope(datadefs, expr_typ, case.pattern)
            old_scope = dict(scope)
            old_scope.update(new_scope)
            case_type, type_int = get_body_type(datadefs, funcdefs, old_scope, case.body, subs, type_int)
            expr_typ = substitute(expr_typ, subs)
            if ret_type is None: 
                ret_type = case_type
            else:
                ret_type = unify(ret_type, case_type, {})
        
        return ret_type, type_int
    else:
        print("Something went wrong during type checking")
        print(type(body))


def check_func(datadefs, funcdefs, func):
    args = {}
    typ = func.type
    typvars = get_vars(typ)
    check_type(datadefs, list(typvars), typ)
    curr_func_typ = fix_type(typ)
    for arg in func.args:
        if curr_func_typ.constr.name != '->':
            print("More args than type would suggest.")
            sys.exit(-1)

        if arg.name == '_': continue
        if arg.name in args:
            print(f"Duplicate argument name {arg.name}")
            sys.exit(-1)

        args[arg.name] = curr_func_typ.args[0]
        curr_func_typ = curr_func_typ.args[1]

    targ_typ = curr_func_typ
    body_typ, type_int = get_body_type(datadefs, funcdefs, args, func.body, {})
    unify_left(body_typ, targ_typ, {})

def typecheck(statements):
    datadefs = dict(DEFAULT_TYPES)
    funcdefs = dict(DEFAULT_FUNCS)

    data_list = []
    func_list = []

    for statement in statements:
        if isinstance(statement, DataDef):
            name = statement.type_constr.constr.name
            datadefs[name] = statement
            data_list.append(statement)
        elif isinstance(statement, FuncDef):
            name = statement.name.name
            funcdefs[name] = statement.type
            func_list.append(statement)
        else:
            print("Something went wrong during parsing\n")
            sys.exit(-1)

    for data in data_list:
        check_data(datadefs, funcdefs, data)

    for func in func_list:
        check_func(datadefs, funcdefs, func)

