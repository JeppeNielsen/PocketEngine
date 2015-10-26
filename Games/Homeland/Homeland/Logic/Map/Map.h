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

	struct Node {
    	Node() : height(0), normal(0,1,0), obstacles(0) {}
    	float height;
        Vector3 normal;
        Colour color;
        int obstacles;
    };

    void Reset();
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

    NavTriangle* CalculatePath(Vector2 start, Vector2 end, std::vector<Vector2>& path, int& navMeshVersion, NavTriangle* hintStartTriangle);

    void CreateNavigationMesh(class NavMesh& navMesh);
    bool IsNodeWalkable(const Node& node);

    NavMesh* NavMesh();

    Event<Map*> NavigationUpdated;

    void SetNavMesh(class NavMesh* navMesh);
private:
	Node outOfBoundsNode;
    class NavMesh* navMesh;
};
