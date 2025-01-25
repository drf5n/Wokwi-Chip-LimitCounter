# Wokwi-Chip-LimitCounter
## Description

A counter with limit switch outputs custom-chip for Wokwi

-  

To use this chip in your project, include it as a dependency in your `diagram.json` file:

```json
"dependencies": { "chip-limit-counter": "github:drf5n/Wokwi-Chip-LimitCounter@1.0.0" }
```

Then, add the chip to your circuit by adding a `chip-limit-counter` item to the `parts` section of `diagram.json`:

```json
  "parts": {
    ...,
    {
      "type": "chip-limit-counter",
      "id": "counter1",
      "attrs": { }
    },
```

The actual source code for the chip lives in [src/main.c](https://github.com/drf5n/Wokwi-Chip-LimitCounter/blob/main/src/main.c),
and the pins are described in [chip.json](https://github.com/drf5n/Wokwi-Chip-LimitCounter/blob/main/chip.json).

This is a copy of the custom chip in
https://wokwi.com/projects/407612178959256577


## Bugs -- 

Currently I don't know how to compile a wasm.  I was following the instructions in the Makefile, but those are aimed at an old
distribution of clang and wasi-libc ifollowing https://github.com/wokwi/inverter-chip/blob/main/Makefile per:

```
# Copied from https://github.com/wokwi/inverter-chip/blob/main/Makefile 
# per https://discord.com/channels/787627282663211009/954892209486966825/1274468132618244128
# modified for mac homebrew environment
# Followed https://00f.net/2019/04/07/compiling-to-webassembly-with-llvm-and-clang/ on Mac
#  but with this for my homebrew install:
#     export PATH="/opt/homebrew/opt/llvm/bin:$PATH" 
#  
```

The https://00f.net/2019/04/07/compiling-to-webassembly-with-llvm-and-clang/ suggestion doesn't work
with /opt/homebrew/Cellar/llvm/19.1.7/ and at the point where it suggests https://github.com/jedisct1/libclang_rt.builtins-wasm32.a
the link suggests "zig" instead of this tool stack for post 2023 development.

    zig cc --target=wasm32-wasi -shared -o dist/chip.wasm src/main.c  # seems to work


## Examples

* [LimitChip Demo -- github](https://wokwi.com/projects/411487509400518657)
* [LimitChip Demo -- tabbed](https://wokwi.com/projects/407612178959256577)

## See also:

* 


## Versions
* github:drf5n/Wokwi-Chip-LimitChip@1.0.0 -- Working release

# notes on making a Wokwi custom chip work with Github repository dependency
To get the Wokwi build script working to build the necessary chip.zip file for distribution with a release so Wokwi can pick it up

1) enable the repository settings for workflow permissions to be read-write
2) make sure the .github/workflows/build.yaml is in the repository
3) commit
4) make a vN.n.n tag: `git tag -a "v1.0.1" -m "build.yaml"`
5) push the tag  to github: `git push origin tag v1.0.1`

Refer to
https://discord.com/channels/787627282663211009/954892209486966825/1274569798231130163
for a little discussion on workflow.


## License

This project is licensed under the MIT license. See the [LICENSE](https://github.com/drf5na/Wokwi-Chip-LimitChip/blob/main/LICENSE) file for more details.
