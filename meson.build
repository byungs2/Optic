project('optic', 'c', 'cpp',
  version: '1.0',
  license: ['LGPL-2.1'],
  meson_version: '>=0.50.0',
  default_options: [
    'werror=true',
    'warning_level=2',
    'c_std=gnu89',
  ]
)

is_native = false

cc = meson.get_compiler('c', native: is_native)
cxx = meson.get_compiler('cpp', native: is_native)

warning_flags = [
  '-Wmissing-braces',
  '-Wmaybe-uninitialized',
  '-Wwrite-strings',
  '-Wformat',
  '-Wformat-nonliteral',
  '-Wformat-security',
  '-Winit-self',
  '-Waddress',
  '-Wno-multichar',
  '-Wvla',
  '-Wpointer-arith'
]

warning_c_flags = [
  '-Wmissing-declarations',
  '-Wmissing-include-dirs',
  '-Wmissing-prototypes',
  '-Wnested-externs',
  '-Waggregate-return',
  '-Wold-style-definition',
  '-Wdeclaration-after-statement'
]

# Setup warning flags for c and cpp
foreach extra_arg : warning_flags
  if cc.has_argument (extra_arg)
    add_project_arguments([extra_arg], language: 'c')
  endif
  if cxx.has_argument (extra_arg)
    add_project_arguments([extra_arg], language: 'cpp')
  endif
endforeach

foreach extra_arg : warning_c_flags
  if cc.has_argument (extra_arg)
    add_project_arguments([extra_arg], language: 'c')
  endif
endforeach

cv4 = dependency('opencv4')
v4l2 = dependency('libv4l2')

optic = executable('optic',
  'main.cc',
  dependencies: [
    cv4,
    v4l2,
  ],
  link_with: [
  ],
)
