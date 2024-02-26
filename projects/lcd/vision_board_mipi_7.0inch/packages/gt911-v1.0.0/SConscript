from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add gt911 src files.
if GetDepend('PKG_USING_GT911'):
    src += Glob('src/gt911.c')

if GetDepend('PKG_USING_GT911_SAMPLE'):
    src += Glob('samples/gt911_sample.c')

# add gt911 include path.
path  = [cwd + '/inc']

# add src and include to group.
group = DefineGroup('gt911', src, depend = ['PKG_USING_GT911'], CPPPATH = path)

Return('group')
