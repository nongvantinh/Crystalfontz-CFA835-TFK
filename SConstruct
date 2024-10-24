#!/usr/bin/env python

import os
import sys
import subprocess
import struct
from SCons.Script import Environment, Variables, Help, ARGUMENTS, EnumVariable

program_name = 'build/lcd_display.out'
root_dir = os.path.abspath('.')

opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable(
    'target',
    'Compilation target',
    'debug',
    allowed_values=('debug', 'release'),
    ignorecase=2
))

env = Environment()
opts.Update(env)
Help(opts.GenerateHelpText(env))

if env['target'] == 'debug':
    env.Append(CXXFLAGS=['-g', '-O0', '-Wall', '-Wextra', '-fPIC'])
    env.Append(CCFLAGS=['-g', '-O0', '-Wall', '-Wextra', '-fPIC'])
elif env['target'] == 'release':
    env.Append(CXXFLAGS=['-O2', '-Wall', '-Wextra', '-flto'])
    env.Append(CCFLAGS=['-O2', '-Wall', '-Wextra', '-flto'])
    env.Append(LINKFLAGS=['-flto'])

env['CXXFLAGS'] = ['-std=c++17']
env['LIBPATH'] = ['lib']
env['LIBS'] = ['m', 'boost_system', 'boost_filesystem', 'boost_thread', 'pthread']

SOURCE_EXTENSION = '*.cpp'
sources = [env.Glob(SOURCE_EXTENSION)]
root_directories = [
    '.', 
    'src', 
    'src/components', 
    'src/generated', 
    'src/configs', 
    'src/models', 
    'src/utils', 
    'src/hardwares',
    'src/hardwares/crystalfontz',
    'src/views',
    'tests',
    'lib',
]

env['CPPPATH'] = root_directories

for root_dir in root_directories:
    pattern = os.path.join(root_dir, '**', SOURCE_EXTENSION)
    sources += env.Glob(pattern)

print(sources)
env.Program(target=program_name, source=sources)
