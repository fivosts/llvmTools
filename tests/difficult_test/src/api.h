#ifndef __API_H__
#define __API_H__
#include <map>
#include <vector>

class poker_player;
class poker_dealer;

struct card{

	unsigned int value;
	unsigned int colour;
};

extern std::map<unsigned int, std::string> card_values;
extern std::map<unsigned int, std::string> card_colour;
extern std::map<unsigned int, std::string> winning_combos;

void sort_vector(std::vector<card> &cards);
long long unsigned int calculate_card_vector_id(const std::vector<card> &cards);
std::vector<card> id_to_vector(long long unsigned int id, unsigned int stage);
void poker_comb_finder(poker_player &player, const poker_dealer &dealer);

#endif