# OBS Equirectangular to Fisheye (Metal) Filter

An OBS Studio video filter that converts equirectangular (360x180) images/videos into a fisheye projection, accelerated via OBS's GPU effect system. On macOS this runs on Apple's Metal backend automatically.

## Controls
- FoV (deg)
- Pan (deg)
- Tilt (deg)
- Yaw (deg)

## Build (macOS)
Prerequisites:
- OBS Studio installed with development files (headers and `libobs`). If you build OBS from source, point pkg-config to its install prefix.
- CMake >= 3.20
- Xcode command line tools

```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

## Install
Copy the built plugin bundle and data into your OBS install:

- Plugin: `build/obs-equi2fisheye.plugin` -> `/Library/Application Support/obs-studio/plugins/obs-equi2fisheye/bin/obs-equi2fisheye.plugin`
- Data: `data/` -> `/Library/Application Support/obs-studio/plugins/obs-equi2fisheye/data/`

Alternatively, use `cmake --install .` to install into the system-default locations used by OBS.

## Usage
- In OBS, add your equirectangular source (e.g., Media Source).
- Right-click the source -> Filters -> Add -> "Equirectangular to Fisheye (Metal)".
- Tweak FoV/Pan/Tilt/Yaw to achieve the desired view.

## Notes
- The shader uses an equidistant fisheye model. Adjust FoV to control the fisheye aperture.
- Color filtering and performance are handled by OBS; on macOS, the effect is compiled to Metal.
