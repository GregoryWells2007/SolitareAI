#include "card_manager.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

void move_card_to_top(card_manager* manager, card* card_to_top) {
    int index = 0;
    for (int i = 0; i < 52; i++) {
        if (card_data_to_number(manager->cards[i]->data) == card_data_to_number(card_to_top->data)) {
            index = i;
            break;
        }
    }

    if (index == 0)
        return;

    card* card_to_copy = manager->cards[index];

    for (int i = index; i >= 0; i--)
        manager->cards[i] = manager->cards[i - 1];

    manager->cards[0] = card_to_copy;
}   

void assign_cards_to_rows(card_manager* manager) {
    int* numbersLeft = malloc(sizeof(int) * 52);
    for (int k = 0; k < 52; k++)
        numbersLeft[k] = k;

    int* numbers_to_use_list = malloc(sizeof(int) * 52);
    for (int k = 0; k < 52; k++) {
        srand((time(NULL))); 
        int value = rand() % (52 - k); 
        numbers_to_use_list[k] = numbersLeft[value];

        numbersLeft[value] = -1;
        for (int g = value; g < 51; g++) {
            numbersLeft[g] = numbersLeft[g + 1]; 
        }
    }
    free(numbersLeft);

    manager->row_1_cards = malloc(sizeof(card) * 13);
    for (int k = 1; k < 13; k++)
        manager->row_1_cards[k] = NULL;
    manager->row_1_cards[0] = manager->cards[numbers_to_use_list[0]];


    manager->row_2_cards = malloc(sizeof(card) * 13);
    for (int k = 2; k < 13; k++)
        manager->row_2_cards[k] = NULL;
    manager->row_2_cards[0] = manager->cards[numbers_to_use_list[1]];
    manager->row_2_cards[1] = manager->cards[numbers_to_use_list[2]];

    manager->row_3_cards = malloc(sizeof(card) * 13);
    for (int k = 3; k < 13; k++)
        manager->row_3_cards[k] = NULL;
    manager->row_3_cards[0] = manager->cards[numbers_to_use_list[3]];
    manager->row_3_cards[1] = manager->cards[numbers_to_use_list[4]];
    manager->row_3_cards[2] = manager->cards[numbers_to_use_list[5]];

    manager->row_4_cards = malloc(sizeof(card) * 13);
    for (int k = 4; k < 13; k++)
        manager->row_4_cards[k] = NULL;
    manager->row_4_cards[0] = manager->cards[numbers_to_use_list[6]];
    manager->row_4_cards[1] = manager->cards[numbers_to_use_list[7]];
    manager->row_4_cards[2] = manager->cards[numbers_to_use_list[8]];
    manager->row_4_cards[3] = manager->cards[numbers_to_use_list[9]];

    manager->row_5_cards = malloc(sizeof(card) * 13);
    for (int k = 5; k < 13; k++)
        manager->row_5_cards[k] = NULL;
    manager->row_5_cards[0] = manager->cards[numbers_to_use_list[10]];
    manager->row_5_cards[1] = manager->cards[numbers_to_use_list[11]];
    manager->row_5_cards[2] = manager->cards[numbers_to_use_list[12]];
    manager->row_5_cards[3] = manager->cards[numbers_to_use_list[13]];
    manager->row_5_cards[4] = manager->cards[numbers_to_use_list[14]];

    manager->row_6_cards = malloc(sizeof(card) * 13);
    for (int k = 6; k < 13; k++)
        manager->row_6_cards[k] = NULL;
    manager->row_6_cards[0] = manager->cards[numbers_to_use_list[15]];
    manager->row_6_cards[1] = manager->cards[numbers_to_use_list[16]];
    manager->row_6_cards[2] = manager->cards[numbers_to_use_list[17]];
    manager->row_6_cards[3] = manager->cards[numbers_to_use_list[18]];
    manager->row_6_cards[4] = manager->cards[numbers_to_use_list[19]];
    manager->row_6_cards[5] = manager->cards[numbers_to_use_list[20]];

    manager->row_7_cards = malloc(sizeof(card) * 13);
    for (int k = 7; k < 13; k++)
        manager->row_7_cards[k] = NULL;
    manager->row_7_cards[0] = manager->cards[numbers_to_use_list[21]];
    manager->row_7_cards[1] = manager->cards[numbers_to_use_list[22]];
    manager->row_7_cards[2] = manager->cards[numbers_to_use_list[23]];
    manager->row_7_cards[3] = manager->cards[numbers_to_use_list[24]];
    manager->row_7_cards[4] = manager->cards[numbers_to_use_list[25]];
    manager->row_7_cards[5] = manager->cards[numbers_to_use_list[26]];
    manager->row_7_cards[6] = manager->cards[numbers_to_use_list[27]];

    // assign stuff to the stack
    manager->cards_in_third_stack = 0;
    manager->rest_of_cards = malloc(sizeof(card) * 22);
    manager->card_stack_cards = malloc(sizeof(card) * 24);
    for (int i = 0; i < 24; i++) {
        manager->card_stack_cards[i] = manager->cards[numbers_to_use_list[i + 28]];
    }

    free(numbers_to_use_list);
}

void set_card_row_positions(card_manager* manager) {
    for (int i = 0; i < 13; i++) {
        if (manager->row_1_cards[i] != NULL) {
            manager->row_1_cards[i]->position = manager->loaded_board->board_cards_position_1;
            manager->row_1_cards[i]->position.y -= (i * 20);

            if (manager->row_1_cards[i + 1] == NULL)
                manager->row_1_cards[i]->flipped = false;
            else
                manager->row_1_cards[i]->flipped = true;
            move_card_to_top(manager, manager->row_1_cards[i]);
        }
        if (manager->row_2_cards[i] != NULL) {
            manager->row_2_cards[i]->position = manager->loaded_board->board_cards_position_2;
            manager->row_2_cards[i]->position.y -= (i * 20);

            if (manager->row_2_cards[i + 1] == NULL)
                manager->row_2_cards[i]->flipped = false;
            else
                manager->row_2_cards[i]->flipped = true;

            move_card_to_top(manager, manager->row_2_cards[i]);
        }
        if (manager->row_3_cards[i] != NULL) {
            manager->row_3_cards[i]->position = manager->loaded_board->board_cards_position_3;
            manager->row_3_cards[i]->position.y -= (i * 20);

            if (manager->row_3_cards[i + 1] == NULL)
                manager->row_3_cards[i]->flipped = false;
            else
                manager->row_3_cards[i]->flipped = true;
            
            move_card_to_top(manager, manager->row_3_cards[i]);
        }
        if (manager->row_4_cards[i] != NULL) {
            manager->row_4_cards[i]->position = manager->loaded_board->board_cards_position_4;
            manager->row_4_cards[i]->position.y -= (i * 20);

            if (manager->row_4_cards[i + 1] == NULL)
                manager->row_4_cards[i]->flipped = false;
            else
                manager->row_4_cards[i]->flipped = true;

            move_card_to_top(manager, manager->row_4_cards[i]);
        }
        if (manager->row_5_cards[i] != NULL) {
            manager->row_5_cards[i]->position = manager->loaded_board->board_cards_position_5;
            manager->row_5_cards[i]->position.y -= (i * 20);

            if (manager->row_5_cards[i + 1] == NULL)
                manager->row_5_cards[i]->flipped = false;
            else
                manager->row_5_cards[i]->flipped = true;

            move_card_to_top(manager, manager->row_5_cards[i]);
        }
        if (manager->row_6_cards[i] != NULL) {
            manager->row_6_cards[i]->position = manager->loaded_board->board_cards_position_6;
            manager->row_6_cards[i]->position.y -= (i * 20);

            if (manager->row_6_cards[i + 1] == NULL)
                manager->row_6_cards[i]->flipped = false;
            else
                manager->row_6_cards[i]->flipped = true;

            move_card_to_top(manager, manager->row_6_cards[i]);
        }
        if (manager->row_7_cards[i] != NULL) {
            manager->row_7_cards[i]->position = manager->loaded_board->board_cards_position_7;
            manager->row_7_cards[i]->position.y -= (i * 20);

            if (manager->row_7_cards[i + 1] == NULL)
                manager->row_7_cards[i]->flipped = false;
            else
                manager->row_7_cards[i]->flipped = true;

           move_card_to_top(manager, manager->row_7_cards[i]);
        }
    }   

    for (int i = 23; i >= 0; i--) {
        manager->card_stack_cards[i]->position = manager->loaded_board->card_stack_position;
        manager->card_stack_cards[i]->flipped = true;
        move_card_to_top(manager, manager->card_stack_cards[i]);
    }
}

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
            new_card->position = (vector2){ 0, 0 };
            new_card->scale = 40.0f;
            new_card->mouse_over = 0;
            new_card->held = 0;
            new_card->flipped = false;
            new_card->is_moveable = false;

            manager->cards[index] = new_card;
            index++;
        }
    }

    assign_cards_to_rows(manager);
    set_card_row_positions(manager);
}

void rotate_stack_array(card_manager* manager) {
    card* last_card = manager->card_stack_cards[0];
    for (int i = 0; i < 23; i++) {
        manager->card_stack_cards[i] = manager->card_stack_cards[i + 1];
    }
    manager->card_stack_cards[23] = last_card;
}

void check_stack_clicked (card_manager* manager) {
    card* first_card = manager->card_stack_cards[0];
    test_card_hover(manager->hover, first_card);

    if ((manager->hover->input_manager->mouse_clicked && first_card->mouse_over)) {
        if (manager->card_stack_show_1 == NULL) {
            manager->card_stack_show_1 = first_card;
        } else if (manager->card_stack_show_2 == NULL) {
            manager->card_stack_show_2 = manager->card_stack_show_1;
            manager->card_stack_show_1 = first_card;
        } else {
            manager->rest_of_cards[manager->cards_in_third_stack] = manager->card_stack_show_2;
            manager->card_stack_show_2 = manager->card_stack_show_1;
            manager->card_stack_show_1 = first_card;
            manager->cards_in_third_stack++;

        }

        first_card->flipped = false;
        move_card_to_top(manager, first_card);
        rotate_stack_array(manager);
    } else {
        if (manager->cards_in_third_stack == 22) {
            if (in_board_area(manager->hover, manager->loaded_board->card_stack_position) && manager->hover->input_manager->mouse_clicked) {    
                printf("resetting board\n");
                manager->card_stack_show_1 = NULL;
                manager->card_stack_show_2 = NULL;
                for (int k = 0; k < manager->cards_in_third_stack; k++) {
                    manager->rest_of_cards[k] = NULL;
                }
                manager->cards_in_third_stack = 0;
                for (int i = 0; i < 24; i++) {
                    manager->card_stack_cards[i]->flipped = true;
                }
            }
        }
    }

    first_card->mouse_over = false;
}

void position_card_stacks(card_manager* manager) {
    for (int i = 0; i < 24; i++) {
        manager->card_stack_cards[i]->position = manager->loaded_board->card_stack_position;
    }

    if (manager->card_stack_show_1 != NULL) {
        manager->card_stack_show_1->position.x = (manager->loaded_board->card_stack_position.x - 100.0f);
    }

    if (manager->card_stack_show_2 != NULL) {
        manager->card_stack_show_2->position.x = (manager->loaded_board->card_stack_position.x - 150.0f);
    }

    for (int k = 0; k < manager->cards_in_third_stack; k++) {
        manager->rest_of_cards[k]->position.x = (manager->loaded_board->card_stack_position.x - 200.0f);
    }
}

void update_card_manager(card_manager* manager) {
    bool card_already_hovered = false;

    check_stack_clicked(manager);
    position_card_stacks(manager);

    for (int i = 0; i < 52; i++) {
        card* current_card = manager->cards[i];
        current_card->mouse_over = 0;
        if (!card_already_hovered && !manager->card_is_currently_held) {
            test_card_hover(manager->hover, current_card);
            if (current_card->mouse_over)
                card_already_hovered = true;
        }

        if (current_card->is_moveable && !current_card->flipped) {
            if (!manager->card_is_currently_held) {
                current_card->held = 0;
                if (current_card->mouse_over && manager->hover->input_manager->mouse_down) {
                    current_card->held = 1;
                    manager->card_is_currently_held = true;

                    float real_mouse_x = (((manager->hover->input_manager->mouse_position.x / 1280.0f) * 2) - 1) * 640;
                    float real_mouse_y = -((((manager->hover->input_manager->mouse_position.y / 720.0f) * 2) - 1) * 360);

                    manager->card_held_offset.x = real_mouse_x - current_card->position.x;
                    manager->card_held_offset.y = real_mouse_y - current_card->position.y;

                    move_card_to_top(manager, current_card);
                }
            } else if (current_card->held) {
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
    }

    for (int i = 51; i >= 0; i--) {
        draw_card(manager->renderer, manager->cards[i]);
    }
}