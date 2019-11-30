#include "classes.h"
#include <iostream>

// void poker_dealer::draw_table(const card *flop_pointer){

// 	for (unsigned int i = 0; i < 3; i++){
// 		flop.push_back(flop_pointer[i]);
// 	}
// 	sort_vector(flop);
// 	turn = flop_pointer[3];
// 	river = flop_pointer[4];

// 	return;
// }

void poker_dealer::reset_round(){

	flop.clear();
	turn.value = 0;
	river.value = 0;

	return;
}

void poker_dealer::print_state() const{

	std::cout << "Flop is: ";
	for (unsigned int i = 0; i < 3; i++){
		std::cout << card_values[flop[i].value] << card_colour[flop[i].colour] << " ";
	}
	std::cout << std::endl << "Turn is: " << card_values[turn.value] << card_colour[turn.colour] << std::endl << "River is: " << card_values[river.value]<< card_colour[river.colour] << std::endl;
	return;
}