#include "classes.h"
#include <iostream>

// void poker_player::draw_hand(const card &first_card, const card &second_card){

// 	hand.push_back(first_card);
// 	hand.push_back(second_card);
// 	sort_vector(hand);
	
// 	return;
// }

void poker_player::reset_round(){

	hand.clear();
	high_cards.clear();
	hand_combination = 0;
	winner = false;

	return;
}

void poker_player::print_state() const{

	std::cout << std::endl << "Hand: ";
	for (unsigned int i = 0; i < 2; i++){
		std::cout << card_values[hand[i].value] << card_colour[hand[i].colour] << " ";
	}
	std::cout << std::endl;

	std::cout << "Best Combination: " << winning_combos[hand_combination] << std::endl << "High cards: ";

	for (unsigned int i = 0; i < high_cards.size(); i++){
		std::cout << card_values[high_cards[i]] << " ";
	}

	std::cout << "Winner? " << [](const poker_player *player){if (player->winner == true){return "Yes!";} else {return "No";}}(this) << std::endl << std::endl;

	return;
}