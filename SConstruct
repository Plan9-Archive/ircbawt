AddOption('--with-boost',
          dest='with-boost',
          type='string',
          nargs=1,
          action='store',
          metavar='DIR',
          help='path to boost installation')
          
AddOption('--with-cxx',
          dest='with-cxx',
          type='string',
          nargs=1,
          action='store',
          metavar='COMMAND',
          help='program used to invoke the C++ compiler')

env = Environment(BOOST_PATH = GetOption('with-boost'), CXX_COMMAND = GetOption('with-cxx'))

target = 'bawt'
buildDirectory = 'build'
SConscript('src/SConscript', exports='env target', variant_dir = buildDirectory , duplicate = 0 )

Clean('.', Glob("*~") + Glob('*/*~') + [ buildDirectory ] )
