#version 430

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
in vec2 TexCoord;
out vec4 outColor;
uniform sampler2D textureData;
uniform samplerCube iChannel0;
/******************************************************************************/

// Created by Stephane Cuillerdier - Aiekick/2016
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.


mat3 RotZ(float a)
{
    return mat3(cos(a),-sin(a),0.,sin(a),cos(a),0.,0.,0.,1.);
}

float df(vec3 p)
{
    vec3 q = p;
    q *= RotZ(q.z * 0.1);
    q += sin(q.zxy * 0.2);
    q *= RotZ(q.z * 0.1);
    q = sin(q.zxy * 0.15)*1.5;
    p += q;
    p *= RotZ(p.z * 0.015);
    p *= 0.5;
    return mix(25.,0.,abs(fract(atan(p.x, p.y)/3.14159*3.)-.5)) - max(abs(p.x),abs(p.y));
}

vec3 nor( vec3 pos, float prec )
{
    vec3 eps = vec3( prec, 0., 0. );
    vec3 nor = vec3(
        df(pos+eps.xyy) - df(pos-eps.xyy),
        df(pos+eps.yxy) - df(pos-eps.yxy),
        df(pos+eps.yyx) - df(pos-eps.yyx) );
    return normalize(nor);
}

// return color from temperature
//http://www.physics.sfasu.edu/astro/color/blackbody.html
//http://www.vendian.org/mncharity/dir3/blackbody/
//http://www.vendian.org/mncharity/dir3/blackbody/UnstableURLs/bbr_color.html
vec3 blackbody(float Temp)
{
    vec3 col = vec3(255.);
    col.x = 56100000. * pow(Temp,(-3. / 2.)) + 148.;
    col.y = 100.04 * log(Temp) - 623.6;
    if (Temp > 6500.) col.y = 35200000. * pow(Temp,(-3. / 2.)) + 184.;
    col.z = 194.18 * log(Temp) - 1448.6;
    col = clamp(col, 0., 255.)/255.;
    if (Temp < 1000.) col *= Temp/1000.;
    return col;
}

// get density of the df at surfPoint
// ratio between constant step and df value
float SubDensity(vec3 surfPoint, float prec, float ms)
{
    vec3 n;
    float s = 0.;
    const int iter = 10;
    for (int i=0;i<iter;i++)
    {
        n = nor(surfPoint,prec);
        surfPoint = surfPoint - n * ms;
        s += df(surfPoint);
    }

    return 1.-s/(ms*float(iter)); // s < 0. => inside df
}

float SubDensity(vec3 p, float s)
{
    vec3 n = nor(p,s);                          // precise normale at surf point
    return df(p - n * s);                       // ratio between df step and constant step
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 g = fragCoord.xy;
    vec2 si = iResolution.xy;
    vec2 uv = (g+g-si)/si.y;
    vec3 ro = vec3(26.5,26.5, iGlobalTime * 80.);
    ro.xy *= sin(-ro.z * 0.005);
    ro *= RotZ(-ro.z * 0.015);
    vec3 cv = ro + vec3(0,0,0.2);
    vec3 cu = normalize(vec3(0,1,0));
    vec3 z = normalize(cv-ro);
    vec3 x = normalize(cross(cu,z));
    vec3 y = cross(z,x);
    float fov = .9;
    vec3 rd = normalize(fov * (uv.x * x + uv.y * y) + z);

    float s = 1., d = 0.;
    for (int i=0; i<200; i++)
    {
        if (log(d*d/s/1e5)>0.) break;
        d += (s=df(ro+rd*d))*.2;
    }

    vec3 p = ro + rd * d;                                           // surface point
    vec3 lid = normalize(ro-p);                                     // light dir
    vec3 n = nor(p, 0.1);                                           // normal at surface point
    vec3 refl = reflect(rd,n);                                      // reflected ray dir at surf point
    float diff = clamp( dot( n, lid ), 0.0, 1.0 );                  // diffuse
    float fre = pow( clamp( 1. + dot(n,rd),0.0,1.0), 4. );          // fresnel
    float spe = pow(clamp( dot( refl, lid ), 0.0, 1.0 ),16.);       // specular
    vec3 col = vec3(.8,.5,.2);

    // here the magic happen
    float sss = df(p - n*0.001)/0.06;                               // quick sss 0.001 of subsurface

    float sb = SubDensity(p, 0.01, 0.1);                            // deep subdensity from 0.01 to 0.1 (10 iterations)
    vec3 bb = clamp(blackbody(200. * sb),0.,1.);                    // blackbody color
    float sss2 = 0.8 - SubDensity(p, 3.);                           // one step sub density of df of 3 of subsurface

    fragColor.rgb = (diff + fre + bb * sss2 * .8 + col * sss * .2) * 0.25 + spe * 1.2;

    fragColor.rgb = mix(fragColor.rgb, col * sss, 1.-exp(-0.0001*d*sss));

    // vigneting from iq Shader Mike : https://www.shadertoy.com/view/MsXGWr
    vec2 q = g/si;
    fragColor.rgb *= 0.5 + 0.5*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.55 );
}

/******************************************************************************/
void main()
{
    mainImage(outColor, gl_FragCoord.xy);
}
