#include "PuzzleManager.h"
#include <string>
#include <iostream>
#include <fstream>
#include "MeshBuilder.h"
#include "LoadTGA.h"

using namespace std;

PuzzleManager::PuzzleManager()
{
	currentPuzzleNumber = currentPart = NULL;
	currentPuzzle = NULL;
	puzzleList.clear();
}

PuzzleManager::~PuzzleManager()
{
}

void PuzzleManager::Init(int noOfPuzzles)
{
	//puzzleList.resize(noOfPuzzles);
}

void PuzzleManager::addPicturePuzzle(const char * picture_file_path)
{
	Mesh* tempMesh;
	tempMesh = MeshBuilder::GenerateQuad("puzzle picture", Color(1, 1, 1));
	tempMesh->textureID[0] = LoadTGA(picture_file_path);
	Puzzle* tempPuzzle;
	tempPuzzle = new Puzzle();
	tempPuzzle->setType(Puzzle::PICTURE);
	tempPuzzle->setPicturePuzzle(tempMesh);
	puzzleList.push_back(tempPuzzle);
}

void PuzzleManager::addTextPuzzle(std::string puzzleText)
{
	Puzzle* tempPuzzle;
	tempPuzzle = new Puzzle();
	tempPuzzle->setType(Puzzle::WORD);
	tempPuzzle->setTextPuzzle(puzzleText);
	puzzleList.push_back(tempPuzzle);
}

Puzzle * PuzzleManager::getCurrentPuzzle()
{
	if (currentPuzzle == NULL)
	{
		currentPuzzle = puzzleList[0];
	}
	return currentPuzzle;
}

void PuzzleManager::goToNextPart()
{
	if (currentPart + 1 >= puzzleList.size())
	{
		changePuzzle();
	}
	else
	{
		++currentPart;
	}
	currentPuzzle = puzzleList[currentPart];
}

void PuzzleManager::changePuzzle()
{
	++currentPuzzleNumber;
	int tempPuzzleNo;
	tempPuzzleNo = currentPart;
	currentPart = 0;
	if (currentPuzzleNumber >= puzzleList.size()|| puzzleList.size() == 0)
	{
		currentPuzzleNumber -= 1;
		currentPart = tempPuzzleNo;
	}
}

void PuzzleManager::cleanUp()
{
	for (int i = 0; i < puzzleList.size(); i++)
	{
		if (puzzleList[i] != NULL)
		{
			puzzleList[i]->cleanUp();
			delete puzzleList[i];
		}
	}
}

