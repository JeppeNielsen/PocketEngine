//
//  Map.h
//  Homeland
//
//  Created by Jeppe Nielsen on 15/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Colour.hpp"
#include "Vector3.hpp"
#include "Point.hpp"

using namespace Pocket;

struct MapRect {
    int x;
    int z;
    int width;
    int depth;
};

Component(Map)

	struct Node {
    	Node() : height(0), normal(0,1,0), islandID(0) {}
    	float height;
        Vector3 normal;
        Colour color;
        int x;
        int z;
        int pathId;
        int g;
        int h;
        Node* parent;
        int islandID;
    };

    void CreateMap(int width, int depth);
	typedef std::vector<std::vector<Node>> Nodes;

    Nodes nodes;

	Node& GetNode(int x, int z);

	int Width();
    int Depth();

    void Randomize(float minHeight, float maxHeight);
    void Smooth(int iterations);
    void SetHeight(float height);
    void SetMaxHeight(float height);
    void SetEdges(float height);
    void AddHill(int xPos, int zPos, int radius, float height);

    void CalcNormals();
    void CalcNormals(const MapRect& rect);

    void CalculatePath(Point start, Point end, std::vector<Vector3>& path);

    std::vector<Vector2> CreateNavigationMesh();

    std::vector<std::vector<Node*>> FindIslands();

    std::vector<Node*> TraceIsland(int x, int z, int& islandIndex);

    bool IsNodeWalkable(const Node& node);

private:
	Node outOfBoundsNode;
    void AddToOpenList(int x, int z, std::vector<Node*>& openList, int pathID);
    bool IsNodeWalkable(Node* node);
    static bool SortNodes(const Node* a, const Node* b);

};
