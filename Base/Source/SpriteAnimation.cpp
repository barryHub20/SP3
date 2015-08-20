#include "SpriteAnimation.h"
#include "MyMath.h"
#include "GL\glew.h"
#include "Vertex.h"
#include <iostream>
SpriteAnimation::SpriteAnimation()
{

}

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
 : Mesh(meshName)
 , m_row(row)
 , m_col(col)
 , m_totalFrame(0)
 , m_currentRow(0)	
 , m_currentCol(0)
 , m_currentTime(0)
 ,m_frameTime(0)
, repeatCount(0)
, m_playCount(0)
, m_currentFrameTime(0)
, ended(false)
{
}

void SpriteAnimation::init(float time, int startCol, int startRow, int endCol, int endRow, int repeatCount, bool oppDir)
{
	/* set total frame and frame time */
	//total rows
	this->oppDir = oppDir;

	this->startRow = startRow;
	this->endRow = endRow;
	this->startCol = startCol;
	this->endCol = endCol;
	this->repeatCount = repeatCount;
	
	int totalRow = endRow - (startRow - 1);

	int totalCol = m_col - startCol;	//first row
	totalCol += m_col * ((endRow) - (startRow + 1));
	totalCol += endCol;

	m_totalFrame = totalCol + totalRow;
	m_frameTime = time / m_totalFrame;

	/* set current row and column */
	if(!this->oppDir)	//if normal dir: start from start
	{
		m_currentRow = startRow;	
		m_currentCol = startCol;
	}
	else	//if !normal dir: start from end
	{
		m_currentRow = endRow;	
		m_currentCol = endCol;
	}

	/* error check */
	if(m_currentRow < 0 || m_currentRow >= m_row)
		m_currentRow = m_row - 1;
	if(m_currentCol < 0 || m_currentCol >= m_col)
		m_currentCol = 0;
}

SpriteAnimation::~SpriteAnimation()
{
}

void SpriteAnimation::Update(double dt)
{
	if (ended == false)
	{
		/* time keeps updating */
		m_currentFrameTime += (float)dt;
		m_currentTime += (float)dt;

		/* if past frame time, go on to next frame */
		if (m_currentFrameTime > m_frameTime)
		{
			/* timer reset */
			m_currentFrameTime = 0.f;

			/* increase column */
			if (!oppDir)
				++m_currentCol;
			else
				--m_currentCol;
		}

		//Move to next row
		if (!oppDir)
		{
			/* if not reach last row */
			if (m_currentRow < endRow)
			{
				/* if col exceed */
				if (m_currentCol >= m_col)
				{
					m_currentCol = 0;
					++m_currentRow;
				}
			}
		}
		else if (oppDir)
		{
			/* if not reach last row */
			if (m_currentRow > startRow)
			{
				/* if col exceed */
				if (m_currentCol < 0)
				{
					m_currentCol = m_col - 1;
					--m_currentRow;
				}
			}
		}
		//std::cout << m_currentTime << " " << m_frameTime * m_totalFrame << std::endl;
		if (m_currentTime > m_frameTime * m_totalFrame) //If time exceeds animation time
		{
			ended = true;
			if (m_playCount + 1 <= repeatCount || repeatCount == 0) //If play again, reset
			{
				if (!oppDir)
				{
					///* reach last row */
					//if (m_currentRow == endRow)
					//{
					/* if col exceed */
					/*if (m_currentCol > endCol)
					{*/
					++m_playCount;
					m_currentTime = 0.f;
					m_currentCol = startCol;
					m_currentRow = startRow;
					//}
					//}
				}

				/* opp. Direction: flip everything */
				else if (oppDir)
				{
					///* reach last row */
					//else if (m_currentRow == startRow)
					//{
					/* if col exceed */
					//if (m_currentCol < startCol)
					//{
					m_currentCol = endCol;
					m_currentRow = endRow;
					//}
					//}
				}
			}
		}
	}
	
	
}

void SpriteAnimation::Reset()
{
	ended = false;
	m_currentFrameTime = 0.f;
	m_currentTime = 0.f;
	m_currentCol = startCol;
	m_currentRow = startRow;
	m_playCount = 0;
}

void SpriteAnimation::Render()
{
	unsigned count = 6;	//a quad has 6 sides
	unsigned offset = 0;
	offset = (m_currentCol + (m_currentRow * m_col)) * count;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	//if(textureID > 0)
	//{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	//}

	//glDrawArrays(GL_TRIANGLES, offset, count);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if(mode == DRAW_LINES)
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else if(mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	else
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//if(textureID > 0)
	//{
		glDisableVertexAttribArray(3);
	//}
 }