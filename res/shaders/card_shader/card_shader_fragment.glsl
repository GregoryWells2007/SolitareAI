#version 330 core

out vec4 color;
in vec2 v_uv;

uniform sampler2D texure;
uniform int u_card_index;
uniform int mouse_over;

ivec2 card_texture_size = ivec2(13, 4);

ivec2 calc_from_1d_index(int index) {
    ivec2 return_value = ivec2(0);
    return_value.x = int(mod(index, card_texture_size.x));
    return_value.y = index / card_texture_size.x;
    return return_value;
}

void main(void) {
    ivec2 card_index = calc_from_1d_index(u_card_index);
    
    vec2 uv = vec2((v_uv.x + card_index.x) / card_texture_size.x, ((v_uv.y - 1) - card_index.y) / card_texture_size.y);
    color = texture(texure, uv) - (vec4(vec3(0.25), 1.0) * mouse_over);
} 