# What this is

Small demos that show several techniques in game-programming.

Currently contains:

[pointer_entities](./pointer_entities/) demonstrates how keeping
pointers between entities can lead to faulty
behavior because entities may keep pointers
to memory that was occupied by some other
entity during gameplay (because the original
died, for example).

Check out Andre Weissflog's blogpost about this
matter: [Handles are the better pointers](https://floooh.github.io/2018/06/17/handles-vs-pointers.html)

## How to build and run

```bash
cmake -B build -S .
cmake --build build
./build/pointer_entities/pointer_entities
```

## Thanks

Thanks to [Raylib](https://github.com/raysan5/raylib) which makes demos like this easy to build.

