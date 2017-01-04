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
ShadertoyScr shaders/ring_twister.frag
```
If you run ShadertoyScr without any arguments then it'll use ring_twister.frag.

## Shaders
- [Metaballs Quintic](https://www.shadertoy.com/view/ld2GRz) was made by iq and Hexes was inspired by [this shader from Shadertoy](https://www.shadertoy.com/view/Xd2GR3) (I think) by [iq, Inigo Quilez](http://www.iquilezles.org).
- [Awesome star](https://www.shadertoy.com/view/4lfSzS) by [foxes](www.panteleymonov.ru)
- [Matroshka](https://www.shadertoy.com/view/XlcSzM) by [BigWIngs](https://www.shadertoy.com/user/BigWIngs)
- [Ring Twister](https://www.shadertoy.com/view/Xt23z3) by [Flyguy](https://www.shadertoy.com/user/Flyguy)
- [Alien Cavern](https://www.shadertoy.com/view/XljGR3), [Fractal Experiment 23](https://www.shadertoy.com/view/MlcXRl), [Ray Marching Experiement 60](https://www.shadertoy.com/view/lttXDn), and [Tunnel Beauty 7](https://www.shadertoy.com/view/XltSDn) are by [aiekick](https://www.shadertoy.com/user/aiekick).
- [Seascape](https://www.shadertoy.com/view/Ms2SD1) and [Wet stone](https://www.shadertoy.com/view/ldSSzV) by [TDM](https://www.shadertoy.com/user/TDM).
- [Plasma Globe](https://www.shadertoy.com/view/XsjXRm) and [HURA Generator](https://www.shadertoy.com/view/MtlXD8) by [nimitz](https://www.shadertoy.com/user/nimitz).

## TODO
- [ ] Make fullscreen on every monitor work.
- [ ] Add in other shader inputs given on Shadertoy (iChannel0..3, et al).
- [ ] Allow passing in of Shadertoy ids.
- [ ] Add more cool shaders, of course.
