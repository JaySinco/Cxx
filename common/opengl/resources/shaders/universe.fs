#version 330 core
out vec4 out_frag_color;

in vec3 io_pos;
in vec2 io_txr;
in vec3 io_norm;

struct Light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

uniform vec3 uf_camera_pos;
uniform Light uf_light;  
uniform Material uf_material;

void main() {
    // ambient
    vec3 ambient  = uf_light.ambient * vec3(texture(uf_material.diffuse, io_txr));
    // diffuse 
    vec3 norm = normalize(io_norm);
    vec3 light_dir = normalize(uf_light.pos - io_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse  = uf_light.diffuse * (diff * vec3(texture(uf_material.diffuse, io_txr)));
    // specular
    vec3 view_dir = normalize(uf_camera_pos - io_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), uf_material.shininess);
    vec3 specular = uf_light.specular * (spec * uf_material.specular);  
    // output
    vec3 mixed = ambient + diffuse + specular;
    out_frag_color = vec4(mixed, 1.0);
}
