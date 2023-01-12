project('pr_first', 'c', 'cpp',
  version : '0.1',
  default_option : [
    'werror=true',
    'warning_level=2',
    'c_std=gnu89',
    'cpp_std=c++14'
  ]
)

compile_flags = [
  '-Wno-unused-parameter',
  '-D_GLIBCXX_USE_CXX11_ABI=1',
  '-Wno-unused-but-set-variable',
  '-Wno-int-conversion',
  '-Wno-unused-variable',
  '-Wno-comment'
]

cc = meson.get_compiler('cpp')
c = meson.get_compiler('c')

foreach arg : compile_flags
  if cc.has_argument (arg)
    add_project_arguments([arg], language: 'cpp')
  endif
  if c.has_argument (arg)
    add_project_arguments([arg], language: 'c')
  endif
endforeach

glib = dependency('glib-2.0')
gobject = dependency('gobject-2.0')

pr_first = executable('pr_first',
  'main.c',
  'optic_collision_object.c',
  'optic_object.c',
  'optic_tensor.c',
  dependencies : [
    glib,
    gobject
    ],
  #link_with : [],
  #install : false,
  #install_dir : '/home/byunghun/workspace/gobject_tester',
  link_args : '-Wl,--no-as-needed,-lm',
  c_args : ['-mavx', '-O3']
  #c_args : ['-mavx']
  )

test('basic', pr_first)
