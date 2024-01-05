#include "card_manager.h"

void create_cards(card_manager* manager) {
    manager->cards = malloc(sizeof(card) * 52);

    int index = 0;

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 13; y++) {
            card_data new_card_data = {};
            
            new_card_data.type = x;
            new_card_data.value = y;

            card* new_card = malloc(sizeof(card));
            new_card->data = new_card_data;
            new_card->position = (vector2){ (y * 95.0f) - 550.0f, (x * -145.0f) + 240.0f };
            new_card->scale = 40.0f;
            new_card->mouse_over = 0;
            new_card->held = 0;
            
            manager->cards[index] = new_card;
            index++;
        }
    }
}

void move_card_to_top(card_manager* manager, int index) {
    if (index == 0)
        return;

    card* card_to_copy = manager->cards[index];

    for (int i = index; i >= 0; i--)
        manager->cards[i] = manager->cards[i - 1];

    manager->cards[0] = card_to_copy;
}   

void update_card_manager(card_manager* manager) {
    bool card_already_hovered = false;

    for (int i = 0; i < 52; i++) {
        card* current_card = manager->cards[i];
        current_card->mouse_over = 0;
        if (!card_already_hovered && !manager->card_is_currently_held) {
            test_card_hover(manager->hover, current_card);
            if (current_card->mouse_over)
                card_already_hovered = true;
        }

        if (!manager->card_is_currently_held) {
            current_card->held = 0;
            if (current_card->mouse_over && manager->hover->input_manager->mouse_down) {
                current_card->held = 1;
                manager->card_is_currently_held = true;

                float real_mouse_x = (((manager->hover->input_manager->mouse_position.x / 1280.0f) * 2) - 1) * 640;
                float real_mouse_y = -((((manager->hover->input_manager->mouse_position.y / 720.0f) * 2) - 1) * 360);

                manager->card_held_offset.x = real_mouse_x - current_card->position.x;
                manager->card_held_offset.y = real_mouse_y - current_card->position.y;

                move_card_to_top(manager, i);
            }
        } else if (current_card->held) {
            //current_card->mouse_over = 1;
            if (manager->hover->input_manager->mouse_down) {
                float real_mouse_x = (((manager->hover->input_manager->mouse_position.x / 1280.0f) * 2) - 1) * 640;
                float real_mouse_y = -((((manager->hover->input_manager->mouse_position.y / 720.0f) * 2) - 1) * 360);

                real_mouse_x -= manager->card_held_offset.x;
                real_mouse_y -= manager->card_held_offset.y;

                current_card->position.x = real_mouse_x;
                current_card->position.y = real_mouse_y;
            } else {
                current_card->held = 0;
                manager->card_is_currently_held = false;
            }
        }
    }

    for (int i = 51; i >= 0; i--) {
        draw_card(manager->renderer, manager->cards[i]);
    }
}