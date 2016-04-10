#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include "Actor.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

void Sort(std::vector<std::shared_ptr<Actor>>& actors);
void MergeSort(std::vector<std::shared_ptr<Actor>>& actors, std::vector<std::shared_ptr<Actor>>& temp, int left, int right);
void Merge(std::vector<std::shared_ptr<Actor>>& actors, std::vector<std::shared_ptr<Actor>>& temp, int leftPos, int rightPos, int rightEnd);
#endif