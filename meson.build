project(
  'tetris-ai',
  'cpp',
  version: '1.0.0',
  default_options: [
    'cpp_std=gnu++2a',
    'warning_level=3',
    'buildtype=release',
  ],
)

include_dirs = include_directories('.')

buildtype = get_option('buildtype')
if buildtype == 'release'
  add_global_arguments('-O3', '-DNDEBUG', language: 'cpp')
elif buildtype == 'debug'
  add_global_arguments('-O0', '-g', '-DDEBUG', language: 'cpp')
endif

tetris_sources = [
  'src/tetris/grid.cpp',
  'src/tetris/piece.cpp',
]
ml_sources = [
  'src/ml/brain.cpp',
  'src/ml/genetic.cpp',
  'src/ml/utils.cpp',
  'src/ml/population.cpp',
]

train = executable(
  'train',
  files(
    ml_sources,
    tetris_sources,
    'src/train.cpp',
  ),
  include_directories: include_dirs,
)
tetris = executable(
  'tetris',
  files(
    ml_sources,
    tetris_sources,
    'src/terminal.cpp',
  ),
  include_directories: include_dirs,
)
