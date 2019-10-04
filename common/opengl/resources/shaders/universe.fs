#version 330 core
out vec4 out_frag_color;

in vec2 io_txr;

uniform sampler2D uf_text_0;

void main() {
    out_frag_color = texture(uf_text_0, io_txr);
}
