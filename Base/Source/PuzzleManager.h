#ifndef PUZZLE_MANAGER_H
#define PUZZLE_MANAGER_H

#include "Puzzle.h"
#include <vector>

using std::vector;

class PuzzleManager
{
private:
	int currentPuzzleNumber;
	int currentPart;
	vector<vector<Puzzle*>> puzzleList;
	Puzzle* currentPuzzle;
public:
	PuzzleManager();
	~PuzzleManager();

	void Init(int noOfPuzzles);
	void addPicturePuzzle(int mapNumber, const char * picture_file_path); //initialize puzzles from text files
	void addTextPuzzle(int mapNumber, std::string puzzleText);

	Puzzle* getCurrentPuzzle();
	void goToNextPart();//proceed with current puzzle
	void changePuzzle(); //change to an entirely different puzzle
	void cleanUp();
};

#endif