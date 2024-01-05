#pragma once
#include "engine.h"
#include "game/card/hover/card_hover.h"
#include "game/card/card.h"
#include "game/card/renderer/card_renderer.h"

typedef struct card_manager {
    linked_list cards;
    card_hover* hover;
    card_renderer* renderer;
} card_manager;

void init_card_manager(card_manager* manager);
void create_cards(card_manager* manager);
void update_card_manager(card_manager* manager); 