import sys
sys.path.append(@@SCRIPTSDIR@@)
sys.path.append('@@PYTHONDIR@@/DLLs')
sys.path.append('@@PYTHONDIR@@/lib')
sys.path.append('@@PYTHONDIR@@')
sys.path.append('@@PYTHONDIR@@/lib/site-packages')
sys.path.append('@@MORSEROOT@@/Lib/site-packages')
exec(compile(open(@@BUILDERSCRIPT@@).read(), @@BUILDERSCRIPT@@, 'exec'), globals (), locals ())
import os
