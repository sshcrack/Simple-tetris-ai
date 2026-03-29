project(
  'tetris-ai',
  'cpp',
  version: '1.0.0',
  default_options: [
    'cpp_std=gnu++2a',
    'warning_level=2',
    'buildtype=release',
    'optimization=3'
  ],
)

include_dirs = include_directories('.')

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
bench = executable(
  'bench',
  files(
    ml_sources,
    tetris_sources,
    'src/benchmark.cpp',
  ),
  include_directories: include_dirs,
)
