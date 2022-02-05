/*
 *  Pacman game - IP course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2021-2022 - Italy
 *
 *  advanced_scoreboard_pacman.h
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
/*---------------------------------------------------------------------------------*/
// FUNCTIONS
/*---------------------------------------------------------------------------------*/
// It records the pacman score with timestamp on file "scoreboard.txt". It returns 
// true if it did it, false otherwise. 1st argument: pacman score, 2nd argument: pacman dot score,
// 3rd argument: level difficulty, 4th argument: maze name.
bool record_score(const int, const int, const int, const std::string);
// It prints on stdout the best point score with timestamp. It returns true if it did it,
// false otherwise.
bool display_best_score();