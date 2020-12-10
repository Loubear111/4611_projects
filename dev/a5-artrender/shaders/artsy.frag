#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using a toon shading model

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

uniform sampler2D diffuse_ramp;
uniform sampler2D specular_ramp;


void main() {
    // transform the vertex position into "eye space"
    vec3 v = position_in_eye_space;

    // unit vector from the pixel to the light
    vec3 l = normalize(light_in_eye_space - v);

    // unit vector from the pixel to the eye point, which is at 0,0,0 in "eye space"
    vec3 e = normalize(vec3(0,0,0) - v);

    // normal transformed into "eye space"
    vec3 n = normalize(normal_in_eye_space);

    // halfway vector
    vec3 h = normalize(l + e);

    // calculating lighting output the color for this pixel
    vec4 ambient = ka * Ia;

    //vec4 diffuse = ka * Id * max(dot(n, l), 0); // clamp the dot product to 0-1
    float diff_intensity = (dot(n, l) + 1)/2;
    vec2 texcoords;
    texcoords.s = diff_intensity;
    vec4 diffuse = ka * Id * texture(diffuse_ramp, texcoords);

    float spec_intensity = pow(max(dot(h, n), 0), s);
    texcoords.s = spec_intensity;
    vec4 specular = ks * Is * texture(specular_ramp, texcoords); // clamp dot product again to 0-1


    color = ambient + diffuse + specular;
}
