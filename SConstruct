vars = Variables()
vars.Add(PackageVariable('boost', 'boost installation directory (should contain boost/ and lib/)', 'yes'))
vars.Add('compiler', 'compiler command to use', 'g++')

env = Environment(variables = vars)

if env['boost'] == True:
  dir = '/usr/local/include'
  env['boost'] = dir  
if env['boost']:
  env.Append(CPPPATH='$boost/include')
  env.Append(LIBPATH='$boost/lib')

env.Replace(CXX = '$compiler')

Help(vars.GenerateHelpText(env))

target = 'bawt'
buildDirectory = '.build'
SConscript('src/SConscript', exports='env target', variant_dir = buildDirectory , duplicate = 0 )

Clean('.', Glob("*~") + Glob('*/*~') + [ buildDirectory ] )
