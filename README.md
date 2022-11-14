# Pacman-Game-Console-Application

My implementation of Pacman game in C++ (a simplest version for console application). Available for Unix and Windows.

## Usage

<details><summary><b>Show instructions</b></summary>

1. Download and compile:
  ```sh
  $ make
  
  $ ./pacman
  ```
  or
  ```sh  
  g++ -D PCMCOLOR -std=c++17 advanced_pacman_test.cpp advanced_pacman_functions.cpp advanced_scoreboard_pacman.cpp -o pacman
  
  $ ./pacman
  ```
  
  PCMCOLOR is a macro that enables the colored version of pacman on terminal/console.
</details>

## Motivation

This was an optional and voluntary university assignment, guided for the version 1.0. I was curious and interested 
in videogames, their creation process, the code details, all the generic and various aspects of what "Make a videogame"
means in a very basic and simple way.

## Why

Because it's fun! And you can see, at code level, how a pacman videogame can works (very basic, for example it lacks of
ghost AI).

## Knowledge

How to program a basic videogame for console application (in C++).

## Versions

These versions were implemented by me and improved over time, only version 1 was guided.

Version 1.0 - Base game without ghosts, fruits and pellet. Just the maze, pacman and dots.

Version 2.0 - Game with ghosts, fruits and pellet. Pacman has new fields like score and invulnerability.

Version 3.0 - Colored maze, scoreboard and direct input.


Have fun!
