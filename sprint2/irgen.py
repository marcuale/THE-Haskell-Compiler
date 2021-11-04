import sys

import thcast
import thcir

from parser import Parser

def genfuncs(s):
    arity = len(s.args)
    newfunc = thcir.gencont(s.body, [x.name for x in reversed(s.args)])
     
    return (s.name.name, newfunc)

def irgen(statements):
    datadefs = []
    funcdefs = []


    for s in reversed(statements):
        if isinstance(s, thcast.DataDef):
            datadefs.append(thcir.DataDef(s))
        else:
            funcdefs.append(genfuncs(s))


    return datadefs, funcdefs

def main():
    m = Parser()
    m.build()
    parsed = m.parse(open(sys.argv[1]).read())
    out = irgen(parsed)
    print(out)

if __name__ == '__main__':
    main()
