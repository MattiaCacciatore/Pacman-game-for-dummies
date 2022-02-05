/*
 *  Pacman game - IP course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2021-2022 - Italy
 *
 *  advanced_pacman_test.cpp
 *
 *  Copyright (c) 2022, Mattia Cacciatore <cacciatore1995@hotmail.it>
 *  All rights reserved.
 *
 *  Advanced pacman is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with micro_pacman.
 *  If not, see <http://www.gnu.org/licenses/>.
 */
#include "advanced_pacman.h"
#include "advanced_scoreboard_pacman.h"
/*--------------------------------GAME------------------------------------*/
bool pacman_game(){
	srand(static_cast<int>(time(NULL))); // Initialize random ghost direction and path.
	std::string maze_file = "Mazes/maze.cfg", d = ""; // Difficulty.
	const size_t p_pos_x = 1; // Pacman start position. Design specs.
	const size_t p_pos_y = 1;
	char c;
	
	for(;;){
		c = display_menu();
		if(c == '0'){
			std::cout << "\nSee you next time. Good bye!\n";
			return false;
		}
		else if(c == '1') break;
		else if(c == '2'){
			std::cout << "\nInsert the file name and play ENTER ---> ";
			std::cin >> maze_file;
			maze_file = "Mazes/" + maze_file;
			break;
		}
		else if(c == '3'){
			std::cout << "Select the difficulty level:\n"
				  << "1. Easy\n2. Normal\n3. Hard\n4. Nightmare\n"
				  << "Insert your level number and play ENTER ---> ";
			for(;;){
				std::cin >> d;
				if(d[0] == '1') setup_game(1);
				else if(d[0] == '2') setup_game(2);
				else if(d[0] == '3') setup_game(3);
				else if(d[0] == '4') setup_game(4);
				else{ 
					std::cout << "Invalid option, insert the correct level number ---> ";
					continue;
				}
				break;
			}
		}
		else if(c == '4') display_best_score();
	}
	
	pacman pac;
	pellet pel; // Power pellet.
	fruit fruits[fruits_n];
	ghost ghosts[ghosts_n];

	if(init_maze(maze_file) && init_pacman(pac, p_pos_x, p_pos_y)){
		char m; // Move.

		init_ghosts(ghosts, ghosts_n);
		init_fruits(fruits, fruits_n);
		init_pellet(pel);
		display(pac);
		for(;;){
			m = input();
			if(m == 'q') break;
			if(!move(m, pac, ghosts, ghosts_n)){
				display_endgame(pac.d_score >= total_dot_score);
				break;
			}
			update_pellet(pel, ghosts, ghosts_n); // Pellet has priority over fruits.
			update_fruits(fruits, fruits_n, ghosts, ghosts_n);
			respawn_ghosts(ghosts, ghosts_n);
			if(check_game_status(pac, ghosts, ghosts_n)){
				std::cout << "\nCHECK\n";
				break;
			}
			display(pac);
		}
		if(pac.d_score > 0) record_score(pac.score, pac.d_score, difficulty_level, maze_file);
		return true;
	}
	else{
		std::cerr << "Error with the initialization. Closing the game...\n";
		return false;
	}
}
/*--------------------------------TEST------------------------------------*/
int main(){
	std::string c = "";
	for(;;){
		if(!pacman_game()) break;
		std::cout << "\nAnother game? [y/n] ---> ";
		std::cin >> c;
		if(c[0] == 'n') break;
	}
	return 0;
}
