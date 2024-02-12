#include "card_renderer.h"

void card_renderer_init_shaders(card_renderer* renderer) {
    file vertex_shader_file = file_load_from_path("../res/shaders/card_shader/card_shader_vertex.glsl");
    file fragment_shader_file = file_load_from_path("../res/shaders/card_shader/card_shader_fragment.glsl");

    renderer->card_shader = shader_program_create();

    renderer->triangle_vertex_shader = shader_stage_create();
    shader_stage_set_type(&renderer->triangle_vertex_shader, vertex_shader);
    shader_stage_set_source(&renderer->triangle_vertex_shader, file_get_data(&vertex_shader_file));
    
    renderer->triangle_fragment_shader = shader_stage_create();
    shader_stage_set_type(&renderer->triangle_fragment_shader, fragment_shader);
    shader_stage_set_source(&renderer->triangle_fragment_shader, file_get_data(&fragment_shader_file));
    
    shader_program_set_stage(&renderer->card_shader, &renderer->triangle_vertex_shader);
    shader_program_set_stage(&renderer->card_shader, &renderer->triangle_fragment_shader);

    shader_program_build(&renderer->card_shader);
}
void card_renderer_create_uniforms(card_renderer* renderer) {

    matrix4 empty_transform_matrix = matrix4_identity();

    renderer->camera_view_matrix = (shader_uniform){ &renderer->game_camera.camera_matrix, "camera_matrix", uniform_matrix4 };

    shader_program_bind(&renderer->card_shader);
    shader_program_set_uniform(&renderer->card_shader, &renderer->camera_view_matrix);
    shader_program_bind(NULL);
}
void card_renderer_create_camera(card_renderer* renderer) {
    // imma move this later
    renderer->game_camera = camera_create();
    camera_set_bounds(&renderer->game_camera, -640.0f, 640.0f, 360.0f, -360.0f);
    camera_set_position(&renderer->game_camera, (vector2){0.0f, 0.0f});
    camera_set_rotation(&renderer->game_camera, 0.0f);
}

struct card_vertex {
    float x, y;
    float u, v;
};

void card_renderer_create_vertex_array(card_renderer* renderer) {
    triangle triangles[22] = {
        { 0,  1, 22 },
        { 0, 23, 22 },

        { 1, 2, 21 },
        { 1, 22, 21 },

        { 2, 3, 20 },
        { 2, 21, 20 },

        { 3, 4, 19 },
        { 3, 20, 19 },

        { 4, 5, 18 },
        { 4, 19, 18 },

        { 5, 6, 17 },
        { 5, 18, 17 },

        { 6, 7, 16 },
        { 6, 17, 16 },

        { 7, 8, 15 },
        { 7, 16, 15 },

        { 8, 9, 14 },
        { 8, 15, 14 },

        { 9, 10, 13 },
        { 9, 14, 13 },

        { 10, 11, 12 },
        { 10, 13, 12 },
    };

    renderer->card_vertex_array = vertex_array_create();

    renderer->card_positions_buffer = array_buffer_create(); 
    array_buffer_set_data(&renderer->card_positions_buffer, NULL);
    array_buffer_set_vertex_count(&renderer->card_positions_buffer, 24);
    array_buffer_set_draw_type(&renderer->card_positions_buffer, dynamic_draw);   

    vertex_attribute positions_attribute = (vertex_attribute){ vertex_attribute_type_float, 2 };
    array_buffer_add_attribute(&renderer->card_positions_buffer, &positions_attribute);

    vertex_attribute uv_attribute = (vertex_attribute){ vertex_attribute_type_float, 2 };
    array_buffer_add_attribute(&renderer->card_positions_buffer, &uv_attribute);

    renderer->card_index_buffer = index_buffer_create();
    index_buffer_set_data(&renderer->card_index_buffer, triangles);
    index_buffer_set_triangle_count(&renderer->card_index_buffer, 22);
    index_buffer_set_draw_type(&renderer->card_index_buffer, static_draw); 

    vertex_array_add_array_buffer(&renderer->card_vertex_array, &renderer->card_positions_buffer);
    vertex_array_set_index_buffer(&renderer->card_vertex_array, &renderer->card_index_buffer);

    vertex_array_build(&renderer->card_vertex_array);
}

void card_renderer_create_texture(card_renderer* renderer) {
    renderer->cards_image = texture_2d_create();
    texture_2d_set_parameter(&renderer->cards_image, texture_2d_magnification_filter, texture_2d_filter_nearest);
    texture_2d_set_parameter(&renderer->cards_image, texture_2d_minification_filter, texture_2d_filter_nearest);

    texture_2d_set_parameter(&renderer->cards_image, texture_2d_wrap_x, texture_2d_wrap_repeat);
    texture_2d_set_parameter(&renderer->cards_image, texture_2d_wrap_y, texture_2d_wrap_repeat);

    texture_file cards_texture_file = texture_file_load_from_path("../res/images/Cards.png");
    texture_2d_set_width(&renderer->cards_image, cards_texture_file.width);
    texture_2d_set_height(&renderer->cards_image, cards_texture_file.height);

    texture_2d_set_color_mode(&renderer->cards_image, RGBA);
    texture_2d_set_data(&renderer->cards_image, cards_texture_file.pixel_data);

    texture_2d_build(&renderer->cards_image);
}

void card_renderer_init(card_renderer* renderer) {
    card_renderer_create_camera(renderer);

    card_renderer_init_shaders(renderer);
    card_renderer_create_uniforms(renderer);

    card_renderer_create_vertex_array(renderer);

    card_renderer_create_texture(renderer);
}
void card_renderer_draw_card(card_renderer* renderer, vector2 position, int card_index) {
    int CARD_SIZE = 40;

    struct card_vertex vertices[24] = {
        { -1.000f * CARD_SIZE,  1.750f * CARD_SIZE,     0.0556f, 1.0000f },
        { -1.045f * CARD_SIZE,  1.745f * CARD_SIZE,     0.0356f, 0.9986f },
        { -1.075f * CARD_SIZE,  1.735f * CARD_SIZE,     0.0222f, 0.9957f },
        { -1.110f * CARD_SIZE,  1.700f * CARD_SIZE,     0.0067f, 0.9857f },
        { -1.120f * CARD_SIZE,  1.670f * CARD_SIZE,     0.0022f, 0.9771f },
        { -1.125f * CARD_SIZE,  1.625f * CARD_SIZE,     0.0000f, 0.9643f },
        
        { -1.125f * CARD_SIZE, -1.625f * CARD_SIZE,     0.0000f, 0.0357f },
        { -1.120f * CARD_SIZE, -1.670f * CARD_SIZE,     0.0022f, 0.0223f },
        { -1.110f * CARD_SIZE, -1.700f * CARD_SIZE,     0.0067f, 0.0143f },
        { -1.075f * CARD_SIZE, -1.735f * CARD_SIZE,     0.0222f, 0.0043f },
        { -1.045f * CARD_SIZE, -1.745f * CARD_SIZE,     0.0356f, 0.0014f },
        { -1.009f * CARD_SIZE, -1.750f * CARD_SIZE,     0.0556f, 0.0000f },

        {  1.000f * CARD_SIZE, -1.750f * CARD_SIZE,     0.9444f, 0.0000f },
        {  1.045f * CARD_SIZE, -1.745f * CARD_SIZE,     0.9644f, 0.0014f },
        {  1.075f * CARD_SIZE, -1.735f * CARD_SIZE,     0.9778f, 0.0043f },
        {  1.110f * CARD_SIZE, -1.700f * CARD_SIZE,     0.9933f, 0.0143f },
        {  1.120f * CARD_SIZE, -1.670f * CARD_SIZE,     0.9978f, 0.0223f },
        {  1.125f * CARD_SIZE, -1.625f * CARD_SIZE,     1.0000f, 0.0357f },

        {  1.125f * CARD_SIZE,  1.625f * CARD_SIZE,     1.0000f, 0.9643f },
        {  1.120f * CARD_SIZE,  1.670f * CARD_SIZE,     0.9978f, 0.9771f },
        {  1.110f * CARD_SIZE,  1.700f * CARD_SIZE,     0.9933f, 0.9857f },
        {  1.075f * CARD_SIZE,  1.735f * CARD_SIZE,     0.9778f, 0.9957f },
        {  1.045f * CARD_SIZE,  1.745f * CARD_SIZE,     0.9644f, 0.9986f },
        {  1.000f * CARD_SIZE,  1.750f * CARD_SIZE,     0.9444f, 1.0000f }
    };
    array_buffer_reset_data(&renderer->card_positions_buffer, 0, vertices, sizeof(vertices));
}
void card_renderer_draw(card_renderer* renderer) {
    shader_program_bind(&renderer->card_shader);

    texture_2d_bind(&renderer->cards_image, 0);
    vertex_array_bind(&renderer->card_vertex_array);
    vertex_array_draw(&renderer->card_vertex_array);   
}
void card_renderer_draw_blank_card(card_renderer* renderer) {
    // vertex_array_bind(&renderer->card_vertex_array);
    // vertex_array_draw(&renderer->card_vertex_array);   
}
void card_renderer_cleanup(card_renderer* renderer) {
    shader_program_delete(&renderer->card_shader);
    vertex_array_delete(&renderer->card_vertex_array);
    texture_2d_delete(&renderer->cards_image);
}