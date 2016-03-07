#pragma once

#include <vector>
#include <iostream>
#include "Tile.h"

using namespace std;

class Quadtree
{
public:

	Quadtree();

	// Inserts a tile into the quadtree
	//
	// t = the tile to insert
	// x = the x location of the tile
	// y = the y location of the tile
	//
	// Returns true if successful insertion, false otherwise
	bool Insert(shared_ptr<Tile> t, int x, int y);

	// Inializes a quadtree to the desired number of levels
	//
	// numLevels - the number of levels of the tree
	// width - the width of the gamespace
	// height - the height of the gamespace
	void Initialize(int numLevels, int width, int height);

	// Checks which tiles need to be drawn
	// 
	// viewport - the rectangle that we want all tiles in to be drawn
	//
	// returns a vector of all of the tiles to draw
	vector<shared_ptr<Tile>> Draw(SDL2pp::Rect viewport);

	int GetWidth() { return _width; }
	int GetHeight() { return _height; }

private:

	// Definition of a node in the quadtree
	struct Node
	{
		Node* topLeft;
		Node* topRight;
		Node* bottomLeft;
		Node* bottomRight;

		vector<shared_ptr<Tile>> objects;

		SDL2pp::Rect bound;

		Node(SDL2pp::Rect bounds, Node* tl = nullptr, Node* tr = nullptr, Node* bl = nullptr, Node* br = nullptr, vector<shared_ptr<Tile>> o = vector<shared_ptr<Tile>>())
			: bound(bounds), topLeft(tl), topRight(tr), bottomLeft(bl), bottomRight(br), objects(o) {}
	};

	Node* _root;
	int _numLevels;
	int _width;
	int _height;
	vector<shared_ptr<Tile>> _tiles;

	// Splits the bounds of the node into 4 children nodes
	//
	// level = the current level in the tree
	// node = the node to partition
	void Partition(int level, Node* node);

	// Inserts a tile into the tree
	//
	// t = the tile to insert
	// x = the tile's x position
	// y = the tiles y position
	// node = the node to insert into
	bool Insert(shared_ptr<Tile> t, int x, int y, Node * node);

	// Determines which tiles to draw
	// 
	// viewport = the rectangle we want to draw from
	// node = the node that contains the tiles.
	void Draw(SDL2pp::Rect viewport, Node* node);

	// Checks to see if the point is in the rectangle
	//
	// r = the rectangle to check with
	// x = x coordinate of the point
	// y = y coordinate of the point
	//
	// returns true if the rectangle conatins the point, false otherwise
	bool ContainsPoint(SDL2pp::Rect r, int x, int y);

	// Checks to see if the rectangle overlaps another rectangle
	//
	// r1 = the first rectangle
	// r2 = the second rectangle
	//
	// returns true if the rectangle conatins the point, false otherwise
	bool IntersectRectangle(SDL2pp::Rect r1, SDL2pp::Rect r2);
};
