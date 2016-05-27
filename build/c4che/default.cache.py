AR = '/usr/bin/ar'
ARFLAGS = 'rcs'
CCFLAGS_MACBUNDLE = ['-fPIC']
CC_VERSION = ('4', '6', '3')
COMPILER_CXX = 'g++'
CPP = '/usr/bin/cpp'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/g++']
CXXDEFINES = ['_DEBUG']
CXXDEFINES_ST = '-D%s'
CXXFLAGS = ['-O0', '-ggdb', '-g3']
CXXLNK_SRC_F = ''
CXXLNK_TGT_F = ['-o', '']
CXX_NAME = 'gcc'
CXX_SRC_F = ''
CXX_TGT_F = ['-c', '-o', '']
FULLSTATIC_MARKER = '-static'
LIBPATH_ST = '-L%s'
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINK_CXX = ['/usr/bin/g++']
NS3_ACTIVE_VARIANT = 'debug'
NS3_BUILDDIR = '/home/ubuntu/College/rapidnet-comp/build'
NS3_OPTIONAL_FEATURES = []
PKG_CONFIG = '/usr/bin/pkg-config'
PREFIX = '/usr/local'
RANLIB = '/usr/bin/ranlib'
RANLIBFLAGS = ''
RPATH_ST = '-Wl,-rpath,%s'
SHLIB_MARKER = '-Wl,-Bdynamic'
SONAME_ST = '-Wl,-h,%s'
STATICLIBPATH_ST = '-L%s'
STATICLIB_MARKER = '-Wl,-Bstatic'
STATICLIB_ST = '-l%s'
macbundle_PATTERN = '%s.bundle'
program_PATTERN = '%s'
shlib_CXXFLAGS = ['-fPIC', '-DPIC']
shlib_LINKFLAGS = ['-shared']
shlib_PATTERN = 'lib%s.so'
staticlib_LINKFLAGS = ['-Wl,-Bstatic']
staticlib_PATTERN = 'lib%s.a'
