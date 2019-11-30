#include "api.h"
#include "classes.h"
#include <map>
#include <vector>
#include <iostream>
#include <iterator>
#include <math.h>

std::map<unsigned int, std::string> card_values{
	{0, "2"}, {1, "3"}, {2, "4"}, {3, "5"},
	{4, "6"}, {5, "7"}, {6, "8"}, {7, "9"},
	{8, "10"}, {9, "J"}, {10, "Q"}, {11, "K"},
	{12, "A"}
};

std::map<unsigned int, std::string> card_colour{ //0: mpastoynia, 1:trifili, 2: koupes, 3: karo
	{0, "\u2660"}, {1, "\u2663"},
	{2, "\u2665"}, {3, "\u2666"}
};

std::map<unsigned int, std::string> winning_combos{
	{0, "Nothing yet"}, {1, "Royal Flush"}, {2, "Straight Flush"},
	{3, "Four of a Kind"}, {4, "Full House"}, {5, "Flush"},
	{6, "Straight"}, {7, "Three of a Kind"}, {8, "Two pair"},
	{9, "One pair"}, {10, "High card"}
};

void sort_vector(std::vector<card> &cards){

	card temp;

	for (unsigned int i = 0; i < cards.size() - 1; i++){
		for (unsigned int j = 0; j < cards.size() - (i + 1); j++){
			if ((cards[j].value > cards[j + 1].value) || ((cards[j].value == cards[j + 1].value) && (cards[j].colour > cards[j + 1].colour)) ){
				temp = cards[j];
				cards[j] = cards[j + 1];
				cards[j + 1] = temp;
			}
		}
	}
	return;
}

long long unsigned int calculate_card_vector_id(const std::vector<card> &cards){

	long long unsigned int id = 0;
	for (std::vector<card>::const_iterator it = cards.end() - 1; it != cards.begin() - 1; it--){

		id += ( (*it).colour * 13 + (*it).value ) * pow(52, (cards.end() - it - 1));
	}

	return id;
}

std::vector<card> id_to_vector(long long unsigned int id, unsigned int stage){

	std::vector<card> cur_card_v;
	card cur_card;
	long long unsigned int div;

	for(unsigned int i = 0; i < stage; i++){

		div = (id / pow(52, stage - 1 - i));
		cur_card.colour = div / 13;
		cur_card.value = div % 13;
		id = id - (div * pow(52, stage - 1 - i));
		cur_card_v.push_back(cur_card);
	}

	return cur_card_v;
}

void poker_comb_finder(poker_player &player, const poker_dealer &dealer){

	std::vector<card> total_hand;

	total_hand.push_back(player.hand[0]);
	total_hand.push_back(player.hand[1]);

	total_hand.push_back(dealer.flop[0]);
	total_hand.push_back(dealer.flop[1]);
	total_hand.push_back(dealer.flop[2]);

	total_hand.push_back(dealer.turn);

	total_hand.push_back(dealer.river);

	sort_vector(total_hand);
	std::vector<card> temp = total_hand;

	//look for straights
	unsigned int cons_counter = 0;
	// bool straight = false;
	for (unsigned int i = 0; i < temp.size() - 1; i++){
		if (temp[i + 1].value == temp[i].value + 1){
			cons_counter++;
		}
		else{
			cons_counter = 0;
		}
		if (cons_counter == 4){
			(player.high_cards).push_back(temp[i + 1].value);
			// straight = true;
			if ((temp[i + 1].colour == temp[i].colour) && (temp[i].colour == temp[i - 1].colour) && (temp[i - 1].colour == temp[i - 2].colour) && (temp[i - 2].colour == temp[i - 3].colour)){
				if (temp[i + 1].value == 12){
					player.hand_combination = 1;
				}
				else{
					player.hand_combination = 2;
				}
			}
			else{
				player.hand_combination = 6;
			}
			temp.erase(temp.begin() + (i - 3), temp.begin() + (i + 2));
			for (int i = temp.size() - 1; i >= 0; i--){
				player.high_cards.push_back(temp[i].value);
			}
			return;
		}
	}

	//look for flush
	unsigned int *colour_freq = new unsigned int[4];
	for (unsigned int i = 0; i < 4; i++){
		colour_freq[i] = 0;
	}
	for (unsigned int i = 0; i < temp.size(); i++){
		colour_freq[temp[i].colour]++;
	}
	for (unsigned int i = 0; i < 4; i++){
		if (colour_freq[i] >= 5){
			player.hand_combination = 5;
			for (int i = temp.size() - 1; i >= 0; i--){
				player.high_cards.push_back(temp[i].value);
			}
			delete [] colour_freq;
			return;
		}
	}
	delete [] colour_freq;

	//look for quads, full houses and three of a kind
	unsigned int *card_freq = new unsigned int[13];
	for (unsigned int i = 0; i < 13; i++){
		card_freq[i] = 0;
	}
	for (unsigned int i = 0; i < temp.size(); i++){
		card_freq[temp[i].value]++;
	}
	for (unsigned int i = 0; i < 13; i++){
		switch(card_freq[i]){
			case 4:{
				player.hand_combination = 3;
				for (unsigned int j = 0; j < temp.size(); j++){
					if (temp[j].value == i){
						temp.erase(temp.begin() + j);
						j--;
					}
				}
				player.high_cards.push_back(i);
				for (int j = temp.size() - 1; j >= 0; j--){
					player.high_cards.push_back(temp[j].value);
				}
				return;
			}
			case 3:{
				player.hand_combination = 7;
				for (unsigned int j = 0; j < temp.size(); j++){
					if (temp[j].value == i){
						temp.erase(temp.begin() + j);
						j--;
					}
				}
				player.high_cards.push_back(i);
				for (unsigned int k = 0; k < 13; k++){
					if (card_freq[k] == 2){
						player.hand_combination = 4;
					}
				}
				for (int j = temp.size() - 1; j >= 0; j--){
					player.high_cards.push_back(temp[j].value);
				}
				return;
			}
			default:
				break;
		}
	}

	//search for 2-pairs
	int first_pair = -1;
	int second_pair = -1;

	for (int i = 0; i < 13; i++){
		if (card_freq[i] == 2){
			if ((first_pair == -1) || ((first_pair < second_pair) && (i > first_pair))){
				first_pair = i;
			}
			else if ((second_pair == -1) || ((first_pair > second_pair) && (i > second_pair))){
				second_pair = i;
			}
		}
	}
	if ((first_pair != -1) && (second_pair == -1)){
		player.hand_combination = 9;
		for (unsigned int j = 0; j < temp.size(); j++){
			if (temp[j].value == first_pair){
				temp.erase(temp.begin() + j);
				j--;
			}
		}
		player.high_cards.push_back(first_pair);
		for (int j = temp.size() - 1; j >= 0; j--){
			player.high_cards.push_back(temp[j].value);
		}
	}
	else if ((first_pair != -1) && (second_pair != -1)){
		player.hand_combination = 8;

		for (unsigned int j = 0; j < temp.size(); j++){
			if ((temp[j].value == first_pair) || (temp[j].value == second_pair)){
				temp.erase(temp.begin() + j);
				j--;
			}
		}
		if (first_pair > second_pair){
			player.high_cards.push_back(first_pair);
			player.high_cards.push_back(second_pair);
		}
		else{
			player.high_cards.push_back(second_pair);
			player.high_cards.push_back(first_pair);			
		}
		for (int j = temp.size() - 1; j >= 0; j--){
			player.high_cards.push_back(temp[j].value);
		}
	}
	else if ((first_pair == -1) && (second_pair == -1)){
		player.hand_combination = 10;
		for (int j = temp.size() - 1; j >= 0; j--){
			player.high_cards.push_back(temp[j].value);
		}		
	}
	return;
}