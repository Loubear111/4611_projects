#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to modify this vertex shader to move the edge vertex along
// the normal away from the mesh surface IF you determine that the vertex
// belongs to a silhouette edge.


uniform mat4 model_view_matrix;
uniform mat4 normal_matrix;
uniform mat4 proj_matrix;
uniform float thickness;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 left_normal;
layout(location = 3) in vec3 right_normal;

void main() {
    
    // transform the vertex position into "eye space"
    vec3 vert = (model_view_matrix * vec4(vertex, 1)).xyz;
    
    // unit vector from the vertex to the eye point, which is at 0,0,0 in "eye space"
    vec3 to_eye = normalize(vec3(0,0,0) - vert);

    // normal transformed into "eye space"
    vec3 vert_norm = (normal_matrix * vec4(normal, 0)).xyz;
    vec3 right_norm = (normal_matrix * vec4(left_normal, 1)).xyz;
    vec3 left_norm = (normal_matrix * vec4(right_normal, 1)).xyz;

    if(dot(to_eye, left_norm) * dot(to_eye, right_norm) < 0.0)
    {
        vec3 newV = vertex + (thickness*normal); // vertex is read-only, so make a new vertex shifted by the normal
        gl_Position = proj_matrix * model_view_matrix * vec4(newV,1);
    }
    else
    {
        gl_Position = proj_matrix * model_view_matrix * vec4(vertex,1);
    }
}
