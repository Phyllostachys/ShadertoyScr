## Build
### Windows 64-bit
Run the following:
```
vcvars64.bat
build_win64.bat
```
### Windows mingw-w64
Run the following:
```
build_mingw-w64.sh
```

Output is sent to the bin folder.

## Examples
Run some of the shaders that come with this like this:
```
ShadertoyScr shaders/hexes.frag
```
If you run ShadertoyScr without any arguments then it'll use hexes.frag.

## Shaders
- Hexes was inspired by [this shader from Shadertoy](https://www.shadertoy.com/view/Xd2GR3) (I think) by [iq, Inigo Quilez](http://www.iquilezles.org).
- [Awesome star](https://www.shadertoy.com/view/4lfSzS) by [foxes](www.panteleymonov.ru)
- [Matroshka](https://www.shadertoy.com/view/XlcSzM) by [BigWIngs](https://www.shadertoy.com/user/BigWIngs)
- [Ring Twister](https://www.shadertoy.com/view/Xt23z3) by [Flyguy](https://www.shadertoy.com/user/Flyguy)

## TODO
- [ ] Make fullscreen on every monitor work.
- [ ] Allow passing in of Shadertoy ids.
- [ ] Add more cool shaders, of course.
