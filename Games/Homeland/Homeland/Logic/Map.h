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

using namespace Pocket;

struct MapRect {
    int x;
    int z;
    int width;
    int depth;
};

Component(Map)

	struct Node {
    	Node() : height(0), normal(0,1,0) {}
    	float height;
        Vector3 normal;
        Colour color;
    };

    void CreateMap(int width, int depth);
	typedef std::vector<std::vector<Node>> Nodes;

    Nodes nodes;

	Node& GetNode(int x, int z);

	int Width();
    int Depth();

    void Randomize(float minHeight, float maxHeight);
    void Smooth(int iterations);

    void CalcNormals();
    void CalcNormals(const MapRect& rect);

private:
	Node outOfBoundsNode;

};
