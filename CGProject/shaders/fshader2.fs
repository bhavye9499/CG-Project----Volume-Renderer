#version 420

in vec4 glPos;
out vec4 fragColor;

uniform sampler3D volumeSampler;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 eye;

uniform float R1 = 1;
uniform float G1 = 0;
uniform float B1 = 0;

uniform float R2 = 0;
uniform float G2 = 1;
uniform float B2 = 0;

uniform float R3 = 0;
uniform float G3 = 0;
uniform float B3 = 1;

uniform float MidColorPos = 0.5;

const int INF = 100000;

struct OBB
{
    vec3 a_c;
    vec3 a[3];
    float h[3];
};

vec2 genIntersectionParameter(vec3 o, vec3 d, OBB obb)
{
    float tmin = -INF;
    float tmax = INF;
    vec3 p = obb.a_c - o;
    for (int i = 0; i < 3; i++) {
        float e = dot(obb.a[i], p);
        float f = dot(obb.a[i], d);
        float one_f = 1 / f;
        if (length(f) > 0.00001) {
            float t1 = (e + obb.h[i]) * one_f;
            float t2 = (e - obb.h[i]) * one_f;
            if (t1 > t2) {
                float temp = t1; t1 = t2; t2 = temp;
            }
            if (t1 > tmin) {
                tmin = t1;
            }
            if (t2 < tmax) {
                tmax = t2;
            }
            if (tmin > tmax || tmax < 0) {
                 return vec2(0, 0);
            }
        }
        else if (-e - obb.h[i] > 0 || -e + obb.h[i] < 0) {
            return vec2(0, 0);
        }
    }
    if (tmin > 0) {
        return vec2(tmin, tmax);
    }
    return vec2(0, tmax);
}

vec3 linearInterpolate(vec3 a, vec3 b, float t)
{
    return a + t * (b - a);
}

vec4 getColorFromTF(float a)
{
    if (a < 0 || a > 1)
    {
        return vec4(0);
    }

    vec3 c[3] = {vec3(R1, G1, B1), vec3(R2, G2, B2), vec3(R3, G3, B3)};
    const float lim = MidColorPos;
    if (0 <= a && a < lim) {
           return vec4(linearInterpolate(c[0], c[1], a / lim), a);
    } else {
        return vec4(linearInterpolate(c[1], c[2], (a - lim) / (1 - lim)), a);
    }
}

vec4 getVoxelData(vec3 p, vec3 c, float R)
{
    float dist = dot(p - c, p - c);
    vec4 result = vec4(vec3(0), length(p));

    if (dist - R * R <= 0)
    {
        // Using Wyvill fall-off function
        for (int i = 0; i < 3; i++)
        {
            result[i] = pow((1 - (p[i] * p[i]) / (R * R)), 3);
        }
    }

    return result;
}

void main()
{
    // Getting ray direction in world coordinates
    // ------------------------------------------
    vec4 ray = vec4((inverse(vProjection) * glPos).xy, -1, 0);
    vec3 d = normalize((inverse(vView) * ray).xyz);

    // Forming the OBB
    // ---------------
    OBB obb;
    obb.a_c = vec3(0);
    obb.a[0] = normalize((vModel * vec4(1, 0, 0, 0)).xyz);
    obb.a[1] = normalize((vModel * vec4(0, 1, 0, 0)).xyz);
    obb.a[2] = normalize((vModel * vec4(0, 0, 1, 0)).xyz);
    obb.h[0] = obb.h[1] = obb.h[2] = 0.75;

    // Finding start and end intersection points
    // -----------------------------------------
    vec2 t = genIntersectionParameter(eye, d, obb);
    // Ray does not intersect with OBB
    if (t[0] == 0)
    {
        fragColor = vec4(0);
        return ;
    }

    vec3 startPoint = eye + t[0] * d;
    vec3 endPoint = eye + t[1] * d;
    startPoint = vec3(vModel * vec4(startPoint, 1.0));
    endPoint = vec3(vModel * vec4(endPoint, 1.0));

    // To view start and end points
//	fragColor = vModel * vec4(endPoint + vec3(0.5), 1.0);
//	return;

    // Ray Marching
    // ------------
    vec3 dir = normalize(endPoint - startPoint);
    vec3 curPoint = startPoint;
    vec3 step = dir * 0.001;

    vec4 outColor = vec4(0);

    for (int i = 0; i < 1000; i++)
    {
        vec4 voxelData = texture(volumeSampler, curPoint);
        vec4 color = getColorFromTF(voxelData.x);

        // From slides, C_s = color.xyz and Alpha_s = color.a
        //              C = outColor.xyz and Alpha = outColor.a
        color.a = 1.0 - pow(1.0 - color.a, 0.25f);
        outColor.rgb = color.rgb * color.a * (1.0 - outColor.a) + outColor.rgb;
        outColor.a = color.a * (1.0 - outColor.a) + outColor.a;

        // If curPoint has exited the volume
        if (length(curPoint - startPoint) > length(endPoint - startPoint))
        {
            outColor.rgb = outColor.rgb * outColor.a + vec3(1) * (1 - outColor.a);
            break;
        }

        // Early Ray Termination
        if (outColor.a > 0.975)
        {
            break;
        }

        curPoint += step;
    }

    fragColor = outColor;

}
