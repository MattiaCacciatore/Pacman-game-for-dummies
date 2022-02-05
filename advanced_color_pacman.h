/*
 *  Pacman game - IP course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2021-2022 - Italy
 *
 *  advanced_color_pacman.h
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
#ifdef PCMCOLOR
	#define BOLD_RED    "\x1b[1m\x1b[31m" // Fruits.
	#define BOLD_YELLOW "\x1b[1m\x1b[33m" // Pacman.
	#define BOLD_BLUE   "\x1b[1m\x1b[34m" // Maze walls.
	#define BOLD_PURPLE "\x1b[1m\x1b[35m" // Pellets.
	#define BOLD_CYAN   "\x1b[1m\x1b[36m" // Ghosts.
	#define BOLD_WHITE  "\x1b[1m\x1b[37m" // Dots.
	#define OFF         "\x1b[0m"
#else
	#define BOLD_RED    ""
	#define BOLD_YELLOW ""
	#define BOLD_BLUE   ""
	#define BOLD_PURPLE ""
	#define BOLD_CYAN   ""
	#define BOLD_WHITE  ""
	#define OFF         ""
#endif