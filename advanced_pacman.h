/*
 *  Pacman game - IP course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2021-2022 - Italy
 *
 *  advanced_pacman.h
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
#include "advanced_pacman_utilities.h"
#include <cstdlib>         // rand, srand and getch.

/*---------------------------------------------------------------------------------*/
// GLOBAL VARIABLES & CONSTANTS
/*---------------------------------------------------------------------------------*/
#define MAZE_SIZE 15     // Maze size.
#define WALL_SIZE MAZE_SIZE+2 // +2 outside walls.

typedef enum{NORTH, WEST, SOUTH, EAST, NO_DIR} DIR;
typedef enum{WALL, EMPTY, DOT, FRUIT, PELLET, PCM_N, PCM_W, PCM_S, PCM_E, GHOST} MAZE_ELEMS;

inline size_t ghosts_n = 4; // Number of ghosts. 4 by default.
inline size_t fruits_n = 5; // Number of fruits. max 5 in game. Design specs.
inline unsigned int difficulty_level = 2;
inline int total_dot_score = 0;
/*---------------------------------------------------------------------------------*/
// STRUCTURES
/*---------------------------------------------------------------------------------*/
struct pacman{
	ssize_t x;   // Position.
	ssize_t y;
	int score;   // Current amount of points.
	int d_score; // Dot score.
	int inv_ttl; // Invulnerability time to live. Pacman is immortal for 'inv_ttl' moves.
	DIR dir;     // Direction.
};

struct ghost{
	ssize_t x;
	ssize_t y;
	int rspwn_cntdown;    // Respawn count down: how many steps are required to respawn a dead ghost.
	MAZE_ELEMS prev_elem; // Element covered by the ghost (e.g. a dot).
	DIR dir;
};

struct fruit{
	ssize_t x;
	ssize_t y;
	int f_ttl; // Fruit time to live.
};

struct pellet{
	ssize_t x;
	ssize_t y;
	int p_ttl; // Pellet time to live.
};
/*---------------------------------------------------------------------------------*/
// FUNCTIONS
/*---------------------------------------------------------------------------------*/
// It setups the difficulty level and all global variables. 1st argument: 1 <= difficulty level <= 4.
void setup_game(const unsigned int);
// It prints on stdout the main game menu with options. It returns the user choice.
char display_menu();
// It prints on stdout the endgame message (victory or defeat). 1st argument: win condition.
void display_endgame(const bool);
// It prints on stdout the current state of the maze and pacman. 1st argument: pacman.
void display(const pacman &);
// It builds the maze from a file. It returns true if it did it, false otherwise.
// 1st argument: filename.
bool init_maze(const std::string);
// It builds pacman. It returns true if it did it, false otherwise. 1st argument: pacman,
// 2nd argument: x-coordinate (orizzontal), 3rd argument: y-coordinate (vertical).
bool init_pacman(pacman &, const size_t, const size_t);
// It builds ghosts in the maze with random positions. 1st argument: ghost array,
// 2nd argument: number of ghost(s).
void init_ghosts(ghost [], const size_t);
// It builds fruits. 1st argument: fruit array, 2nd argument: number of fruit(s).
void init_fruits(fruit [], const size_t);
// It builds pellet. 1st argument: pellet.
void init_pellet(pellet &);
// It respawns dead ghosts in the maze in random legit position(s) if their rspwn_cntdown <= 0.
// 1st argument: ghosts array, 2nd argument: number of ghost(s).
void respawn_ghosts(ghost [], const size_t);
// It updates all fruits in the maze one time per move, eventually update ghost(s).
// 1st argument: fruit array, 2nd argument: number of fruit(s).
void update_fruits(fruit [], const size_t, ghost [], const size_t);
// It updates the pellet in the maze one time per move, eventually update the ghost.
// 1st argument: pellet, 2nd argument: ghosts array, 3rd argument: number of ghost(s).
void update_pellet(pellet &, ghost [], const size_t);
// It reads input from keyboard and returns the first char.
char input();
// It executes pacman and ghosts move in the maze. It returns true if nothing happened, false
// if a collision with a ghost was detected. 1st argument: input move (legit: wasd),
// 2nd argument: pacman, 3rd argument: ghost array, 4th argument: number of ghost(s).
bool move(const char, pacman &, ghost [], const size_t);
// It checks the current status of the game and, eventually, print on stdout if
// the player won or lost the game. It returns true if nothing happened, false otherwise.
// 1st argument: pacman, 2nd argument: ghost array, 3rd argument: number of ghost(s).
bool check_game_status(pacman &, ghost[], const size_t);
