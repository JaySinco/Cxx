#version 330 core
out vec4 out_frag_color;

in vec3 io_pos;
in vec2 io_txr;
in vec3 io_norm;

struct Light {
    int all_type;
    vec3 all_ambient;
    vec3 all_diffuse;
    vec3 all_specular;
    vec3 point_spot_position;
    vec3 direct_spot_direction;
    float point_constant;
    float point_linear;
    float point_quadratic;
    float spot_innerCutOff;
    float spot_outerCutOff;
};

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

uniform vec3 uf_camera_pos;
// 0 - directional light
// 1 - point light
// 2 - spot light
uniform Light uf_light;
uniform Material uf_material;

void main() {
    // ambient
    vec3 ambient  = uf_light.all_ambient * vec3(texture(uf_material.diffuse, io_txr));
    // diffuse 
    vec3 norm = normalize(io_norm);
    vec3 light_dir = normalize(uf_light.point_spot_position - io_pos);
    if (uf_light.all_type == 0)
        light_dir = normalize(-uf_light.direct_spot_direction);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse  = uf_light.all_diffuse * (diff * vec3(texture(uf_material.diffuse, io_txr)));
    // specular
    vec3 view_dir = normalize(uf_camera_pos - io_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), uf_material.shininess);
    vec3 specular = uf_light.all_specular * (spec * uf_material.specular);  
    // output
    if (uf_light.all_type == 1) {
        float distance = length(uf_light.point_spot_position - io_pos);
        float attenuation = 1.0 / (uf_light.point_constant + uf_light.point_linear * distance + 
            uf_light.point_quadratic * (distance * distance));  
        ambient  *= attenuation; 
        diffuse  *= attenuation;
        specular *= attenuation;
    } else if (uf_light.all_type == 2) {
        float theta = dot(light_dir, normalize(-uf_light.direct_spot_direction));
        float epsilon = uf_light.spot_innerCutOff - uf_light.spot_outerCutOff;
        float intensity = clamp((theta - uf_light.spot_outerCutOff) / epsilon, 0.0, 1.0);   
        diffuse  *= intensity;
        specular *= intensity;
    }
    vec3 mixed = ambient + diffuse + specular;
    out_frag_color = vec4(mixed, 1.0);
}
