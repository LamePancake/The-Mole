#include "Util.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		if (!item.empty())
		{
			elems.push_back(item);
		}
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void Sort(std::vector<std::shared_ptr<Actor>>& actors)
{
	std::vector<std::shared_ptr<Actor>> temp(actors.size());
	MergeSort(actors, temp, 0, actors.size() - 1);
}

void MergeSort(std::vector<std::shared_ptr<Actor>>& actors, std::vector<std::shared_ptr<Actor>>& temp, int left, int right) 
{
	
	if (left < right)
	{
		int middle = int((left + right) / 2);
		MergeSort(actors, temp, left, middle);
		MergeSort(actors, temp, middle + 1, right);
		Merge(actors, temp, left, middle + 1, right);
	}
}

void Merge(std::vector<std::shared_ptr<Actor>>& actors, std::vector<std::shared_ptr<Actor>>& temp, int leftPos, int rightPos, int rightEnd)
{
	int leftEnd = rightPos - 1;
	int tempPos = leftPos;
	int numElements = rightEnd - leftPos + 1;

	while (leftPos <= leftEnd && rightPos <= rightEnd)
	{
		if (actors[leftPos]->GetZIndex() <= actors[rightPos]->GetZIndex())
			temp[tempPos++] = std::move(actors[leftPos++]);
		else
			temp[tempPos++] = std::move(actors[rightPos++]);
	}

	while (leftPos <= leftEnd)
		temp[tempPos++] = std::move(actors[leftPos++]);

	while (rightPos <= rightEnd)
		temp[tempPos++] = std::move(actors[rightPos++]);

	for (int i = 0; i < numElements; ++i, --rightEnd)
		actors[rightEnd] = std::move(temp[rightEnd]);
}

