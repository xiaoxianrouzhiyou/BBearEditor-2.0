#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 51) out;

in V2G
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} v2g[];

struct G2FTriangle
{
    vec3 positions[3];
    vec3 normal;
    vec2 texcoord;
    vec2 area_and_level;
};

out G2F
{
    G2FTriangle triangle;
} g2f;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBCameraParameters;

uniform int NoiseNum;
uniform float Noise[64];
uniform float Sp[64];
// Divided into N stacks according to the size of the triangle
uniform int SNum;

const int TriangleVerticesNum = 51;


float getArea(vec3 positions[3])
{
    // Calculate the area according to the three points of the triangle
    vec3 V1 = positions[1] - positions[0];
	vec3 V2 = positions[2] - positions[0];
	vec3 c = cross(V1, V2);
	return 0.5 * sqrt(dot(c, c));
}

int getMaxEdge(vec3 edge[3])
{
    float e0 = dot(edge[0], edge[0]);
    float e1 = dot(edge[1], edge[1]);
    float e2 = dot(edge[2], edge[2]);
    if (e0 > e1 && e0 > e2)
        return 0;
    if (e1 > e0 && e1 > e2)
        return 1;
    if (e2 > e0 && e2 > e1)
        return 2;
}

void splitTriangle(vec3 triangle[3], inout vec3 sub_triangle0[3], inout vec3 sub_triangle1[3])
{
    vec3 V[3];
	V[0] = triangle[1] - triangle[0];
	V[1] = triangle[2] - triangle[1];
	V[2] = triangle[2] - triangle[0];
    int max_edge = getMaxEdge(V);
    if (max_edge == 0)
    {
        vec3 mid = 0.5 * (triangle[0] + triangle[1]);
		sub_triangle0[0] = triangle[0];
		sub_triangle0[1] = triangle[2];
		sub_triangle0[2] = mid;

		sub_triangle1[0] = triangle[2];
		sub_triangle1[1] = triangle[1];
		sub_triangle1[2] = mid;
    }
    else if (max_edge == 1)
    {
        vec3 mid = 0.5 * (triangle[2] + triangle[1]);
		sub_triangle0[0] = triangle[1];
		sub_triangle0[1] = triangle[0];
		sub_triangle0[2] = mid;

		sub_triangle1[0] = triangle[0];
		sub_triangle1[1] = triangle[2];
		sub_triangle1[2] = mid;
    }
    else
    {
        vec3 mid = 0.5 * (triangle[2] + triangle[0]);
		sub_triangle0[0] = triangle[1];
		sub_triangle0[1] = triangle[0];
		sub_triangle0[2] = mid;

		sub_triangle1[0] = triangle[2];
		sub_triangle1[1] = triangle[1];
		sub_triangle1[2] = mid;
    }
}

int getLevel(float area, float u)
{
    // A/Sp_k-1 < u < A/Sp_k
    int level = 0;
    for (int i = 1; i < SNum; i++)
    {
        float AS = area / Sp[i];
        if (u < AS)
        {
            level = i - 1;
            break;
        }
    }
    return level;
}

vec3 getNormal(vec3 triangle[3])
{
    vec3 V1 = triangle[1] - triangle[0];
	vec3 V2 = triangle[2] - triangle[0];
	return normalize(cross(V1, V2));
}

vec2 getTexcoord(vec3 triangle[3])
{
    // Center
    vec3 P = (triangle[0] + triangle[1] + triangle[2]) / 3.0;

    vec3 P0 = v2g[0].position;
	vec3 P1 = v2g[1].position;
	vec3 P2 = v2g[2].position;

	float b = ((P0.x - P2.x) * (P1.y - P2.y) - (P0.y - P2.y) * (P1.x - P2.x));
	float u = (P.x * (P1.y - P2.y) - P.y * (P1.x - P2.x)) / b;
	float v = (P.x * (P0.y - P2.y) - P.y * (P0.x - P2.x)) / (-b);
	
	vec2 T0 = v2g[0].texcoord - v2g[2].texcoord;
	vec2 T1 = v2g[1].texcoord - v2g[2].texcoord;

	return T0 * u + T1 * v;
}

void emitTriangle(vec3 triangle[3], float area, float level, int index)
{
    G2FTriangle g2f_triangle;
    g2f_triangle.positions[0] = triangle[0];
    g2f_triangle.positions[1] = triangle[1];
    g2f_triangle.positions[2] = triangle[2];
    g2f_triangle.normal = getNormal(triangle);
    g2f_triangle.texcoord = getTexcoord(triangle);
    g2f_triangle.area_and_level = vec2(area, level);

    for (int i = 0; i < 3; i++)
    {
        g2f.triangle = g2f_triangle;
        // gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * vec4(triangle[i], 1.0);
        gl_Position = vec4(g2f_triangle.positions[i], 1);
        EmitVertex();
    }
}


void main()
{
    // 1. To avoid uneven triangle segmentation, get the length of three sides of triangle
    // 2. Take the midpoint of the longest edge and divide it into two triangles
    // 3. Continue with the new two triangles

    // The triangle inputted from VS push into stack
    vec3 stack[TriangleVerticesNum];
    // The index of top of stack
    int top = -1;
    stack[++top] = v2g[0].position;
	stack[++top] = v2g[1].position;
	stack[++top] = v2g[2].position;

    // Random number: used for random culling, and get random number according to triangle ID
    float u = Noise[gl_PrimitiveIDIn % NoiseNum];
    float Sp_1 = Sp[0];
    float Sp_N = Sp[SNum - 1];

    int index = 0;

    // Pop triangle from the stack and subdivide triangle
    while (top > 0 && top < TriangleVerticesNum)
    {
        // 3 vertices of triangle
        vec3 triangle[3];
		triangle[0] = stack[top--];
		triangle[1] = stack[top--];
		triangle[2] = stack[top--];
        // The area of the triangle
        float area = getArea(triangle);

        if (area <= u * Sp_1)
            continue;

        if (area < u * Sp_N)
        {
            // The triangle needs to be subdivided
            if (top >= TriangleVerticesNum)
                continue;

            vec3 sub_triangle0[3];
            vec3 sub_triangle1[3];
            splitTriangle(triangle, sub_triangle0, sub_triangle1);

            // new sub triangles are pushed into the stack, and will be calculated
            if (top < TriangleVerticesNum)
            {
                stack[++top] = sub_triangle0[0];
				stack[++top] = sub_triangle0[1];
				stack[++top] = sub_triangle0[2];
            }
            if (top < TriangleVerticesNum)
            {
                stack[++top] = sub_triangle1[0];
				stack[++top] = sub_triangle1[1];
				stack[++top] = sub_triangle1[2];
            }
        }
        else
        {
            // Calculate the level of the triangle
            // G2F
            int level = getLevel(area, u);
            emitTriangle(triangle, area, level, index);
            index++;
        }
    }

    gl_Position = vec4(0, 0, 0, 1);
	EmitVertex();
    EndPrimitive();
}