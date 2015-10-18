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
#include "NavMesh.hpp"
#include "RenderSystem.hpp"

using namespace Pocket;

struct MapRect {
    int x;
    int z;
    int width;
    int depth;
};

Component(Map)

    RenderSystem* renderSystem;

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

    void CalculatePath(Vector3 start, Vector3 end, std::vector<Vector3>& path, GameWorld* world);

    std::vector<Vector2> CreateNavigationMesh();
    bool IsNodeWalkable(const Node& node);

    void CreateTriangleObject(NavTriangle* tri, GameWorld* world);

private:
	Node outOfBoundsNode;
    void AddToOpenList(int x, int z, std::vector<Node*>& openList, int pathID);
    bool IsNodeWalkable(Node* node);
    static bool SortNodes(const Node* a, const Node* b);
    NavMesh navMesh;
};
