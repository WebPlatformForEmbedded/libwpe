# Implementing Backends

## Installation

Backend implementations must be installed to a specific directory. The
location is determined at build time to be `<prefix>/lib/wpe-<apiversion>`,
with `<prefix>` being the base directory where *libwpe* is installed, and
`<apiversion>` the public API version.

The `pkg-config` tool can be used to query the `backendsdir` variable,
which contains the location where backend implementations will be searched
for:

```sh
pkg-config wpe-1.0 --variable=backendsdir
```

For example, the following Make snippet will install a backend in the
correct location:

```make
install: libMyBackend.so
	install -Dm755 -t "$$(pkg-config wpe-1.0 --variable=backendsdir)" $<
```
