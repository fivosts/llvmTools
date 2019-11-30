#include "api.h"
#include "classes.h"
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <chrono>
#include <signal.h>
#include <sys/mman.h>
#include <memory>
#include <cstring>

// #define BENCHMARK_IT 100

volatile sig_atomic_t flag = 0;
static unsigned int db_counter = 2;
extern std::map<unsigned int, std::string> card_values;

void child_ppid_alarm(int sig){

    std::array<char, 128> buffer;
    std::vector<std::string> pts_out_v;

    std::shared_ptr<FILE> pipe(popen("ls /dev/pts", "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr){
			if (strcmp(buffer.data(), "ptmx\n") != 0){
	            pts_out_v.push_back(strtok(buffer.data(), "\n"));
	        }
        }
    }

	std::string shell_str = "Daemon process PID: " + std::to_string(getppid());
	const char *shell_var = shell_str.c_str();
	const char *shell_pts;
	setenv("shellvar", shell_var, 1);

	for (std::vector<std::string>::iterator it = pts_out_v.begin(); it != pts_out_v.end(); it++){
		
		shell_pts = (*it).c_str();
		setenv("shellpts", shell_pts, 1);
		system("echo $shellvar > /dev/pts/$shellpts");
	}
	return;
}

void parent_id_alarm(int sig){

    std::array<char, 128> buffer;
    std::vector<std::string> pts_out_v;

    std::shared_ptr<FILE> pipe(popen("ls /dev/pts", "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr){
			if (strcmp(buffer.data(), "ptmx\n") != 0){
	            pts_out_v.push_back(strtok(buffer.data(), "\n"));
	        }
        }
    }

	std::string shell_str = "Daemon process PID: " + std::to_string(getpid());
	const char *shell_var = shell_str.c_str();
	const char *shell_pts;
	setenv("shellvar", shell_var, 1);

	for (std::vector<std::string>::iterator it = pts_out_v.begin(); it != pts_out_v.end(); it++){
		
		shell_pts = (*it).c_str();
		setenv("shellpts", shell_pts, 1);
		system("echo $shellvar > /dev/pts/$shellpts");
	}
	return;
}

void signal_catcher(int sig){

	flag = 1;
	return;
}

void train_db(unsigned int players){

	if(flag){ 
		std::cout << "Interrupt raised" << std::endl;
		return;
	}

	db_communicator *database = new db_communicator(players);
	game *poker_game = new game(players);

	// while(!flag){

		poker_game->play_round();
		database->store_to_db(poker_game);
		poker_game->reset_round();    	


	// }
	delete database;
	return;
}

void training_process_daemon(unsigned int* &pid){

	// int p = fork();

	// if (p == -1){ perror("Fork failure: "); }

	// else if (p == 0){

		signal(SIGUSR1, child_ppid_alarm);	
		train_db(db_counter);
		pid[db_counter - 2] = 0;
	// }

	// else{

	// 	pid[db_counter - 2] = p;
	// 	db_counter++;
	// 	if ((db_counter < 11) && (!flag)){
	// 		usleep(300000);
	// 		training_process_daemon(pid);
	// 	}
	// 	signal(SIGUSR1, parent_id_alarm);
	// }

	return;
}

// int main(int argc, char** argv){
int main(){

    // std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	sigset_t signal_set;
	int sig;

	// sigemptyset(&signal_set);
	// sigaddset(&signal_set, SIGINT);
	// signal(SIGINT, signal_catcher);

	unsigned int *pid_list = (unsigned int*)mmap(NULL, 9 * sizeof(unsigned int), PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);

	training_process_daemon(pid_list);

	// sleep(1);

	std::cout << "Opened successfully connection with  " << db_counter - 2 << " databases" << std::endl;

	// sigwait(&signal_set, &sig);

	// for(unsigned int i = 0; i < db_counter; i++){
	// 	kill(pid_list[i], 2);
	// }

	std::cout << std::endl << "Waiting for processes to terminate..." << std::endl;

	// while([](unsigned int* &pid_list, unsigned int &db_counter){
	// 	unsigned int sum = 0;
	// 	for (unsigned int i = 0; i < db_counter; i++){
	// 		sum += pid_list[i];
	// 	}
	// 	return sum;
	// }(pid_list, db_counter)){}

	std::cout << "Program terminated successfully" << std::endl;


	// for(unsigned int j = 0; j < 2651; j++){
	// 	std::vector<card> card_v = id_to_vector(j + 1, 2);

	// 	for (unsigned int i = 0; i < card_v.size(); i++){
	// 		std::cout << card_values[card_v[i].value] << card_colour[card_v[i].colour] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
    // std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    // std::cout << duration << std::endl;
    
	return 0;
}