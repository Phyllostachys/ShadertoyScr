#version 430

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
in vec2 TexCoord;
out vec4 outColor;
uniform sampler2D textureData;
uniform samplerCube iChannel0;
/******************************************************************************/

// z * z
vec2 zmul(vec2 a, vec2 b)
{
    //return vec2(a.x*b.x-a.y*b.y, a.x*b.y+b.x*a.y);
    return mat2(a,-a.y,a.x)*b;
}

// 1 / z
vec2 zinv(vec2 a)
{
    return vec2(a.x, -a.y) / dot(a,a);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 g = fragCoord.xy;

    vec2 si = iResolution.xy;

    vec2 uv = (g+g-si)/min(si.x,si.y) ;

    uv *= 30. * (sin(10.6+iGlobalTime * 0.01)*.5+.5);//zoom

    uv += vec2(-0.46022,0.746155);

    if (iMouse.z > 0.)
        uv = (g+g-si)/min(si.x,si.y) * 2.;

    vec2 z = uv;

    vec2 c = vec2(0.66,1.23);

    float it = 0.;
    for (int i=0;i<600;i++)
    {
        z = zinv(zmul(z, z) + c);
        if( dot(z,z)>4. ) break;
        it++;
    }

    float sit = it-it/(log2(log2(dot(z,z))));

    fragColor = 0.5 + 0.5 * cos( 3. + sit*0.2 + vec4(0,0.6,1,1));
}

/******************************************************************************/
void main()
{
    mainImage(outColor, gl_FragCoord.xy);
}
