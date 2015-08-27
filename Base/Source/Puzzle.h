#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>

#include "Mesh.h"

class Puzzle
{
public:
	enum PUZZLETYPE
	{
		NONE,
		WORD,
		PICTURE,
		MAX_TYPE,
	};
private:
	std::string puzzleText;
	Mesh* puzzleMesh;
	PUZZLETYPE puzzleType;

public:
	Puzzle();
	~Puzzle();

	void setTextPuzzle(std::string puzzleText);
	std::string getTextPuzzle();

	void setPicturePuzzle(Mesh* puzzleMesh);
	Mesh* getPicturePuzzle();

	void setType(PUZZLETYPE puzzleType);
	Puzzle::PUZZLETYPE getType();

	void cleanUp();
};

#endif
