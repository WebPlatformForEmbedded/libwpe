project('libwpe', 'cpp', 'c',
	meson_version: '>=0.49',
	default_options: [
		'b_ndebug=if-release',
		'c_std=c99',
		'cpp_std=c++11',
	],
	license: 'BSD-2-Clause',
	version: run_command(join_paths('scripts', 'version.py')).stdout().strip(),
)

# This refers to the API level provided. This is modified only with major,
# breaking changes, which need modifications in programs using the library
# before they can be compiled again.
api_version = '1.0'

# Before making a release, the LT_VERSION string should be modified.
# The string is of the form [C, R, A].
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to [C+1, 0, A+1].
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to [C+1, 0, 0]
# - If the interface is the same as the previous version, use [C, R+1, A].
soversion = [5, 2, 4]

# Mangle [C, R, A] into an actual usable *soversion*.
soversion_major = soversion[0] - soversion[2]  # Current-Age
soversion_minor = soversion[2]  # Age
soversion_micro = soversion[1]  # Revision
soversion = '@0@.@1@.@2@'.format(soversion_major, soversion_minor, soversion_micro)

add_project_arguments('-DWPE_COMPILATION=1', language: ['c', 'cpp'])

# Switch to the 'cpp_rtti=false' default option when updating to Meson 0.53 or newer, see
# https://mesonbuild.com/FAQ.html#how-do-i-disable-exceptions-and-rtti-in-my-c-project
add_project_arguments(
	meson.get_compiler('cpp').get_supported_arguments(['-fno-rtti', '-fno-exceptions']),
	language: 'cpp'
)

default_backend = get_option('default-backend').strip()
if default_backend != ''
	add_project_arguments('-DWPE_BACKEND="@0@"'.format(default_backend), language: ['c', 'cpp'])
endif

dependencies = [
	dependency('xkbcommon'),
]

cc = meson.get_compiler('c')
egl_dep = dependency('egl', required: false)
if egl_dep.found()
	dependencies += egl_dep.partial_dependency(
		compile_args: true,
		includes: true,
	)
else
	assert(cc.has_header('EGL/eglplatform.h'),
		'Required header <EGL/eglplatform.h> not found'
	)
endif

if not cc.has_function('dlopen')
	dependencies += cc.find_library('dl')
endif

libwpe_sources = files(
	'src/input.c',
	'src/key-unicode.c',
	'src/loader.c',
	'src/pasteboard.c',
	'src/pasteboard-generic.cpp',
	'src/pasteboard-noop.cpp',
	'src/renderer-backend-egl.c',
	'src/renderer-host.c',
	'src/version.c',
	'src/view-backend.c',
)

libwpe = library('wpe-' + api_version,
	libwpe_sources,
	install: true,
	dependencies: dependencies,
	version: soversion,
	soversion: soversion_major,
	gnu_symbol_visibility: 'hidden',
	include_directories: include_directories('include'),
)

subdir('include')
import('pkgconfig').generate(
	description: 'The wpe library',
	name: 'wpe-' + api_version,
	subdirs: 'wpe-' + api_version,
	libraries: libwpe,
	version: meson.project_version(),
)

libwpe_dep = declare_dependency(link_with : libwpe,
  include_directories : include_directories('include'),
  dependencies : dependencies,
)

if get_option('build-docs')
	gi_docgen_exe = find_program('gi-docgen', 'gi-docgen.py')

	libwpe_gir = import('gnome').generate_gir(
		libwpe,
		install: false,
		nsversion: api_version,
		export_packages: ['wpe-' + api_version],
		namespace: 'wpe',
		identifier_prefix: 'wpe_',
		symbol_prefix: 'wpe',
		header: 'wpe/wpe.h',
		sources: libwpe_sources + api_headers,
	)

	libwpe_doc_config = configure_file(
		configuration: {'VERSION': meson.project_version()},
		input: 'docs/wpe.toml.in',
		output: 'wpe.toml',
	)

	libwpe_doc = custom_target('docs',
		build_by_default: true,
		install: false,
		output: 'docs',
		command: [gi_docgen_exe, 'generate',
			'--quiet',
			'--fatal-warnings',
			'--no-namespace-dir',
			'--config', libwpe_doc_config,
			'--content-dir', join_paths(meson.current_source_dir(), 'docs'),
			'--output-dir', '@OUTPUT@',
			libwpe_gir[0],
		],
	)
endif
