# kode54 Input Plugins

Three input plugins from [fooyin-kode54-plugins](https://github.com/kode54/fooyin-kode54-plugins)
are built into this tree rather than built out of tree against an installed
fooyin:

| Directory | Target | Formats |
|---|---|---|
| `src/plugins/midi` | `fyplugin_midiinput` | MIDI, via SpessaSynth |
| `src/plugins/vgmstream` | `fyplugin_vgmstreaminput` | ~700 game audio formats, via vgmstream |
| `src/plugins/xsf` | `fyplugin_xsfinput` | PSF, PSF2, SSF, DSF, QSF, GSF, USF, 2SF, NCSF, SNSF |

They are registered in `src/plugins/CMakeLists.txt` like any other plugin, so
`PLUGIN_SELECTION` can include or exclude them by directory name (`midi`,
`vgmstream`, `xsf`).

## What changed when embedding them

- **Include paths.** Out-of-tree plugins include `<fooyin/core/...>`; in-tree
  ones use `<core/...>`, which is what `create_fooyin_library` puts on the
  include path.
- **Plugin macro.** `create_fooyin_plugin` became `create_fooyin_plugin_internal`,
  which adds the install rules and output directory in-tree plugins need.
- **Settings providers.** This tree's `PluginSettingsProvider` overrides a
  protected `createSettings()` returning `QDialog*`; the base class owns the
  single-instance behaviour. Upstream overrode `showSettings()`.
- **xsf emulator libraries.** `psflib`, `highly_experimental`,
  `highly_theoretical`, `highly_quixotic`, `lazyusf2`, `vio2sf`, `sseqplayer`
  and `snes9x` were git submodules upstream. They are checked in as plain
  sources under `src/plugins/xsf/`, with a local `.gitignore` that undoes the
  top-level `Makefile*` and `*.o` rules so nothing is dropped.

## Third-party libraries

Fetched the way projectM already is, through `3rdparty/*.cmake`, each behind an
option that defaults to `ON`:

| Option | Module | Source |
|---|---|---|
| `FETCH_SPESSASYNTH` | `3rdparty/spessasynth.cmake` | `kode54/spessasynth_core_c` @ `99fc352d` |
| `FETCH_VGMSTREAM` | `3rdparty/vgmstream.cmake` | `vgmstream/vgmstream` @ `r2117` |
| `FETCH_MGBA` | `3rdparty/mgba.cmake` | `kode54/mgba` branch `gsfplayer` @ `183a2e5b` |

Each plugin returns early when its dependency is unavailable, the same way the
gme plugin does without libgme.

### mGBA must be kode54's gsfplayer branch

Upstream mGBA has no `mgba/core/blip_buf.h`, which `xsfinput.cpp` includes. The
`gsfplayer` branch adds that header and keeps the audio mixer in minimal-core
builds, which GSF playback needs. It also still declares
`cmake_minimum_required(VERSION 3.1)`, which CMake 4 rejects, so the module sets
`CMAKE_POLICY_VERSION_MINIMUM` to 3.5 for that fetch only.

### vgmstream codec libraries must be linked by hand

`libvgmstream` is a static archive. Upstream links CELT, ATRAC9 and G.719 only
into its shared and CLI targets — `setup_target()` takes a `LINK` argument that
is false for the archive — so a consumer of the archive has to supply them.

Those libraries are `IMPORTED` CMake targets created inside vgmstream's own
directory, and `IMPORTED` targets are directory-scoped, so `if(TARGET celt0110)`
is false from `3rdparty/`. The module therefore references the archives the
sub-builds produce under `<vgmstream binary dir>/dependencies/` and adds a
dependency on the custom targets that build them.

G.719 is the exception with two shapes: on a first configure vgmstream
`add_subdirectory()`s it, giving a global `g719_decode` target, and on later
configures it sees the built archive and creates an `IMPORTED` target instead.
The module handles both.

Building vgmstream as a shared library instead is not an option: its
`LIBVGMSTREAM_EXPORT` macro expands to `__declspec(dllexport)`, so the shared
target does not compile with GCC.

Because vgmstream is linked statically, its codec dependencies become runtime
dependencies of the package. `CPACK_DEBIAN_PACKAGE_DEPENDS` in
`cmake/FooyinPackaging.cmake` lists them: `libmpg123-0t64`, `libvorbis0a`,
`libvorbisfile3`, `libvorbisenc2`, `libspeex1`, `libflac14`, `libogg0`. Building
also needs their `-dev` packages, which are not in `ci/ubuntu-depends.sh`.

## Verifying a plugin actually works

A fooyin plugin is a CMake `MODULE`, which links successfully with undefined
symbols and only fails when fooyin calls `dlopen` on it. Two defects got past a
green build this way, so check both of the following after touching a plugin or
its dependencies.

### 1. It loads

```c
/* dltest.c */
#include <dlfcn.h>
#include <stdio.h>
int main(int argc, char** argv) {
    void* h = dlopen(argv[1], RTLD_NOW);
    if(!h) { printf("FAIL: %s\n", dlerror()); return 1; }
    printf("OK\n");
    return 0;
}
```

```sh
gcc -o /tmp/dltest dltest.c -ldl
cd build/run/lib/fooyin
for p in plugins/*.so; do printf '%-40s ' "$p"; LD_LIBRARY_PATH=. /tmp/dltest "$p"; done
```

This is what caught two separate undefined symbols:

- `celt_decoder_destroy_0110` — the vgmstream codec archives described above.
- `syx_reset_gm` — SpessaSynth replaced it with `syx_reset_gs` in commit
  `402b2a0`, while `MIDIPlayer.cpp` still expected the library to export it.
  `MIDIPlayer.cpp` now defines `syx_reset_gm` itself and takes `syx_reset_gs`
  from the library.

### 2. It registers the extensions it should

Loading is not enough. `AudioLoader::addDecoder()` calls `decoder->extensions()`
once at registration and builds the extension-to-decoder table from the result.
A decoder that reports an empty list loads fine and then never gets offered any
file, and fooyin reports `No decoder available for track:` for every format the
plugin was supposed to handle.

Drive the plugin through `QPluginLoader` the way fooyin does, and check the
extension count as well as `init()`:

```cpp
QPluginLoader loader(absolutePluginPath);          // must be absolute
auto* input = qobject_cast<Fooyin::InputPlugin*>(loader.instance());
auto decoder = input->inputCreator().decoder();
qInfo() << decoder->extensions().size();           // vgmstream: 703

QFile file(path);
file.open(QIODevice::ReadOnly);
Fooyin::AudioSource source;
source.filepath = path;
source.device   = &file;
qInfo() << decoder->init(source, Fooyin::Track{path}, {}).has_value();
```

Link it against `include/`, `libfooyin_core` and `libfooyin_utils`.

This caught the third defect: `fileExtensions()` passed both the call that fills
a count and the count itself as arguments to one function,

```cpp
appendExtensions(extensions, libvgmstream_get_extensions(&size), size);
```

Argument evaluation is unsequenced. GCC read `size` before making the call, saw
`0`, and produced an empty list, so no vgmstream format could be played at all
even though decoding itself was fine.
