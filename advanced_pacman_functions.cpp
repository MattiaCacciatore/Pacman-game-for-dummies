/*
 *  Pacman game - IP course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2021-2022 - Italy
 *
 *  advanced_pacman_functions.cpp
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
#include "advanced_color_pacman.h"

#ifdef _WIN32 // Windows operative system.
	#include <windows.h>
	#include <conio.h> // C header file used mostly by MS-DOS compilers to provide console input/output.
#endif                 // It's not in the C standard library or ISO C, nor is it defined by POSIX.

MAZE_ELEMS maze[WALL_SIZE][WALL_SIZE]; // Global maze.

// 1 < rpc < 100. 1 almost no random (same direction), 100 completely random.
static unsigned int random_path_chance = 50;
static int invulnerability             = 20; // Pacman's invulnerability, it must be positive.
static const int dot_score             = 10;
static const int fruit_score           = 300;
static const int pellet_score          = 1000;
static int ttl_fruit                   = 20; // Fruit time to live. It must be positive.
static int ttl_pellet                  = 20; // Pellet time to live. It must be positive.
static int respawn_count_down          = 15; // How many moves are required to respawn a dead ghost.
/*---------------------------------------------------------------------------------*/
// HELPER FUNCTIONS
/*---------------------------------------------------------------------------------*/
// It generates a random direction.
DIR rand_dir(){
	const int dir_num = 4; // Number of directions.
	return static_cast<DIR>(rand() % dir_num);
}
//---------------------------------------------------------------------------------
// It generates 2 random legit coordinates (one position) in the maze for pacman and ghosts.
void gen_rand_coord(size_t &x_c, size_t &y_c){
	size_t x, y;
	for(;;){ // (size_t i = 0; i < (SIZE * SIZE); ++i)
		x = static_cast<size_t>(rand() % MAZE_SIZE);
		y = static_cast<size_t>(rand() % MAZE_SIZE);
		if(maze[y][x] == DOT || maze[y][x] == EMPTY){
			x_c = x;
			y_c = y;
			break;
		}
	}
}
//---------------------------------------------------------------------------------
// It transforms the pacman direction into its representation in the maze.
MAZE_ELEMS pacman_sprite(const DIR pac_dir){
	if(pac_dir == NORTH) return PCM_N;
	else if(pac_dir == WEST) return PCM_W;
	else if(pac_dir == SOUTH) return PCM_S;
	else if(pac_dir == EAST) return PCM_E;
	else return EMPTY;
}
//---------------------------------------------------------------------------------
// It removes and kill the ghost from the maze. Only pacman can kill a ghost.
void kill_ghost(ghost &g, const DIR pac_dir){
	maze[g.y][g.x] = pacman_sprite(pac_dir);
	g.x = 0;
	g.y = 0;
	g.rspwn_cntdown = respawn_count_down;
	g.prev_elem = EMPTY;
}
//---------------------------------------------------------------------------------
// It update the current pacman score and invulnerability status.
void update_pacman(MAZE_ELEMS elem, pacman &p, ghost ghosts[], const size_t size_g){
	if(elem == GHOST){ // Possible collision.
		for(size_t i = 0; i < size_g; ++i){
			if(ghosts[i].x == p.x && ghosts[i].y == p.y){
				if(ghosts[i].prev_elem != PELLET) // In order to prevent illegal immortality.
					elem = ghosts[i].prev_elem;
				else
					elem = EMPTY;

				ghosts[i].prev_elem = pacman_sprite(p.dir);
				break;
			}
		}
	}

	if(elem == DOT){
		p.score += dot_score;
		p.d_score++;
	}
	else if(elem == FRUIT) p.score += fruit_score;
	else if(elem == PELLET){
		p.score += pellet_score;
		p.inv_ttl = invulnerability;
	}
}
//---------------------------------------------------------------------------------
// It checks if a position is legit in the maze (No walls in the range (1, MAZE_SIZE)).
bool check_pos(const size_t x, const size_t y){
	return (x > 0 && x <= MAZE_SIZE && y > 0 && y <= MAZE_SIZE && maze[y][x] != WALL);
}
//---------------------------------------------------------------------------------
// It checks if a position is legit in the maze (No walls or ghosts in the range (1, MAZE_SIZE)).
bool check_ghost_pos(const size_t x, const size_t y){
	return (check_pos(x,y) && maze[y][x] != GHOST);
}
//---------------------------------------------------------------------------------
// It checks if a position is legit in the maze (No pacman (1,MAZE_SIZE)).
bool check_pacman_pos(const size_t x, const size_t y){
	return (maze[y][x] != PCM_N && maze[y][x] != PCM_W && maze[y][x] != PCM_S && maze[y][x] != PCM_E);
}
//---------------------------------------------------------------------------------
// It checks if a position is legit in the maze (No walls, ghosts or pacman in the range (1,MAZE_SIZE)).
bool check_entities_pos(const size_t x, const size_t y){
	return (check_ghost_pos(x,y) && check_pacman_pos(x,y));
}
//---------------------------------------------------------------------------------
// It checks if pacman has been killed by a ghost, in that case is GAME OVER.
bool check_pacman_status(pacman &p, ghost ghosts[], const size_t size_g){
	for(size_t i = 0; i < size_g; ++i){
		if(ghosts[i].x == p.x && ghosts[i].y == p.y){ // Collision with ghost detected.
			if(p.inv_ttl > 0){
				// Killing the ghost because invulnerability status. Pacman survives.
				update_pacman(ghosts[i].prev_elem, p, ghosts, size_g);
				kill_ghost(ghosts[i], p.dir);
				return false;
			}
			else{
				maze[p.y][p.x] = GHOST; // Pacman dead.
				return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------------
// It builds the maze with walls.
void fill_maze(){
	for(size_t y = 0; y < WALL_SIZE; ++y){
		for(size_t x = 0; x < WALL_SIZE; ++x){ maze[y][x] = WALL;}
	}
}
//---------------------------------------------------------------------------------
// It prints on stdout the current pacman score.
void display_score(const pacman &p){
	std::cout << "\n***** PACMAN SCORE *****\n\n" << p.score << " points\n"
		  << p.d_score << " dot(s)\n\n";
	if(p.inv_ttl > 0) std::cout << "PACMAN IS INVULNERABLE FOR " << p.inv_ttl - 1
				    << " MOVES\n";
}
//---------------------------------------------------------------------------------
// It changes pacman direction on place, always legit. It returns true if the input
// was correct, false otherwise.
bool change_dir(const char d, pacman &p){
	switch(d){
		case 'w': // North.
			p.dir = NORTH;
			maze[p.y][p.x] = PCM_N;
			return true;
		case 'a': // West.
			p.dir = WEST;
			maze[p.y][p.x] = PCM_W;
			return true;
		case 's': // South.
			p.dir = SOUTH;
			maze[p.y][p.x] = PCM_S;
			return true;
		case 'd': // East.
			p.dir = EAST;
			maze[p.y][p.x] = PCM_E;
			return true;
		default:
			return false;
	}
}
//---------------------------------------------------------------------------------
// Random movement generator.
void change_ghosts_dir(ghost ghosts[], const size_t size_g){
	if((rand() % 100) < random_path_chance){
		for(size_t i = 0; i < size_g; ++i){
			ghosts[i].dir = rand_dir();
		}
	}
}
//---------------------------------------------------------------------------------
// Ghosts random movements in the maze.
void move_ghosts(ghost ghosts[], const size_t size_g){
	change_ghosts_dir(ghosts, size_g); // Always legit.
	ssize_t x, y;
	for(size_t i = 0; i < size_g; ++i){
		x = 0;
		y = 0;
		if(ghosts[i].dir == NORTH){
			x = ghosts[i].x;
			y = ghosts[i].y - 1;
		}
		else if(ghosts[i].dir == WEST){
			x = ghosts[i].x - 1;
			y = ghosts[i].y;
		}
		else if(ghosts[i].dir == SOUTH){
			x = ghosts[i].x;
			y = ghosts[i].y + 1;
		}
		else{ // ghosts[i].dir == EAST
			x = ghosts[i].x + 1;
			y = ghosts[i].y;
		}

		if(check_ghost_pos(x, y)){
			maze[ghosts[i].y][ghosts[i].x] = ghosts[i].prev_elem;
			ghosts[i].prev_elem = maze[y][x];
			maze[y][x] = GHOST;
			ghosts[i].x = x;
			ghosts[i].y = y;
		}
	}
}
/*---------------------------------------------------------------------------------*/
// FUNCTIONS
/*---------------------------------------------------------------------------------*/
void setup_game(const unsigned int d){ // Difficulty.
	if(d <= 1){
		std::cout << "\nEASY DIFFICULTY SELECTED\n";
		difficulty_level   = 1;
		ghosts_n           = 1;
		fruits_n           = 5;
		respawn_count_down = 20;
		invulnerability    = 30;
		ttl_fruit          = 30;
		ttl_pellet         = 30;
		random_path_chance = 100;
	}
	else if(d == 2){
		std::cout << "\nNORMAL DIFFICULTY SELECTED\n";
		difficulty_level   = 2;
		ghosts_n           = 4;
		fruits_n           = 3;
		respawn_count_down = 15;
		invulnerability    = 20;
		ttl_fruit          = 20;
		ttl_pellet         = 20;
		random_path_chance = 50;
	}
	else if(d == 3){
		std::cout << "\nHARD DIFFICULTY SELECTED\n";
		difficulty_level   = 3;
		ghosts_n           = 10;
		fruits_n           = 2;
		respawn_count_down = 10;
		invulnerability    = 15;
		ttl_fruit          = 20;
		ttl_pellet         = 20;
		random_path_chance = 25;
	}
	else if(d >= 4){
		std::cout << "\nNIGHTMARE DIFFICULTY SELECTED\n";
		difficulty_level   = 4;
		ghosts_n           = 15;
		fruits_n           = 1;
		respawn_count_down = 5;
		invulnerability    = 10;
		ttl_fruit          = 10;
		ttl_pellet         = 10;
		random_path_chance = 13;
	}
}
//---------------------------------------------------------------------------------
char display_menu(){
	std::string c = ""; // User choice.
	std::cout << "--------------------------------------------------------------------------\n"
                     "                  PACMAN GAME - BY MATTIA CACCIATORE v3.0\n"
                     "--------------------------------------------------------------------------\n\n"
                     " '#' = WALL    'o' = FRUIT    '*' = PELLET    'M' = GHOST    '<' = PACMAN\n\n"
                     "                  Press w/a/s/d on keyboard to make a move\n"
		     "                        q to exit from the game\n\n"
                     "1. Select the default maze\n"
                     "2. Insert the maze from file (it must be 15x15)\n"
                     "3. Change difficulty (now is ";
	if(difficulty_level == 1) std::cout << "EASY";
	else if(difficulty_level == 2) std::cout << "NORMAL";
	else if(difficulty_level == 3) std::cout << "HARD";
	else if(difficulty_level == 4) std::cout << "NIGHTMARE";
	else std::cout << "---";
	std::cout << ")\n"
                     "4. Show the best score\n"
                     "0. Exit\n"
                     "Insert option number and play ENTER ---> ";
	for(;;){
		std::cin >> c;
		if(c[0] >= '0' && c[0] <= '4') break;
		else std::cout << "Invalid option, insert the correct option number ---> ";
	}
	return c[0];
}
//---------------------------------------------------------------------------------
void display_endgame(const bool b){
	if(b)
		std::cout << "\n\n***** YOU WIN *****\n\n";
	else
		std::cout << "\n\n***** GAME OVER *****\n\nYou have been"
			  << " caught by a ghost!\n\n";
}
//---------------------------------------------------------------------------------
void display(const pacman &p){
	display_score(p);
	std::cout << "\n";
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	for(size_t y = 0; y < WALL_SIZE; ++y){
		for(size_t x = 0; x < WALL_SIZE; ++x){
			switch(maze[y][x]){
				case WALL:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
#elif __unix__
					std::cout << BOLD_BLUE;
#endif
					std::cout << '#';
					break;
				case DOT:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY
								| FOREGROUND_BLUE | FOREGROUND_RED
								| FOREGROUND_GREEN);
#elif __unix__
					std::cout << BOLD_WHITE;
#endif
					std::cout << '.';
					break;
				case EMPTY:
					std::cout << ' ';
					break;
				case PCM_N:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY
								| FOREGROUND_RED | FOREGROUND_GREEN);
#elif __unix__
					std::cout << BOLD_YELLOW;
#endif
					std::cout << 'v';
					break;
				case PCM_W:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY
								| FOREGROUND_RED | FOREGROUND_GREEN);
#elif __unix__
					std::cout << BOLD_YELLOW;
#endif
					std::cout << '>';
					break;
				case PCM_S:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY
								| FOREGROUND_RED | FOREGROUND_GREEN);
#elif __unix__
					std::cout << BOLD_YELLOW;
#endif
					std::cout << '^';
					break;
				case PCM_E:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY
								| FOREGROUND_RED | FOREGROUND_GREEN);
#elif __unix__
					std::cout << BOLD_YELLOW;
#endif
					std::cout << '<';
					break;
				case GHOST:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY
								| FOREGROUND_GREEN | FOREGROUND_BLUE);
#elif __unix__
					std::cout << BOLD_CYAN;
#endif
					std::cout << 'M';
					break;
				case FRUIT:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
#elif __unix__
					std::cout << BOLD_RED;
#endif
					std::cout << 'o';
					break;
				case PELLET:
#ifdef _WIN32
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);
#elif __unix__
					std::cout << BOLD_PURPLE;
#endif
					std::cout << '*';
					break;
				default:
					break;
			}
#ifdef __unix__
			std::cout << OFF;
#endif
			std::cout << ' ' << ' ';
		}
#ifdef __unix__
		std::cout << OFF;
#endif
		std::cout << "\n\n";
	}
#ifdef _WIN32
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED
				| FOREGROUND_GREEN);
#elif __unix__
	std::cout << OFF;
#endif
	std::cout << "\n";
}
//---------------------------------------------------------------------------------
bool init_maze(const std::string filename){
	std::ifstream ifs(filename);
	if(ifs.good()){
		fill_maze();
		std::string line;
		total_dot_score = 0; // Reset.
		for(size_t y = 1; !ifs.eof() && y <= MAZE_SIZE; ++y){
			getline(ifs, line);
			for(size_t x = 1, c = 0; c < MAZE_SIZE; ++x, ++c){
				switch(line[c]){
					case '#':
						maze[y][x] = WALL;
						break;
					case '.':
						maze[y][x] = DOT;
						total_dot_score++; // Updating the total score.
						break;
					default: // Design specs: consider other characterts as ' '.
						maze[y][x] = EMPTY;
						break;
				}
			}
		}
		ifs.close();
		return true;
	}
	else{
		std::cerr << "\nMaze file not found\n";
		return false;
	}
}
//---------------------------------------------------------------------------------
bool init_pacman(pacman &p, const size_t x, const size_t y){
	if(check_pos(x,y)){
		p.x = x;
		p.y = y;
		if(maze[y][x] == DOT){
			p.score = dot_score;
			p.d_score = 1; // He spawns over a dot.
		}
		else p.score = p.d_score = 0;
		p.inv_ttl = 0;
		p.dir = SOUTH;      // Design specs.
		maze[y][x] = PCM_S; // Design specs.
		return true;
	}
	else{
		std::cerr << "\nIllegal pacman position\n";
		return false;
	}
}
//---------------------------------------------------------------------------------
void init_ghosts(ghost ghosts[], const size_t size_g){
	size_t x, y;
	for(size_t i = 0; i < size_g; ++i){
		gen_rand_coord(x,y);
		ghosts[i].x = x;
		ghosts[i].y = y;
		ghosts[i].prev_elem = maze[y][x]; // It should be a dot.
		ghosts[i].rspwn_cntdown = respawn_count_down;
		ghosts[i].dir = NO_DIR;
		maze[y][x] = GHOST;
	}
}
//---------------------------------------------------------------------------------
void init_fruits(fruit fruits[], const size_t size_f){
	for(size_t i = 0; i < size_f; ++i){
		fruits[i].x = 0; // No position. It's a wall.
		fruits[i].y = 0;
		fruits[i].f_ttl = 0;
	}
}
//---------------------------------------------------------------------------------
void init_pellet(pellet &p){
	p.x = 0; // No position. It's a wall.
	p.y = 0;
	p.p_ttl = 0;
}
//---------------------------------------------------------------------------------
void respawn_ghosts(ghost ghosts[], const size_t size_g){
	size_t x, y;
	for(size_t i = 0; i < size_g; ++i){
		if(ghosts[i].x == 0 && ghosts[i].y == 0){
			if(ghosts[i].rspwn_cntdown <= 0){
				gen_rand_coord(x,y);
				ghosts[i].x = x;
				ghosts[i].y = y;
				ghosts[i].rspwn_cntdown = respawn_count_down;
				ghosts[i].prev_elem = maze[y][x]; // It should be a dot or an empty space.
				ghosts[i].dir = NO_DIR;
				maze[y][x] = GHOST;
			}
			else ghosts[i].rspwn_cntdown--;
		}
	}
}
//---------------------------------------------------------------------------------
void update_fruits(fruit fruits[], const size_t size_f, ghost ghosts[], const size_t size_g){
	size_t x, y;
	for(size_t i = 0; i < size_f; ++i){
		if(fruits[i].x == 0 && fruits[i].y == 0){ // Spawn fruit.
			// One chance to spawn once per move in a random location.
			x = static_cast<size_t>(rand() % MAZE_SIZE);
			y = static_cast<size_t>(rand() % MAZE_SIZE);
			if(maze[y][x] == EMPTY){
				maze[y][x] = FRUIT;
				fruits[i].x = x;
				fruits[i].y = y;
				fruits[i].f_ttl = ttl_fruit;
			}
		}
		else if(fruits[i].f_ttl > 0){
			fruits[i].f_ttl--;
			if(fruits[i].f_ttl == 0){
				// Check if there is a ghost. In that case update its covered elem.
				if(maze[fruits[i].y][fruits[i].x] == GHOST){
					for(size_t j = 0; j < size_g; ++j){
						if(fruits[i].x == ghosts[j].x
						&& fruits[i].y == ghosts[j].y){
							ghosts[j].prev_elem = EMPTY;
							break;
						}
					}
				}
				else if(check_pacman_pos(fruits[i].x, fruits[i].y))
					maze[fruits[i].y][fruits[i].x] = EMPTY;
				fruits[i].x = 0;
				fruits[i].y = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------------
void update_pellet(pellet &p, ghost ghosts[], const size_t size_g){
	if(p.x == 0 && p.y == 0){ // Spawn pellet.
		size_t x,y;
		x = static_cast<size_t>(rand() % MAZE_SIZE);
		y = static_cast<size_t>(rand() % MAZE_SIZE);
		if(maze[y][x] == EMPTY){
			maze[y][x] = PELLET;
			p.x = x;
			p.y = y;
			p.p_ttl = ttl_pellet;
		}
	}
	else if(p.p_ttl > 0){
		p.p_ttl--;
		if(p.p_ttl == 0){
			// Check if there is a ghost. In that case update its covered elem.
			if(maze[p.y][p.x] == GHOST){
				for(size_t i = 0; i < size_g; ++i){
					if(p.x == ghosts[i].x && p.y == ghosts[i].y)
						ghosts[i].prev_elem = EMPTY;
				}
			}
			else if(check_pacman_pos(p.x, p.y)) maze[p.y][p.x] = EMPTY;
			p.x = 0;
			p.y = 0;
		}
	}
}
//---------------------------------------------------------------------------------
char input(){
	char m;
#ifdef _WIN32
	m = _getch(); // system("clear") in Cygwin!
	//std::cin >> move;
#elif __unix__
	system("stty raw -echo");
	m = getchar();
	system("stty cooked echo");
#endif
	return m;
}
//---------------------------------------------------------------------------------
bool move(const char d, pacman &p, ghost ghosts[], const size_t size_g){
	if(change_dir(d, p)){  // Change direction. Always legit.
		ssize_t x = 0, y = 0;
		// Collision detection system.
		bool possible_collision = false;
		// Pacman direction and possible collision detection.
		if(p.dir == NORTH){
			x = p.x;
			y = p.y - 1;
		}
		else if(p.dir == WEST){
			x = p.x - 1;
			y = p.y;
		}
		else if(p.dir == SOUTH){
			x = p.x;
			y = p.y + 1;
		}
		else{ // p.dir == EAST
			x = p.x + 1;
			y = p.y;
		}
		// Possible collision detected.
		if(maze[y][x] == GHOST) possible_collision = true;
		// Pacman move.
		if(check_pos(x, y)){
			maze[p.y][p.x] = EMPTY;
			p.x = x;
			p.y = y;
			update_pacman(maze[y][x], p, ghosts, size_g);
		}

		move_ghosts(ghosts, size_g); // Ghosts movement.

		if(check_pos(x,y)){ // Double check for a bug with 2 ghosts with same directions.
			if(p.dir == NORTH) maze[y][x] = PCM_N;
			else if(p.dir == WEST) maze[y][x] = PCM_W;
			else if(p.dir == SOUTH) maze[y][x] = PCM_S;
			else maze[y][x] = PCM_E;
		}
		// Collision detection system. Checking the position behind pacman when all
		// entities have moved. If their direction is opposite, that means they collided.
		// Only one ghost can have the opposite direction.
		// If pacman is invulnerable then the ghost is killed, otherwise it's game over.
		if(possible_collision){
			DIR opposite_pacman_dir = NO_DIR;

			if(p.dir == NORTH && maze[p.y + 1][p.x] == GHOST){
				opposite_pacman_dir = SOUTH;
				x = p.x;
				y = p.y + 1;
			}
			else if(p.dir == WEST && maze[p.y][p.x + 1] == GHOST){
				opposite_pacman_dir = EAST;
				x = p.x + 1;
				y = p.y;
			}
			else if(p.dir == SOUTH && maze[p.y - 1][p.x] == GHOST){
				opposite_pacman_dir = NORTH;
				x = p.x;
				y = p.y - 1;
			}
			else if(p.dir == EAST && maze[p.y][p.x - 1] == GHOST){
				opposite_pacman_dir = WEST;
				x = p.x - 1;
				y = p.y;
			}

			if(opposite_pacman_dir != NO_DIR){ // Collision detected.
				for(size_t i = 0; i < size_g; ++i){
					if(ghosts[i].dir == opposite_pacman_dir &&
						ghosts[i].x == x && ghosts[i].y == y){
						if(p.inv_ttl > 0){ // Pacman survives. Ghost killed.
							kill_ghost(ghosts[i], NO_DIR);
							break;
						}
						else{ // Pacman dead.
							maze[ghosts[i].y][ghosts[i].x] = GHOST;
							maze[p.y][p.x] = EMPTY;
							return false;
						}
					}
				}
			}
		}
		// Decreasing pacman invulnerability.
		if(p.inv_ttl > 0) p.inv_ttl--;
	}
	return true;
}
//---------------------------------------------------------------------------------
bool check_game_status(pacman &p, ghost ghosts[], const size_t size_g){
	if(check_pacman_status(p, ghosts, size_g) || p.d_score >= total_dot_score){
		display(p);
		display_endgame(p.d_score >= total_dot_score);
		return true;
	}
	else return false;
}
