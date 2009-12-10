env = Environment(CCFLAGS='-Wall -Wextra -pedantic -ggdb')
env.ParseConfig('pkg-config --cflags --libs sdl')
env.ParseConfig('pkg-config --cflags --libs cairo')
env.Program('Main', Glob('Anim/*.cpp'))
