api_headers = [
	'export.h',
	'input.h',
	'keysyms.h',
	'loader.h',
	'pasteboard.h',
	'renderer-backend-egl.h',
	'renderer-host.h',
	'view-backend.h',
	'wpe-egl.h',
	'wpe.h',

	# Generated API headers.
	configure_file(
		input: 'version.h.in',
		output: 'version.h',
		configuration: version_info,
	),
	configure_file(
		input: 'version-deprecated.h.in',
		output: 'version-deprecated.h',
		configuration: version_info,
	),
]
install_headers(api_headers,
	subdir: join_paths('wpe-' + api_version, 'wpe'),
)

