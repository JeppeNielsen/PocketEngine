#pragma once
#include "BoundingBox.hpp"
#include "BoundingFrustum.hpp"
#include "Ray.hpp"
#include <vector>

namespace Pocket {
	class Octree
	{
	public:

		

		struct Node {
			BoundingBox box;
			Octree* node;
			size_t childIndex;
            int order;
			void* data;
		};

		typedef std::vector<Node*> Nodes;

		Octree();
		Octree(const BoundingBox& box);
		~Octree();

		bool Insert(Node& node);
		void Remove(Node& node);
		void Move(Node& node);

		void Get(const BoundingFrustum& frustum, Nodes& list);

		void SetBoundingBox(const BoundingBox& box);
        
        template<typename T>
        void Get(const BoundingFrustum& frustum, std::vector<T>& list) {
            if (!nodes) return;
            
            BoundingFrustum::Intersection test = frustum.Intersect(box);
            
            if (test == BoundingFrustum::OUTSIDE) {
                return;
            } else if (test == BoundingFrustum::INSIDE) {
                GetRecursive<T>(list);
                return;
            } else {
                for(size_t i = 0, size=nodes->size(); i<size; i++) {
                    Node* node = nodes->at(i);
                    
                    if (frustum.Intersect(node->box)!=BoundingFrustum::OUTSIDE) list.push_back((T)node->data);
                    //list.push_back(node);
                }
                if (!children) return;
                for (int i=0; i<8; i++) {
                    children[i].Get<T>(frustum, list);
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
            for(int i=0; i<8; i++) children[i].GetRecursive(list);
        }
        
        template<typename T>
        void Get(const Ray& ray, std::vector<T>& list) {
            if (!nodes) return;
            
            if (!ray.Intersect(box)) {
                return;
            }
            else {
                for(size_t i = 0, size=nodes->size(); i<size; i++) {
                    Node* node = nodes->at(i);
                    
                    if (ray.Intersect(node->box)) list.push_back((T)node->data);
                }
                if (!children) return;
                for (int i=0; i<8; i++) {
                    children[i].Get<T>(ray, list);
                }
            }
        }
        
        

	private:

		BoundingBox box;
		Octree* parent;
		Octree* children;
		
		Nodes* nodes;
		
		void Split();

		void GetRecursive(Nodes& list);


	};
}
