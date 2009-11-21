
env = Environment(CCFLAGS='-Wall -Werror -ggdb')

env.ParseConfig('pkg-config --cflags --libs sdl')

env.Program('Main', Glob('Anim/*.cpp'))
