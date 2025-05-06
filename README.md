# Rhythm Racing Engine [![Discord Badge]][discord]

[Discord Badge]: https://img.shields.io/discord/1328433046105362546?color=%237289DA&logo=discord&logoColor=%23FFFFFF
[discord]: https://discord.gg/VW3Ud4zWcm

A work-in-progress *non-matching* decompilation of Rainbow Studios' Rhythm Racing Engine (1.0), as implemented in the PC version of Cars: Mater-National Championship.

This repository does not contain any game assets or assembly whatsoever. An existing copy of the game is required. You can purchase the game on Steam [here](https://store.steampowered.com/app/332260/).

# Dependencies

We currently use [vcpkg](https://vcpkg.io) in manifest mode for dependency management, and depend on the following libraries:
 - zlib
 - dxsdk-d3dx

Additionally, we need to manually link against two other libraries: [BASS](https://www.un4seen.com/bass.html) and [Bink](https://www.radgametools.com/bnkmain.htm).

The correct version of BASS can easily be found [on Un4Seen's website](https://www.un4seen.com/files/bass23.zip). After downloading the `bass23.zip` archive, simply copy `bass23.zip/c/bass.lib` to the `dependencies` folder.

Since Bink is proprietary middleware, you'll need to provide `.lib` and `.dll` files yourself; `binkw32.dll` can be found in the root of the original game's install folder, but the `.lib` must be generated from the `.dll` via `DUMPBIN` and `LIB`.

# Building

We currently use Visual Studio 2022 as our build system, as it plays nicely with vcpkg. However, since it is not cross-platform, we may switch to a different build system later down the line...

# Contributing

We are extremely lax with contribution requirements, as we are NOT trying to match the original game's assembly. That being said, we do use special tags for each function definition to help track our progress:

```c++
// OFFSET: 0xDEADBEEF, STATUS: WIP
void* ExampleClass::ExampleFunction(const char* name) {
  return nullptr;
}
```
The name of each tag must be followed by a semicolon, a space, and then its corresponding value. Each tag/value pair is delimited with a comma and a space.

As of now, we have two different tags: `OFFSET` and `STATUS`. `OFFSET` refers to the address of the function in the original executable, while `STATUS` refers to the implementation status of the function.

`OFFSET` can be either an absolute address in the original executable in hexadecimal, OR `INLINE`. `INLINE` denotes that the function is either actually `inlined` or that the implementation is shared by multiple functions; for example, a function that does nothing and returns 0. 

`STATUS` can be one of three values:
- `TODO` - Denotes that the implementation is either empty or particularly 'hacky'.
- `WIP` - Denotes that the implementation is a work-in-progress or contains minor 'hacks'.
- `COMPLETE` - Denotes that the implementation is functionally complete and should behave nearly identically to the original.

Intentional inaccuracies for the sake of compatibility or debugging *are permitted*, as long as there is adequate documentation within the code that explains the discrepancy.
