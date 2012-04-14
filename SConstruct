env = Environment(CCFLAGS='-Wall -Wextra -pedantic -ggdb')
env.ParseConfig('pkg-config --cflags --libs sdl')
env.ParseConfig('pkg-config --cflags --libs cairo')
env.ParseConfig('pkg-config --cflags --libs jack')

env.Program('Main', Glob('src/*.cpp'))
