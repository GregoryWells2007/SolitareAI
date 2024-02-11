#include "stdio.h"
#include "engine.h"

#include "board/board_renderer.h"
#include "card/card_renderer/card_renderer.h"

struct triangle_vertex {
    float x, y;
    float u, v;
};

int main(int argc, char** argv) {
    engine_init();

    window_manager win_manager = window_manager_create();
    
    window main_window = window_create();
    window_manager_set_main_window(&win_manager, &main_window);

    window_data* main_window_data = window_get_data(&main_window); 
    window_data_set_name(main_window_data, "Main Window\0");
    window_data_set_size(main_window_data, (int2){ 1280, 720 });
    window_update_data(&main_window);

    window_properties* main_window_properties = window_get_properties(&main_window); // imma add some properties later
    window_properties_set_resizable(main_window_properties, false);
    window_properties_set_maximized(main_window_properties, false);
    window_properties_set_decorated(main_window_properties, true);
    window_update_properties(&main_window);

    window_open(&main_window);

    framebuffer screen_framebuffer = framebuffer_create();

    // create a screen texture

    texture_2d color_texture = texture_2d_create();
    texture_2d_set_parameter(&color_texture, texture_2d_magnification_filter, texture_2d_filter_linear);
    texture_2d_set_parameter(&color_texture, texture_2d_minification_filter, texture_2d_filter_linear);

    texture_2d_set_parameter(&color_texture, texture_2d_wrap_x, texture_2d_wrap_repeat);
    texture_2d_set_parameter(&color_texture, texture_2d_wrap_y, texture_2d_wrap_repeat);

    texture_2d_set_width(&color_texture, 1280);
    texture_2d_set_height(&color_texture, 720);

    texture_2d_set_color_mode(&color_texture, RGBA);

    texture_2d_build(&color_texture);

    framebuffer_attachment framebuffer_color_attachment = framebuffer_attachment_create();
    framebuffer_attachment_set_data_type(&framebuffer_color_attachment, framebuffer_data_type_texture_2d);
    framebuffer_attachment_set_attachment_type(&framebuffer_color_attachment, color_attachment_0);
    framebuffer_attachment_set_data(&framebuffer_color_attachment, &color_texture);
    framebuffer_add_attachment(&screen_framebuffer, &framebuffer_color_attachment);

    renderbuffer depth_stencil_texture = renderbuffer_create();
    renderbuffer_set_width(&depth_stencil_texture, 1280);
    renderbuffer_set_height(&depth_stencil_texture, 720);
    renderbuffer_set_color_mode(&depth_stencil_texture, DEPTHSTENCIL);   

    renderbuffer_build(&depth_stencil_texture);

    framebuffer_attachment framebuffer_depth_stencil_attachment = framebuffer_attachment_create();
    framebuffer_attachment_set_data_type(&framebuffer_depth_stencil_attachment, framebuffer_data_type_renderbuffer);
    framebuffer_attachment_set_attachment_type(&framebuffer_depth_stencil_attachment, depth_stencil_attachment);
    framebuffer_attachment_set_data(&framebuffer_depth_stencil_attachment, &depth_stencil_texture);
    framebuffer_add_attachment(&screen_framebuffer, &framebuffer_depth_stencil_attachment);

    framebuffer_build(&screen_framebuffer);

    struct triangle_vertex vertices[24] = {
        { -1.000f,  1.750f,     0.0556f, 1.0000f },
        { -1.045f,  1.745f,     0.0356f, 0.9986f },
        { -1.075f,  1.735f,     0.0222f, 0.9957f },
        { -1.110f,  1.700f,     0.0067f, 0.9857f },
        { -1.120f,  1.670f,     0.0022f, 0.9771f },
        { -1.125f,  1.625f,     0.0000f, 0.9643f },
        
        { -1.125f, -1.625f,     0.0000f, 0.0357f },
        { -1.120f, -1.670f,     0.0022f, 0.0223f },
        { -1.110f, -1.700f,     0.0067f, 0.0143f },
        { -1.075f, -1.735f,     0.0222f, 0.0043f },
        { -1.045f, -1.745f,     0.0356f, 0.0014f },
        { -1.009f, -1.750f,     0.0556f, 0.0000f },

        {  1.000f, -1.750f,     0.9444f, 0.0000f },
        {  1.045f, -1.745f,     0.9644f, 0.0014f },
        {  1.075f, -1.735f,     0.9778f, 0.0043f },
        {  1.110f, -1.700f,     0.9933f, 0.0143f },
        {  1.120f, -1.670f,     0.9978f, 0.0223f },
        {  1.125f, -1.625f,     1.0000f, 0.0357f },

        {  1.125f,  1.625f,     1.0000f, 0.9643f },
        {  1.120f,  1.670f,     0.9978f, 0.9771f },
        {  1.110f,  1.700f,     0.9933f, 0.9857f },
        {  1.075f,  1.735f,     0.9778f, 0.9957f },
        {  1.045f,  1.745f,     0.9644f, 0.9986f },
        {  1.000f,  1.750f,     0.9444f, 1.0000f }
    };
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

    vertex_array triangle = vertex_array_create();

    array_buffer triangle_positions_buffer = array_buffer_create(); 
    array_buffer_set_data(&triangle_positions_buffer, vertices);
    array_buffer_set_vertex_count(&triangle_positions_buffer, 24);
    array_buffer_set_draw_type(&triangle_positions_buffer, static_draw);   

    vertex_attribute positions_attribute = (vertex_attribute){ vertex_attribute_type_float, 2 };
    array_buffer_add_attribute(&triangle_positions_buffer, &positions_attribute);

    vertex_attribute uv_attribute = (vertex_attribute){ vertex_attribute_type_float, 2 };
    array_buffer_add_attribute(&triangle_positions_buffer, &uv_attribute);

    index_buffer triangle_index_buffer = index_buffer_create();
    index_buffer_set_data(&triangle_index_buffer, triangles);
    index_buffer_set_triangle_count(&triangle_index_buffer, 22);
    index_buffer_set_draw_type(&triangle_index_buffer, static_draw); 

    vertex_array_add_array_buffer(&triangle, &triangle_positions_buffer);
    vertex_array_set_index_buffer(&triangle, &triangle_index_buffer);

    vertex_array_build(&triangle);

    texture_2d cards_image = texture_2d_create();
    texture_2d_set_parameter(&cards_image, texture_2d_magnification_filter, texture_2d_filter_nearest);
    texture_2d_set_parameter(&cards_image, texture_2d_minification_filter, texture_2d_filter_nearest);

    texture_2d_set_parameter(&cards_image, texture_2d_wrap_x, texture_2d_wrap_repeat);
    texture_2d_set_parameter(&cards_image, texture_2d_wrap_y, texture_2d_wrap_repeat);

    texture_file cards_texture_file = texture_file_load_from_path("../res/images/Cards.png");
    texture_2d_set_width(&cards_image, cards_texture_file.width);
    texture_2d_set_height(&cards_image, cards_texture_file.height);

    texture_2d_set_color_mode(&cards_image, RGBA);
    texture_2d_set_data(&cards_image, cards_texture_file.pixel_data);

    texture_2d_build(&cards_image);

    clear_screen_data screen_clear = clear_screen_data_create(); 
    clear_screen_data_enable_layer(&screen_clear, color_layer);
    clear_screen_data_set_screen_color(&screen_clear, (color){ 150, 150, 150, 1.0f });

    card_renderer card_renderer = {};
    card_renderer_init(&card_renderer);

    while (window_is_open(&main_window)) {      
        framebuffer_bind(&screen_framebuffer);
        clear_screen(&screen_clear);

        texture_2d_bind(&cards_image, 0);
        card_renderer_draw_card(&card_renderer, (vector2){}, 0);

        vertex_array_bind(&triangle);
        vertex_array_draw(&triangle);

        framebuffer_bind(NULL);
        texture_2d_bind(&color_texture, 0);

        window_manager_update(&win_manager);
    }
    
    clear_screen_data_delete(&screen_clear);
    
    texture_2d_delete(&color_texture);
    renderbuffer_delete(&depth_stencil_texture);
    framebuffer_delete(&screen_framebuffer);
    
    card_renderer_cleanup(&card_renderer);
    vertex_array_delete(&triangle);
    texture_2d_delete(&cards_image);

    window_manager_delete(&win_manager);
    return 0;
}