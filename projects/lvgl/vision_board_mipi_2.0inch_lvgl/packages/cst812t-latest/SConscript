from building import *

src = []
cwd     = GetCurrentDir()
path    = [cwd + '/inc']

if GetDepend('PKG_USING_CST812T'):
    src += ['cst812t.c']

if GetDepend('PKG_USING_CST812T_EXAMPLE'):
    src += Glob('example/drv_cst812t.c')

group = DefineGroup('cst812t', src, depend = ['PKG_USING_CST812T'], CPPPATH = path)

Return('group')
