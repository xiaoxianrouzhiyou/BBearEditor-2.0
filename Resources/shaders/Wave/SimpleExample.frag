varying vec3 v2f_view_space_pos;
varying vec2 v2f_texcoords;
varying vec3 v2f_normal;

uniform vec4 BBCameraPosition;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;


vec3 computeTangent()
{
    vec3 Q1 = dFdx(v2f_view_space_pos);
    vec3 Q2 = dFdy(v2f_view_space_pos);
    vec2 st1 = dFdx(v2f_texcoords);
    vec2 st2 = dFdy(v2f_texcoords);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    return T;
}

vec4 computeAnisotropicHighlights(float u, vec3 N, vec3 H, float r, vec4 hiliteColor)
{
    // Anisotropic highlights from Greg Ward
    // Roughness r, hiliteColor
    // The final result is the superposition of dispersion and anisotropic highlights.
    
    // Related to H
    float w = dot(N, H);
    float e = r * u / w;
    float c = exp(-e * e);
    vec4 specular = hiliteColor * vec4(c, c, c, 1.0);
    return specular;
}

vec3 blend3(vec3 x)
{
    vec3 y = 1 - x * x;
    y = max(y, vec3(0.0));
    return y;
}

void main(void)
{
    vec3 N = normalize(v2f_normal);
    vec3 L = normalize(BBLightPosition.xyz);
    vec3 V = normalize(BBCameraPosition.xyz - v2f_view_space_pos);
    vec3 H = normalize(L + V);
    vec3 T = computeTangent();
    float d = 0.001;
    float u = dot(T, H) * d;
    
    vec4 specular = computeAnisotropicHighlights(u, N, H, 0.2, vec4(0.9, 0.9, 0.9, 1.0));

    if (u < 0.0) 
        u = -u;

    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    // iteration
    for (int n = 1; n <= 8; n++)
    {
        // Wave length 0~1mm
        float y = 2 * u / n - 1;
        // Spectrum
        // R(Wave length) = bump(C * (y - 0.75));
        // G(Wave length) = bump(C * (y - 0.50));
        // B(Wave length) = bump(C * (y - 0.25));
        // C is shape func, 4.0 is great
        diffuse.xyz += blend3(4.0 * vec3(y - 0.75, y - 0.5, y - 0.25));
    }

    gl_FragColor = diffuse;
}

