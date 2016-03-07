#include "Quadtree.h"

Quadtree::Quadtree() 
	: _root(nullptr), _numLevels(0), _width(0), _height(0), _tiles() 
{}

bool Quadtree::Insert(shared_ptr<Tile> t, int x, int y)
{
	return Insert(t, x, y, _root);
}

void Quadtree::Initialize(int numLevels, int width, int height)
{
	_numLevels = numLevels;
	_width = width;
	_height = height;
	_root = new Node(SDL2pp::Rect(0, 0, width, height));
	Partition(1, _root);
}

vector<shared_ptr<Tile>> Quadtree::Draw(SDL2pp::Rect viewport)
{
	_tiles.clear();
	Draw(viewport, _root);
	return _tiles;
}

void Quadtree::Partition(int level, Node* node)
{
	if (node == nullptr)
		return;

	// if we reached the number of desired levels then stop partiioning, else partition
	if (level < _numLevels)
	{
		int childWidthLeft    = node->bound.GetW() / 2;
		int childHeightTop    = node->bound.GetH() / 2;

		int childWidthRight   = node->bound.GetW() - childWidthLeft;
		int childHeightBottom = node->bound.GetH() - childHeightTop;

		int topLeftX          = node->bound.x;
		int topLeftY          = node->bound.y;

		int topRightX         = node->bound.x + childWidthLeft;
		int topRightY         = node->bound.y;

		int bottomLeftX       = node->bound.x;
		int bottomLeftY       = node->bound.y + childHeightTop;

		int bottomRightX      = node->bound.x + childWidthLeft;
		int bottomRightY      = node->bound.y + childHeightTop;

		node->topLeft         = new Node(SDL2pp::Rect(topLeftX,     topLeftY,     childWidthLeft,  childHeightTop));
		node->topRight        = new Node(SDL2pp::Rect(topRightX,    topRightY,    childWidthRight, childHeightTop));
		node->bottomLeft      = new Node(SDL2pp::Rect(bottomLeftX,  bottomLeftY,  childWidthLeft,  childHeightBottom));
		node->bottomRight     = new Node(SDL2pp::Rect(bottomRightX, bottomRightY, childWidthRight, childHeightBottom));

		level++;

		Partition(level, node->topLeft);
		Partition(level, node->topRight);
		Partition(level, node->bottomLeft);
		Partition(level, node->bottomRight);
	}
}

bool Quadtree::Insert(shared_ptr<Tile> t, int x, int y, Node * node)
{
	if (!ContainsPoint(node->bound, x, y))
		return false;

	if (node->topLeft == nullptr)
	{
		node->objects.push_back(t);
		return true;
	}

	if (Insert(t, x, y, node->topLeft)) return true;
	if (Insert(t, x, y, node->topRight)) return true;
	if (Insert(t, x, y, node->bottomLeft)) return true;
	if (Insert(t, x, y, node->bottomRight)) return true;

	return false;
}

void Quadtree::Draw(SDL2pp::Rect viewport, Node* node)
{
	if (!IntersectRectangle(node->bound, viewport))
		return;

	if (node->topLeft == nullptr)
	{
		for (auto it : node->objects)
		{
			if (ContainsPoint(viewport, it->GetIndices().GetX(), it->GetIndices().GetY())
				&& it->GetID() != Tile::blank)
			{
				_tiles.push_back(it);
			}
		}
		return;
	}

	Draw(viewport, node->topLeft);
	Draw(viewport, node->topRight);
	Draw(viewport, node->bottomLeft);
	Draw(viewport, node->bottomRight);
}

bool Quadtree::ContainsPoint(SDL2pp::Rect r, int x, int y)
{
	return ((x < r.x + r.GetW() && x >= r.x) && (y < r.y + r.GetH() && y >= r.y));
}

bool Quadtree::IntersectRectangle(SDL2pp::Rect r1, SDL2pp::Rect r2)
{
	return(r1.x < r2.x + r2.GetW() && r1.x + r1.GetW() > r2.x && r1.y < r2.y + r2.GetH() && r1.y + r1.GetH() > r2.y);
}