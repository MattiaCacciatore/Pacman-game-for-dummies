/*
 *  Pacman game - IP course Computer Science
 *  Universita' Degli Studi Di Genova a.a. 2021-2022 - Italy
 *
 *  advanced_scoreboard_pacman.cpp
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
#include "advanced_scoreboard_pacman.h"

static const std::string filename = "Scoreboard/scoreboard.txt";
/*---------------------------------------------------------------------------------*/
// FUNCTIONS
/*---------------------------------------------------------------------------------*/
bool record_score(const int score, const int d_score, const int difficulty, const std::string mazename){
	std::fstream file;
	// ios::out -> file open for writing, ios::app -> All output operations happen at the end of the file.
	file.open(filename, std::ios::out | std::ios::app);

	if(file.good()){
		time_t rawtime;
		struct tm* timeinfo;
		std::string timestamp = "", str = "";

		time (&rawtime);
		timeinfo = localtime(&rawtime);
		
		timestamp =  std::to_string(score) + " " + std::to_string(d_score) + " ";
		str = std::to_string(timeinfo->tm_hour);
		if(str.size() == 1) str = "0" + str;
		timestamp += str + ":";
		str = std::to_string(timeinfo->tm_min);
		if(str.size() == 1) str = "0" + str;
		timestamp += str + ":";
		str = std::to_string(timeinfo->tm_sec);
		if(str.size() == 1) str = "0" + str;
		timestamp += str + " ";
		str = std::to_string(timeinfo->tm_mday);
		if(str.size() == 1) str = "0" + str;
		timestamp += str + "/";
		str = std::to_string(timeinfo->tm_mon + 1);
		if(str.size() == 1) str = "0" + str;
		timestamp += str + "/";
		timestamp += std::to_string(timeinfo->tm_year + 1900) + " " + mazename + " ";
		if(difficulty <= 1) timestamp += "EASY";
		else if(difficulty == 2) timestamp += "NORMAL";
		else if(difficulty == 3) timestamp += "HARD";
		else if(difficulty >= 4) timestamp += "NIGHTMARE";
	
		file << timestamp << "\n";
		file.close();
		std::cout << "Score saved!\n";
		return true;
	}
	else{ 
		std::cerr << "File 'scoreboard.txt' unavailable. Cannot record pacman score.\n";
		return false;
	}
}
//-----------------------------------------------------------------------------------
bool display_best_score(){
	std::ifstream ifs(filename);

    	if(ifs.good()){
		std::string line = "", best_timestamp = "";
		int dot_score = 0, best_score = 0;

		for(;getline(ifs, line);){
			dot_score = std::stoi(line.substr(0, line.find(" ")));
			if(dot_score > best_score){
				best_score = dot_score;
				best_timestamp = line;
			}
		}
		ifs.close();
		std::cout << "\n\nYour best score is:\n(point score - dot score - date - maze name" 
			  << " - difficulty)\n\n" << best_timestamp << "\n\n";
		return true;
    	}
	else{ 
		std::cerr << "File 'scoreboard.txt' unavailable. Cannot display best pacman score.\n";
		return false;
	}

}