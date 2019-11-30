#include "classes.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>

game::game(unsigned int players){

	std::srand(std::time(0));
	player_number = players;
	player_list = new poker_player[player_number];
	dealer = new poker_dealer;
	// std::cout << "The dealer and " << player_number << " players were created!" << std::endl;
	for (unsigned int colour = 0; colour < 4; colour++){
		for (unsigned int value = 0; value < 13; value++){
			deck[13 * colour + value].value = value;
			deck[13 * colour + value].colour = colour;
		}
	}
	// std::cout << "Deck initialized: " << std::endl;
	// print_deck();
}

void game::play_round(){

	//1. shuffle the deck. Done
	std::random_shuffle(std::begin(deck), std::end(deck));
	// print_deck();

	//2. give cards to the players
	unsigned int card_it = 0;
	for (unsigned int i = 0; i < player_number; i++){
		player_list[i].draw_hand(deck[card_it], deck[card_it + 1]);
		card_it += 2;
	}

	//3.show the flop, turn and river
	dealer->draw_table((card*)(&deck[card_it]));

	//4.find the winner and the losers
	for (unsigned int i = 0; i < player_number; i++){
		poker_comb_finder(player_list[i], *dealer);
	}
	
	unsigned int winning_hand = 0;
	std::vector<int> winning_highs;
	std::vector<int> temp_win;
	unsigned int winner = 0;

	for (unsigned int i = 0; i < player_number; i++){
		if (!winning_hand){
			winning_hand = player_list[i].get_hand_combo();
			winner = i;
			winning_highs = player_list[i].get_high_cards();
		}
		else if (player_list[i].get_hand_combo() < winning_hand){
			winning_hand = player_list[i].get_hand_combo();
			winner = i;
			winning_highs = player_list[i].get_high_cards();
		}
		else if (player_list[i].get_hand_combo() == winning_hand){
			
			temp_win = player_list[i].get_high_cards();

			for (unsigned int j = 0; j < temp_win.size(); j++){
			
				if (temp_win[j] > winning_highs[j]){
					winning_hand = player_list[i].get_hand_combo();
					winner = i;
					winning_highs = player_list[i].get_high_cards();
					break;
				}
				else if (temp_win[j] < winning_highs[j]){
					break;
				}
			}
		}
	}
	for (unsigned int pl = 0; pl < player_number; pl++){
		if (pl == winner){
			player_list[pl].set_win_state(true);
		}
		else{
			player_list[pl].set_win_state(false);
		}
	}

	// print_state();

	// std::cout << "Winner is: Player " << winner + 1 << " with winning hand: " << winning_combos[winning_hand] << std::endl;

	return;
}

void game::reset_round(){

	for (unsigned int i = 0; i < player_number; i++){
		player_list[i].reset_round();
	}
	dealer->reset_round();

	return;
}

void game::print_deck() const{

	std::cout << std::endl;
	for (unsigned int colour = 0; colour < 4; colour++){
		for (unsigned int value = 0; value < 13; value++){
			std::cout << card_values[deck[13 * colour + value].value] << card_colour[deck[13 * colour + value].colour] << " ";
		}
		std::cout << std::endl;
	}
	return;
}

void game::print_state() const{

	std::cout << std::endl << "Players: " << std::endl << std::endl;
	for (unsigned int i = 0; i < player_number; i++){
		std::cout << "Player " << (i + 1) << ": ";
		player_list[i].print_state();
	}
	std::cout << "Dealer: " << std::endl;
	dealer->print_state();

	return;
}