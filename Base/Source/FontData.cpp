#include "FontData.h"

void getFontData(char* name, float* fontData)
{
	ifstream myfile(name);
	string sentence;
	int multiplier = 1;	//for conversion from string to numerical
	float data = 0;	//width data
	float ImageWidth = 64;

	/* error check */
	try
	{
		if(!myfile.is_open())
			throw "Font File cannot be found: good luck noobshit";
		if(fontData == NULL)
			throw "What kind of shit you passing in??";
	}
	catch(string n)
	{
		cout << n << endl;
	}

	/* search for string begining with: Char */
	int counter = 0;	//counts to 255

	while(getline(myfile, sentence) && counter < 256)
	{
		if(searchTerm(sentence, "Base Width"))
		{
			/* get the width */
			//.......Base Width,26
			for(int i = sentence.length() - 1; i > 0; --i)
			{
				/* convert to numerical data */
				if(sentence[i] != ',')
				{
					data += (sentence[i] - 48) * multiplier;
					multiplier *= 10;
				}
				else
				{
					fontData[counter] = data / ImageWidth;
					break;	//reach comma, go on to find next char width
				}
				
			}
			multiplier = 1;
			data = 0;
			++counter;
		}
	}
}