from parser import Parser
from irgen import irgen
from codegen import codegen

import sys

m = Parser()
m.build()
parsed = m.parse(open(sys.argv[1]).read())
out = irgen(parsed)
code = codegen(*out)
open(sys.argv[1]+'.c', 'w').write(code)
