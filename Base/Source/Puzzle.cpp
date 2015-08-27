#include "Puzzle.h"



Puzzle::Puzzle()
{
	puzzleText = "";
	puzzleMesh = NULL;
	puzzleType = NONE;
}

Puzzle::~Puzzle()
{
}

void Puzzle::setTextPuzzle(std::string puzzleText)
{
	this->puzzleType = WORD;
	this->puzzleText = puzzleText;
}

std::string Puzzle::getTextPuzzle()
{
	return puzzleText;
}

void Puzzle::setPicturePuzzle(Mesh * puzzleMesh)
{
	this->puzzleType = PICTURE;
	this->puzzleMesh = puzzleMesh;
}

Mesh * Puzzle::getPicturePuzzle()
{
	return puzzleMesh;
}

void Puzzle::setType(PUZZLETYPE puzzleType)
{
	this->puzzleType = puzzleType;
}

Puzzle::PUZZLETYPE Puzzle::getType()
{
	return puzzleType;
}

void Puzzle::cleanUp()
{
}
