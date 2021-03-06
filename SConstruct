#
# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

COMMON_CFLAGS = ['-Wall', '-Werror']
COMMON_CXXFLAGS = ['-Wall', '-Werror', '-fno-exceptions']

configs = {
    'debug': {
        'CFLAGS': COMMON_CFLAGS + ['-g'],
        'CXXFLAGS': COMMON_CXXFLAGS + ['-g']
    },

    'release': {
        'CFLAGS': COMMON_CFLAGS + ['-O2'],
        'CXXFLAGS': COMMON_CXXFLAGS + ['-O2']
    },
}

toolsets = {
    'gcc': {
        'CC': 'gcc',
        'CXX': 'g++',
    },

    'clang': {
        'CC': 'clang',
        'CXX': 'clang++',
    },
}

mode = ARGUMENTS.get('mode', 'release')
toolset = ARGUMENTS.get('toolset', 'gcc')

if not mode in configs:
    print 'ERROR: Unknown building mode:', mode
    Exit(1)

if not toolset in toolsets:
    print 'ERROR: Unknown toolset:', toolset
    Exit(1)

build_config = configs[mode]
build_toolset = toolsets[toolset]

print '===> BUILDING IN', mode.upper(), 'MODE ...'

import os

env = Environment(CC=build_toolset['CC'],
                  CXX=build_toolset['CXX'],
                  CFLAGS=build_config['CFLAGS'],
                  CXXFLAGS=build_config['CXXFLAGS'],
                  CPPPATH=['.', 'include'],
                  ENV={'PATH': os.environ['PATH']})

env.ParseConfig('llvm-config --cxxflags --ldflags --libs support')

env.Program('rsloader',
            source=['lib/ELFHeader.cpp',
                    'lib/ELFSectionHeader.cpp',
                    'lib/ELFSymbol.cpp',
                    'lib/ELFTypes.cpp',
                    'lib/MemChunk.cpp',
                    'lib/StubLayout.cpp',
                    'utils/helper.cpp',
                    'utils/raw_ostream.cpp',
                    'utils/rsl_assert.cpp',
                    'main.cpp'])
