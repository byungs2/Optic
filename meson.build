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

add_project_arguments('-g', language : 'cpp')
add_project_arguments('-g', language : 'c')

cv4 = dependency('opencv4')
v4l2 = dependency('libv4l2')
rknnapi = dependency('rknn_api')
rga = dependency('rga')
cvm_inc = include_directories('./opencv-mobile-4.10.0-luckfox-pico/include/opencv4')
cvm_core = cxx.find_library('opencv_core', dirs: '/home/byunghun/workspace/luckfox_app/opencv-mobile-4.10.0-luckfox-pico/lib')
cvm_img = cxx.find_library('opencv_imgproc', dirs: '/home/byunghun/workspace/luckfox_app/opencv-mobile-4.10.0-luckfox-pico/lib')
cvm_high = cxx.find_library('opencv_highgui', dirs: '/home/byunghun/workspace/luckfox_app/opencv-mobile-4.10.0-luckfox-pico/lib')

optic = executable('optic',
  'main.cc',
  dependencies: [
    cv4,
    v4l2,
    rknnapi,
    rga,
    cvm_core,
    cvm_img,
    cvm_high,
  ],
  link_with: [
  ],
  include_directories: cvm_inc,
)
