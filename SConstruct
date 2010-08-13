env = Environment()

target = 'bawt'
buildDirectory = 'build'
SConscript('src/SConscript', exports='env target', variant_dir = buildDirectory , duplicate = 0 )

Clean('.', Glob("*~") + Glob('*/*~') + [ buildDirectory ] )
