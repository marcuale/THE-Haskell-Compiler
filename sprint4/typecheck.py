import sys
from thcast import *

ARROW = TypeConstrVar('->', 2)
INT = TypeConstrVar('Int', 0)

DEFAULT_TYPES = {
    '->': DataDef(Type(ARROW, ['', '']), []),
    'Int': DataDef(Type(INT, []), []),
}

DEFAULT_FUNCS = {
    '+': FuncDef(Type(Type(ARROW, INT), Type(Type(ARROW, INT), INT)), ['+', '_', '_'], None), 
    '*': FuncDef(Type(Type(ARROW, INT), Type(Type(ARROW, INT), INT)), ['+', '_', '_'], None), 
}

def is_unknown(v):
    return isinstance(v, TypeConstrVar) and v.name[0].islower()

def substitute(typ, tdict):
    if isinstance(typ, TypeConstrVar): 
        return tdict.get(typ.name, typ)
    else:
        return Type(substitute(typ.constr, tdict), substitute(typ.args, tdict))

def contains(typ, name):
    if typ == name: return True
    if isinstance(typ, TypeConstrVar): return False
    return contains(typ.constr, name) or contains(typ.args, name)

def update_tdict(tdict, name, targ):
    tdict[name] = targ
    for key in tdict:
        if key == name: continue
        tdict[key] = substitute(tdict[key], tdict)

def unify(tdict, a, b):
    if is_unknown(a) or is_unknown(b):
        if is_unknown(b):
            t, u = a, b
        else:
            t, u = b, a
        if t == u: return t
        if contains(t, u): 
            print(f"Recursive type {t} contains {u}")
            sys.exit(-1)
        
        update_tdict(tdict, u.name, t)
        u = substitute(u, tdict)
        return u
    elif isinstance(a, TypeConstrVar) and isinstance(b, TypeConstrVar):
        if a != b:
            print(f"Could not match types {a} with {b}")
            sys.exit(-1)
        return b
    elif isinstance(a, Type) and isinstance(b, Type):
        return Type(unify(tdict, a.constr, b.constr), unify(tdict, a.args, b.args))

    print(f"Failed to match types {a} and {b}")
    sys.exit(-1)

def get_type_arg_count(typ):
    if isinstance(typ, Type) and isinstance(typ.args, list):
        return len(typ.args)
    c = 0
    while isinstance(typ, Type):
        c += 1
        typ = typ.args

    return c

def check_type(typedefs, typescope, typ):
    if isinstance(typ, TypeConstrVar):
        success = typ.name in typedefs or typ.name in typescope
        if not success:
            print(f"Unrecognized type {typ.constr.name}")
            raise Exception
            sys.exit(-1)
        return
    
    success = typ.constr.name in typedefs or typ.constr.name in typescope
    if not success:
        print(f"Unrecognized type {typ.constr}")
        sys.exit(-1)

    if typ.constr.name in typescope:
        # TODO: Do kind checking on type variables
        return

    ref = typedefs[typ.constr.name]
    ref_count = get_type_arg_count(ref.type_constr)
    typ_count = get_type_arg_count(typ)
    success = ref_count == typ_count
    if not success:
        print(f"{ref.type_constr.constr} expected {ref_count} args got {typ_count}")
        sys.exit(-1)
    
    check_type(typedefs, typescope, typ.constr)


def check_data(datadefs, cstrdefs, funcdefs, data):
    args = [x.name for x in data.type_constr.args]
    fin_typ = TypeConstrVar(data.type_constr.constr.name)
    for v in reversed(data.type_constr.args):
        fin_typ = Type(fin_typ, TypeConstrVar(v.name))
    for constr in data.constructors:
        typ = fin_typ
        cstrdefs[constr.constr.name] = (list(reversed(constr.args)), typ)
        for arg in constr.args:
            check_type(datadefs, args, arg)
            typ = Type(Type(TypeConstrVar('->'), arg), typ)
        
        func = FuncDef(typ, [Variable(constr.constr.name)], None)
        funcdefs[constr.constr.name] = func


def get_vars(typ):
    v = set()
    if isinstance(typ, TypeConstrVar):
        if typ.name[0].islower():
            return {typ.name}
    elif isinstance(typ, Type):
        v |= get_vars(typ.args)
        v |= get_vars(typ.constr)
    else:
        print("Something went wrong during function type detection")
        print(type(typ))
        sys.exit(-1)
    
    return v

def is_function_type(f):
    return isinstance(f, Type) and isinstance(f.constr, Type) and f.constr.constr == TypeConstrVar('->')

def unify_function(ftype, vtype, tdict, n):
    if is_unknown(ftype):
        ret_type = TypeConstrVar(f'u-{n}')
        new_ftype = Type(Type(ARROW, vtype), ret_type)
        tdict[ftype.name] = new_ftype
        return ret_type, n+1

    if not is_function_type(ftype):
        print(f'{ftype} is not a function type')
        sys.exit(-1)

    ltype = unify(tdict, ftype.constr.args, vtype)
    rtype = substitute(ftype.args, tdict)
    return rtype, n

def number_type(typ, n):
    if isinstance(typ, Type):
        constr = number_type(typ.constr, n)
        args = number_type(typ.args, n)
        return Type(constr, args)
    elif isinstance(typ, TypeConstrVar):
        if is_unknown(typ):
            return TypeConstrVar(typ.name+f'-{n}')

        return typ
    else:
        print("Invalid type detected")
        print(type(typ))
        raise Exception
        sys.exit(-1)

def fix_type(typ):
    if isinstance(typ, TypeConstrVar):
        if is_unknown(typ):
            return TypeConstrVar('-'+typ.name)
        return typ
    elif isinstance(typ, Type):
        return Type(fix_type(typ.constr), fix_type(typ.args))
    else:
        print("Invalid type detected")
        print(type(typ))
        raise Exception
        sys.exit(-1)

def get_pattern_scope(cstrdefs, tdict, typ, pattern, n):
    if pattern.constr.name not in cstrdefs:
        print("Invalid pattern match constructor")
        sys.exit(-1)
    argtyps, newtyp = cstrdefs[pattern.constr.name]
    if len(pattern.args) != len(argtyps):
        print(f"{pattern.constr.name} expected {len(argtyps)} fields, got {len(pattern.args)}")
        sys.exit(-1)

    typ = unify(tdict, typ, number_type(newtyp, n))
    argtyps = [number_type(t, n) for t in argtyps]
    d = {k.name: v for k, v in zip(pattern.args, argtyps)}
    return d, typ
    
def check_func(datadefs, cstrdefs, funcdefs, func):
    tdict = {}
    if func.type is None:
        func.type, _ = infer_type(tdict, datadefs, cstrdefs, funcdefs, (), func, skip_func=False)
        return
    ftype, _ = infer_type(tdict, datadefs, cstrdefs, funcdefs, {}, func, skip_func=False)
    orig_typ = fix_type(func.type)
    unify(tdict, ftype, orig_typ)
    return

def infer_type(tdict, datadefs, cstrdefs, globs, env, obj, unknowns=0, skip_func=True):
    if isinstance(obj, Literal):
        return INT, unknowns

    elif isinstance(obj, Variable):
        t = globs.get(obj.name, None)
        if t is not None:
            typ, unknowns = infer_type(tdict, datadefs, cstrdefs, globs, env,t, unknowns)
            return number_type(typ, unknowns), unknowns+1

        t = env.get(obj.name, None)
        if t is None:
            print(f'{obj} is not defined')
            sys.exit(-1)
        return t, unknowns

    elif isinstance(obj, FuncDef):
        if obj.type is not None and skip_func: return obj.type, unknowns

        new_env = {}
        upd_env = dict(env)
        arg_c = 0
        for arg in obj.args:
            if arg.name == '_': continue
            if arg.name in new_env:
                print(f"Duplicate argument name {arg.name}")
                sys.exit(-1)

            new_env[arg.name] = TypeConstrVar(f'u-{arg_c}-{unknowns}')
            arg_c += 1

        unknowns += 1
        upd_env.update(new_env)
        body_type, unknowns = infer_type(tdict, datadefs, cstrdefs, globs, upd_env, obj.body, unknowns)
        # update environment outside of function
        for key in upd_env:
            if key in new_env: continue
            env[key] = upd_env[key]
 
        arg_c = 0
        for arg in reversed(obj.args):
            if arg.name == '_': t = TypeConstrVar(f'a{arg_c}')
            else: t = upd_env[arg.name]
            body_type = Type(Type(ARROW, t), body_type)

        
        body_type = substitute(body_type, tdict)
        # TODO: Check body type matches actual function type
        if obj.type is None: obj.type = body_type
        return body_type, unknowns

    elif isinstance(obj, FuncApp):
        func_typ, unknowns = infer_type(tdict, datadefs, cstrdefs, globs, env, obj.func, unknowns)
        valu_typ, unknowns = infer_type(tdict, datadefs, cstrdefs, globs, env, obj.value, unknowns)
        res, unknowns = unify_function(func_typ, valu_typ, tdict, unknowns)
        return substitute(res, tdict), unknowns

    elif isinstance(obj, CaseExpr):
        expr_typ, unknowns = infer_type(tdict, datadefs, cstrdefs, globs, env, obj.expr, unknowns)
        ret_type = None
        expr_unknowns = unknowns
        unknowns += 1
        for case in obj.cases:
            new_scope, expr_typ = get_pattern_scope(cstrdefs, tdict, expr_typ, case.pattern, expr_unknowns)
            old_scope = dict(env)
            old_scope.update(new_scope)
            case_type, unknowns = infer_type(tdict, datadefs, cstrdefs, globs, old_scope, case.body, unknowns)
            expr_typ = substitute(expr_typ, tdict)
            if ret_type is None: 
                ret_type = case_type
            else:
                ret_type = unify(tdict, ret_type, case_type)
        
        return ret_type, unknowns

def typecheck(statements):
    datadefs = dict(DEFAULT_TYPES)
    funcdefs = dict(DEFAULT_FUNCS)
    cstrdefs = {}

    data_list = []
    func_list = []

    for statement in statements:
        if isinstance(statement, DataDef):
            name = statement.type_constr.constr.name
            datadefs[name] = statement
            data_list.append(statement)
        elif isinstance(statement, FuncDef):
            name = statement.name.name
            funcdefs[name] = statement
            func_list.append(statement)
        else:
            print("Something went wrong during parsing\n")
            sys.exit(-1)

    for data in data_list:
        check_data(datadefs, cstrdefs, funcdefs, data)

    for func in func_list:
        check_func(datadefs, cstrdefs, funcdefs, func)

if __name__ == '__main__':
    t = {}
    a = Type(Type(ARROW, TypeConstrVar('a')), INT)
    b = Type(Type(ARROW, INT), TypeConstrVar('b'))
    o = unify(t, a, b)
    print(t)
    print(o)
