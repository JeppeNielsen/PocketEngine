#include "Quadtree.hpp"
#include <iostream> 
using namespace Pocket;

const static unsigned MaxObjectsInNode = 64;

Quadtree::Quadtree() {
	children = 0;
	nodes = 0;
	parent = 0;
}

Quadtree::Quadtree(const Vector2& min, const Vector2& max) {
	SetMinMax(min, max);
}

Quadtree::~Quadtree() {
	if (children) delete[] children;
	if (nodes) delete nodes;
}

void Quadtree::SetMinMax(const Vector2& min, const Vector2& max) {
	this->min = min;
    this->max = max;
}

bool Quadtree::Insert(Quadtree::Node& node) {
    
    bool test;
    if (parent) {
        test = node.min.x>=min.x && node.min.y>=min.y && node.max.x<max.x && node.max.y<max.y;
    } else {
        test = !(node.min.x > max.x || node.max.x < min.x || node.min.y > max.y || node.max.y < min.y); //box.Intersects(node.box);
    }

	if (test) {
		if (!nodes) nodes = new Nodes();

		if (nodes->size()>=MaxObjectsInNode) {
			Split();
            bool insertedIntoChildren = false;
            for (int c=0; c<4; c++) {
                insertedIntoChildren = children[c].Insert(node);
                if (insertedIntoChildren) break;
            }
            
            if (!insertedIntoChildren) {
                node.childIndex = nodes->size();
                node.node = this;
                nodes->push_back(&node);
            }
		} else {
			node.childIndex = nodes->size();
			node.node = this;
			nodes->push_back(&node);
		}
		return true;
	}

	return false;
}

void Quadtree::Remove(Node& node) {
    if (!node.node) return;
	Nodes& nodes = *node.node->nodes;
	
	if (nodes.size()<=1) {
		nodes.clear();
	} else {
		Node* last = nodes.back();
		nodes[node.childIndex] = last;
		last->childIndex = node.childIndex;
		nodes.pop_back();
	}
    
    node.node = 0;
}


void Quadtree::Move(Node& node) {

    if (node.min.x>node.node->min.x && node.min.y>node.node->min.y && node.max.x<node.node->max.x && node.max.y<node.node->max.y) {
		return;
	} else {
		Remove(node);
		Insert(node);
	}
}


void Quadtree::Split() {
	if (children) return;
	children = new Quadtree[4];

	Vector2 center = (min + max) * 0.5f;

	children[0].min = min;
    children[0].max = center;
    
	children[1].min = Vector2(center.x, min.y);
    children[1].max = Vector2(max.x, center.y);
    
    children[2].min = center;
    children[2].max = max;
    
    children[3].min = Vector2(min.x, center.y);
    children[3].max = Vector2(center.x, max.y);
    
	for (int i=0; i<4; i++) {
		children[i].parent = this;
	}
}