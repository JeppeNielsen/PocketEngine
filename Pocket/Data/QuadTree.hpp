#pragma once
#include "BoundingBox.hpp"
#include "BoundingFrustum.hpp"
#include "Ray.hpp"
#include <vector>

namespace Pocket {
	class Quadtree {
	public:
		struct Node {
            Node() : node(0), data(0) {}
        	Vector2 min;
            Vector2 max;
			Quadtree* node;
			size_t childIndex;
            int order;
			void* data;
		};

		typedef std::vector<Node*> Nodes;

		Quadtree();
		Quadtree(const Vector2& min, const Vector2& max);
		~Quadtree();

		bool Insert(Node& node);
		void Remove(Node& node);
		void Move(Node& node);

		void SetMinMax(const Vector2& min, const Vector2& max);
        
        template<typename T>
        void Get(const Vector2& testMin, const Vector2& testMax, std::vector<T>& list) {
            if (!nodes) return;
            
            bool outside = (testMin.x > max.x || testMax.x < min.x || testMin.y > max.y || testMax.y < min.y);
            
            if (outside) {
                return;
            } else {
            
                //bool inside = testMin.x>=min.x && testMin.y>=min.y && testMax.x<=max.x && testMax.y<=max.y;
                bool inside = min.x>=testMin.x && min.y>=testMin.y && max.x<=testMax.x && max.y<=testMax.y;
            
                if (inside) {
                    GetRecursive<T>(list);
                    return;
                } else {
                    for(int i = 0; i<nodes->size(); i++) {
                        Node* node = nodes->operator[](i);
                        
                        bool outsideNode = (testMin.x > node->max.x || testMax.x < node->min.x || testMin.y > node->max.y || testMax.y < node->min.y);
                        
                        if (!outsideNode) list.push_back((T)node->data);
                    }
                    if (!children) return;
                    for (int i=0; i<4; i++) {
                        children[i].Get<T>(testMin, testMax, list);
                    }
                }
            }
        }
        
        template<typename T>
        void GetRecursive(std::vector<T>& list) {
            if (!nodes) return;
            for	(unsigned i=0;i<nodes->size(); i++) {
                list.push_back((T)nodes->at(i)->data);
            }
            if (!children) return;
            for(int i=0; i<4; i++) children[i].GetRecursive(list);
        }
        
	private:

		Vector2 min;
        Vector2 max;
		Quadtree* parent;
		Quadtree* children;
		
		Nodes* nodes;
		
		void Split();

	};
}
