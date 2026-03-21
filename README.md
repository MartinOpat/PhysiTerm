# PhysiTerm
Retro-style terminal-based 2D physics sandbox

## Instructions
It is written in C and terminal-based so just compile it and you can probably run it on everything from super clusters to a smart fridge.

Compilation command:
```bash
mkdir build && cd build
cmake ..
make -j
```

You can run the game (from the `build` folder) like:
```bash
./bin/physiterm
```

Optionally, you can specify the number of particles to spawn initially (default: 64)
```bash
./bin/physiterm 1024
```

To quite press "q".
