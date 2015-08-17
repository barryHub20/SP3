#include "utilities.h"

//client: string to be compared, matchee: string used to match
bool searchTerm(const string& client, const string& matchee)
{
	if(client.length() == 0)
	{
		if(matchee.length() == 0)
			return true;
		else
			return false;
	}

	for(int j = 0; j < client.length(); ++j)
	{
		bool c= true;
		if(matchee[0] == client[j])
		{
			if(matchee.length() < 2 || (matchee.length() == 1 && j == client.length() - 1))
			return true;

			if(client.length() - (j + 1) >= matchee.length() - 1)
			{
				int index = 1;
				for(int i = j + 1; i < client.length(); ++i, ++index)
				{
					if(client[i] != matchee[index])
					{
						c = false;
						break;	//continue search
					}
					if(index == matchee.length() - 1)
						return true;
				}
				if(c == true)
					return true;
			}
			else
				return false;
		}
		else if(j == client.length() - 1)
			return false;
	}
	return false;
}

void deleteChar(string& word, char del)
{
	int lastChar = 0;
	bool c = false;
	for(int i = 0; i < word.length(); ++i)
	{
		if(word[i]  == del)
		{
			if(c == false)
			{
				c = true;
				lastChar = i;
			}
		}
		else
			c = false;
	}

	if(c == true)
		word.resize(lastChar);
}