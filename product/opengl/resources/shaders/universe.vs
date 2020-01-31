#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_txr;
layout (location = 2) in vec3 in_norm;

out vec3 io_pos;
out vec2 io_txr;
out vec3 io_norm;

uniform mat4 uf_xform_model;
uniform mat4 uf_xform_view;
uniform mat4 uf_xform_projection;

void main() {
    gl_Position = uf_xform_projection * uf_xform_view * uf_xform_model * vec4(in_pos, 1.0f);
    io_pos = vec3(uf_xform_model * vec4(in_pos, 1.0));
    io_txr = in_txr;
    io_norm = mat3(transpose(inverse(uf_xform_model))) * in_norm;
}
