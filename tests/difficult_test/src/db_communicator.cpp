#include "api.h"
#include "classes.h"
#include <iostream>
#include <sys/stat.h>
#include <string>
#include <sqlite3.h>

db_communicator::db_communicator(unsigned int players){

	//lets name the dbs -> poker_db_X.db

	struct stat buffer;
	db_name = "/home/fivosts/poker_db_" + std::to_string(players) + ".db";
	bool file_exists = (stat(db_name.c_str(), &buffer) == 0 ? true : false);

	// std::cout << "File exists? " << file_exists << std::endl;

	//if db does not exist, create it. Else just connect with it.

	if(!file_exists){
		if (sqlite3_open(db_name.c_str(), &db_conn) != SQLITE_OK){
			std::cout << sqlite3_errmsg(db_conn) << std::endl;
			exit(1);
		}
		else {
			std::cout << db_name << " created" << std::endl;
		}
		// if (!prepare_stage_write("CREATE TABLE HAND (ID integer, Hand text, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
		// 	sqlite3_close(db_conn);
		// 	exit(1);
		// }
		// if (!prepare_stage_write("CREATE TABLE FLOP (ID integer, Hand text, Table_cards text, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
		// 	sqlite3_close(db_conn);
		// 	exit(1);
		// }
		// if (!prepare_stage_write("CREATE TABLE TURN (ID integer, Hand text, Table_cards text, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
		// 	sqlite3_close(db_conn);
		// 	exit(1);
		// }
		// if (!prepare_stage_write("CREATE TABLE RIVER (ID integer, Hand text, Table_cards text, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
		// 	sqlite3_close(db_conn);
		// 	exit(1);
		// }
		if (!prepare_stage_write("CREATE TABLE HAND (ID integer, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
			sqlite3_close(db_conn);
			exit(1);
		}
		if (!prepare_stage_write("CREATE TABLE FLOP (ID integer, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
			sqlite3_close(db_conn);
			exit(1);
		}
		if (!prepare_stage_write("CREATE TABLE TURN (ID integer, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
			sqlite3_close(db_conn);
			exit(1);
		}
		if (!prepare_stage_write("CREATE TABLE RIVER (ID integer, Encounters integer, Wins integer, PRIMARY KEY (ID))")){
			sqlite3_close(db_conn);
			exit(1);
		}
	}
	else{
		if (sqlite3_open(db_name.c_str(), &db_conn) != SQLITE_OK){
			std::cout << sqlite3_errmsg(db_conn) << std::endl;
			sqlite3_close(db_conn);
			exit(1);
		}
		else{
			std::cout << db_name << " connected successfully" << std::endl;
		}	
	}
}

db_communicator::~db_communicator(){

	int rc = sqlite3_close(db_conn);
	if (rc != SQLITE_OK){
		std::cout << "Error closing the database: " << rc << " " << sqlite3_errstr(rc) << std::endl;
	}
	std::cout << db_name << " disconnected successfully" << std::endl;
}

bool db_communicator::prepare_stage_write(const char *command){

	sqlite3_prepare_v2(db_conn, command, -1, &db_stmt, NULL);

	unsigned int status = sqlite3_step(db_stmt);
	
	while (!((status == SQLITE_DONE) || (status == SQLITE_OK))){
		switch(status){
			case SQLITE_BUSY:
				status = sqlite3_step(db_stmt);
				break;
			case SQLITE_DONE:
				break;
			case SQLITE_OK:
				break;
			default:{
				std::cout << "Error code on write staging: " << status << ": " << sqlite3_errstr(status) << std::endl;
				sqlite3_reset(db_stmt);
				return false;
				break;
			}
		}
	}
	if (sqlite3_finalize(db_stmt) != SQLITE_OK){
		std::cout << "Error code on finalizing: " << status << ": " << sqlite3_errstr(status) << std::endl;
		sqlite3_reset(db_stmt);
	}

	return true;
}

const unsigned char *db_communicator::prepare_stage_read(const char *command){

	const unsigned char *query;
	sqlite3_prepare_v2(db_conn, command, -1, &db_stmt, NULL);

	unsigned int status = sqlite3_step(db_stmt);

	while(!((status == SQLITE_DONE) || (status == SQLITE_OK) || (status == SQLITE_ROW))){
		switch(status){
			case SQLITE_BUSY:
				status = sqlite3_step(db_stmt);
				break;
			default:{
				std::cout << "Error code on read staging: " << status << ": " << sqlite3_errstr(status) << std::endl;
				sqlite3_reset(db_stmt);
				return (const unsigned char*)"error";
			}
		}
	}

	query = sqlite3_column_text(db_stmt, 1);

	if (sqlite3_finalize(db_stmt) != SQLITE_OK){
		std::cout << "Error code on finalizing: " << status << ": " << sqlite3_errstr(status) << std::endl;
		sqlite3_reset(db_stmt);
	}

	return query;
}

void db_communicator::store_to_db(const game * const &poker_game){

	//find a way to calculate IDs

	std::vector<card> hand_cards;
	std::vector<card> table_cards;
	std::vector<card> concatenated_cards;

	std::string command;
	long long unsigned int id = 0;
	const unsigned char *query;

	for (unsigned int i = 0; i < poker_game->player_number; i++){

		//store to hand db
		hand_cards = poker_game->player_list[i].get_hand();
		id = calculate_card_vector_id(hand_cards);
		// command = "SELECT * FROM HAND WHERE ID = " + std::to_string(id);
		// query = prepare_stage_read(command.c_str());

		// if (query == (const unsigned char*)"error"){
		if (false){
			sqlite3_close(db_conn);
			std::cout << "something went wrong" << std::endl;
			exit(1);
		}
		else{
			// if (query == 0){ //new entry

			// 	command = "INSERT INTO HAND (ID, Encounters, Wins) VALUES ("
			// 					+ std::to_string(id) + ", "
			// 					+ "1, "
			// 					+ std::to_string(poker_game->player_list[i].is_winner()) + ")";
			// 	// std::cout << command <<std::endl;
			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
			// else{ //update old entry
			// 	// std::cout << "Entry already exists " << (const char*)query << std::endl;

			// 	command = "UPDATE HAND SET Encounters = Encounters + 1, Wins = Wins + "
			// 				+ std::to_string(poker_game->player_list[i].is_winner());
			// 	// std::cout << command <<std::endl;
			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
		}

		//store to flop db
		table_cards = poker_game->dealer->get_flop();
		// concatenated_cards = hand_cards.insert(hand_cards.end(), table_cards.begin(), table_cards.end());
		concatenated_cards = hand_cards;
		concatenated_cards.insert(concatenated_cards.end(), table_cards.begin(), table_cards.end());
		id = calculate_card_vector_id(concatenated_cards);
		command = "SELECT * FROM FLOP WHERE ID = " + std::to_string(id);
		query = prepare_stage_read(command.c_str());

		if (query == (const unsigned char*)"error"){
			sqlite3_close(db_conn);
			std::cout << "something went wrong" << std::endl;
			exit(1);
		}
		else{
			// if (query == 0){ //new entry

			// 	command = "INSERT INTO FLOP (ID, Hand, Table_cards, Encounters, Wins) VALUES ("
			// 					+ std::to_string(id) + ", '"
			// 					+ [](const std::vector<card> &player){
			// 						std::string hand_card;
			// 						for (std::vector<card>::const_iterator it = player.begin(); it != player.end(); it++){
			// 							hand_card += card_values[(*it).value] + card_colour[(*it).colour];
			// 						}
			// 						return hand_card;
			// 					}(hand_cards) + "', '"
			// 					+ [](const std::vector<card> &flop){
			// 						std::string hand_card;
			// 						for (std::vector<card>::const_iterator it = flop.begin(); it != flop.end(); it++){
			// 							hand_card += card_values[(*it).value] + card_colour[(*it).colour];
			// 						}
			// 						return hand_card;
			// 					}(table_cards) + "', "
			// 					+ "1, "
			// 					+ std::to_string(poker_game->player_list[i].is_winner()) + ")";
			// 	// std::cout << command <<std::endl;
			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
			// else{ //update old entry
			// 	// std::cout << "Entry already exists " << (const char*)query << std::endl;

			// 	command = "UPDATE FLOP SET Encounters = Encounters + 1, Wins = Wins + "
			// 				+ std::to_string(poker_game->player_list[i].is_winner());
			// 	// std::cout << command <<std::endl;
			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
			if (query == 0){ //new entry

				command = "INSERT INTO FLOP (ID, Encounters, Wins) VALUES ("
								+ std::to_string(id) + ", "
								+ "1, "
								+ std::to_string(poker_game->player_list[i].is_winner()) + ")";
				// std::cout << command <<std::endl;
				if (!prepare_stage_write(command.c_str())){
					sqlite3_close(db_conn);
					exit(1);
				}
			}
			else{ //update old entry
				// std::cout << "Entry already exists " << (const char*)query << std::endl;

				command = "UPDATE FLOP SET Encounters = Encounters + 1, Wins = Wins + "
							+ std::to_string(poker_game->player_list[i].is_winner());
				// std::cout << command <<std::endl;
				if (!prepare_stage_write(command.c_str())){
					sqlite3_close(db_conn);
					exit(1);
				}
			}
		}
					

		//store to turn db
		table_cards.push_back(poker_game->dealer->get_turn());
		sort_vector(table_cards);


		concatenated_cards.erase(concatenated_cards.begin() + 2, concatenated_cards.end());
		concatenated_cards.insert(concatenated_cards.end(), table_cards.begin(), table_cards.end());		

		id = calculate_card_vector_id(concatenated_cards);

		command = "SELECT * FROM TURN WHERE ID = " + std::to_string(id);
		query = prepare_stage_read(command.c_str());

		if (query == (const unsigned char*)"error"){
			sqlite3_close(db_conn);
			std::cout << "something went wrong" << std::endl;
			exit(1);
		}
		else{
			// if (query == 0){ //new entry

			// 	command = "INSERT INTO TURN (ID, Hand, Table_cards, Encounters, Wins) VALUES ("
			// 					+ std::to_string(id) + ", '"
			// 					+ [](const std::vector<card> &player){
			// 						std::string hand_card;
			// 						for (std::vector<card>::const_iterator it = player.begin(); it != player.end(); it++){
			// 							hand_card += card_values[(*it).value] + card_colour[(*it).colour];
			// 						}
			// 						return hand_card;
			// 					}(hand_cards) + "', '"
			// 					+ [](const std::vector<card> &flop){
			// 						std::string hand_card;
			// 						for (std::vector<card>::const_iterator it = flop.begin(); it != flop.end(); it++){
			// 							hand_card += card_values[(*it).value] + card_colour[(*it).colour];
			// 						}
			// 						return hand_card;
			// 					}(table_cards) + "', "
			// 					+ "1, "
			// 					+ std::to_string(poker_game->player_list[i].is_winner()) + ")";

			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
			// else{ //update old entry
			// 	// std::cout << "Entry already exists " << (const char*)query << std::endl;

			// 	command = "UPDATE TURN SET Encounters = Encounters + 1, Wins = Wins + "
			// 				+ std::to_string(poker_game->player_list[i].is_winner());
			// 	// std::cout << command <<std::endl;
			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
			if (query == 0){ //new entry

				command = "INSERT INTO TURN (ID, Encounters, Wins) VALUES ("
								+ std::to_string(id) + ", "
								+ "1, "
								+ std::to_string(poker_game->player_list[i].is_winner()) + ")";

				if (!prepare_stage_write(command.c_str())){
					sqlite3_close(db_conn);
					exit(1);
				}
			}
			else{ //update old entry
				// std::cout << "Entry already exists " << (const char*)query << std::endl;

				command = "UPDATE TURN SET Encounters = Encounters + 1, Wins = Wins + "
							+ std::to_string(poker_game->player_list[i].is_winner());
				// std::cout << command <<std::endl;
				if (!prepare_stage_write(command.c_str())){
					sqlite3_close(db_conn);
					exit(1);
				}
			}
		}

		//store to river db
		table_cards.push_back(poker_game->dealer->get_river());
		sort_vector(table_cards);


		concatenated_cards.erase(concatenated_cards.begin() + 2, concatenated_cards.end());
		concatenated_cards.insert(concatenated_cards.end(), table_cards.begin(), table_cards.end());		

		id = calculate_card_vector_id(concatenated_cards);

		command = "SELECT * FROM RIVER WHERE ID = " + std::to_string(id);
		query = prepare_stage_read(command.c_str());

		if (query == (const unsigned char*)"error"){
			sqlite3_close(db_conn);
			std::cout << "something went wrong" << std::endl;
			exit(1);
		}
		else{
			// if (query == 0){ //new entry

			// 	command = "INSERT INTO RIVER (ID, Hand, Table_cards, Encounters, Wins) VALUES ("
			// 					+ std::to_string(id) + ", '"
			// 					+ [](const std::vector<card> &player){
			// 						std::string hand_card;
			// 						for (std::vector<card>::const_iterator it = player.begin(); it != player.end(); it++){
			// 							hand_card += card_values[(*it).value] + card_colour[(*it).colour];
			// 						}
			// 						return hand_card;
			// 					}(hand_cards) + "', '"
			// 					+ [](const std::vector<card> &flop){
			// 						std::string hand_card;
			// 						for (std::vector<card>::const_iterator it = flop.begin(); it != flop.end(); it++){
			// 							hand_card += card_values[(*it).value] + card_colour[(*it).colour];
			// 						}
			// 						return hand_card;
			// 					}(table_cards) + "', "
			// 					+ "1, "
			// 					+ std::to_string(poker_game->player_list[i].is_winner()) + ")";

			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
			// else{ //update old entry
			// 	// std::cout << "Entry already exists " << (const char*)query << std::endl;

			// 	command = "UPDATE RIVER SET Encounters = Encounters + 1, Wins = Wins + "
			// 				+ std::to_string(poker_game->player_list[i].is_winner());
			// 	// std::cout << command <<std::endl;
			// 	if (!prepare_stage_write(command.c_str())){
			// 		sqlite3_close(db_conn);
			// 		exit(1);
			// 	}
			// }
			if (query == 0){ //new entry

				command = "INSERT INTO RIVER (ID, Encounters, Wins) VALUES ("
								+ std::to_string(id) + ", "
								+ "1, "
								+ std::to_string(poker_game->player_list[i].is_winner()) + ")";

				if (!prepare_stage_write(command.c_str())){
					sqlite3_close(db_conn);
					exit(1);
				}
			}
			else{ //update old entry

				command = "UPDATE RIVER SET Encounters = Encounters + 1, Wins = Wins + "
							+ std::to_string(poker_game->player_list[i].is_winner());
				if (!prepare_stage_write(command.c_str())){
					sqlite3_close(db_conn);
					exit(1);
				}
			}
		}

	}

	return;
}