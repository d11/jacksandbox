env = Environment(CCFLAGS='-Wall -Werror -ggdb')
env.ParseConfig('pkg-config --cflags --libs sdl')
env.ParseConfig('pkg-config --cflags --libs cairo')
env.Program('Main', Glob('Anim/*.cpp'))
