//
//  Atlas.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Atlas.hpp"
#include <sstream>
#include "tinyxml.h"
#include "FileReader.hpp"
#include "StringHelper.hpp"
#include <fstream>

using namespace Pocket;
using namespace std;

Atlas::Atlas()  {
    defaultNode.inner = defaultNode.outer = Box(0,0,1,1);
    nodes.clear();
}

bool Atlas::Load(std::string textureFile, const Vector2& textureSize) {
    
    textureFile = FileReader::GetFile(textureFile);
    
	TiXmlDocument doc(textureFile.c_str());
    
	//std::stringstream s;
	//s<<textureSize.x<< " : "<<textureSize.y;
	//OutputDebugStringA(s.str().c_str());
    
	bool succes = doc.LoadFile();
    
	string error = doc.ErrorDesc();
    
	if (!succes) return false;

    this->textureSize = textureSize;
    
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	
	// block: windows
	{
        
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) return false;
        
		// save this for later
		hRoot=TiXmlHandle(pElem);
        
        Vector2 halfPixel = Vector2(0.5f / textureSize.x, 0.5f / textureSize.y);
        
		TiXmlElement* pWindowNode=hRoot.FirstChild( "Image" ).Element();//.FirstChild("Image").Element();
		for( ; pWindowNode; pWindowNode=pWindowNode->NextSiblingElement())
		{
            TiXmlAttribute* nameAttribute = 0;
            TiXmlAttribute* positionAttribute = 0;
            TiXmlAttribute* sizeAttribute = 0;
            TiXmlAttribute* offsetAttribute = 0;
            
            TiXmlAttribute* a = pWindowNode->FirstAttribute();
            while (a) {
                std::string aName = a->Name();
                if (aName == "name") nameAttribute = a;
                else if (aName == "position") positionAttribute = a;
                else if (aName == "size") sizeAttribute = a;
                else if (aName == "offset") offsetAttribute = a;
                a = a->Next();
            }
            
            if (!nameAttribute) continue;
            if (!positionAttribute) continue;
            if (!sizeAttribute) continue;
            
			Vector2 position = String2Vector(positionAttribute->Value());
			Vector2 size = String2Vector(sizeAttribute->Value());
            
            Node node;
			
			node.outer.left = position.x / textureSize.x + halfPixel.x;
			node.outer.top = position.y / textureSize.y + halfPixel.y;
			node.outer.right = (position.x + size.x) / textureSize.x - halfPixel.x;
			node.outer.bottom = (position.y + size.y) / textureSize.y - halfPixel.y;
            
            Vector2 topOffset = 4;
            Vector2 bottomOffset = 4;
            
            if (offsetAttribute) {
                std::vector<float> values;
                StringHelper::splitToFloats(offsetAttribute->Value(), ',', values);
                if (values.size()>=4) {
                    topOffset.x = values[0];
                    topOffset.y = values[1];
                    bottomOffset.x = values[2];
                    bottomOffset.y = values[3];
                }
            }
            
            node.inner.left = (position.x + topOffset.x) / textureSize.x - halfPixel.x;
			node.inner.top = (position.y + topOffset.y) / textureSize.y - halfPixel.y;
			node.inner.right = (position.x + size.x - bottomOffset.x) / textureSize.x - halfPixel.x;
			node.inner.bottom = (position.y + size.y - bottomOffset.y) / textureSize.y- halfPixel.y;
            
			nodes[nameAttribute->Value()] = node;
            
		}
	}
    
    return true;
}

Vector2 Atlas::String2Vector(string s) {
	Vector2 v;
	
	size_t l = s.find(",");
    
	string left = s.substr(0,l);
	string right = s.substr(l+1,s.size() - (l+1));
	
	v.x = atoi(left.c_str());
	v.y = atoi(right.c_str());
    
	return v;
}

const Atlas::Node& Atlas::GetNode(string name) {
	Nodes::iterator it = nodes.find(name);
	if (it!=nodes.end()) {
		return it->second;
	}
	return defaultNode;
}

const Vector2& Atlas::TextureSize() const {
    return textureSize;
}

bool Atlas::LoadSpineAtlas(std::string filename) {

    filename = FileReader::GetFile(filename);
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) return false;
    
    Vector2 textureSize;
    
    while (!file.eof()) {
    
        std::string line;
        file>>line;
        
        if (line == "size:") {
            std::string size;
            file>>size;
            textureSize = String2Vector(size);
        } else if (line=="repeat:") {
            if (!file.eof()) {
                string dummy;
                file>>dummy;
            }
            break;
        }
        
        //std::cout<<line<<std::endl;
    }
    
    Vector2 halfPixel = Vector2(0.5f / textureSize.x, 0.5f / textureSize.y);
    
    Node* node = 0;
    Vector2 position;
    
    while (!file.eof()) {
    
        string key;
        file>>key;
        
        string value;
        
        if (key == "rotate:") {
            file>>value;
        } else if (key=="xy:") {
            file>>value;
            string next;
            file>>next;
            value+=next;
            position = String2Vector(value);
            if (!node) {
                continue;
            }
            
            node->outer.left = position.x / textureSize.x + halfPixel.x;
			node->outer.top = position.y / textureSize.y + halfPixel.y;
            node->inner.left = node->outer.left;
            node->inner.top = node->outer.top;
        } else if (key=="size:") {
            file>>value;
            string next;
            file>>next;
            value+=next;
            Vector2 size = String2Vector(value);
            if (!node) {
                continue;
            }
        
            node->outer.right = (position.x + size.x) / textureSize.x - halfPixel.x;
			node->outer.bottom = (position.y + size.y) / textureSize.y - halfPixel.y;
            node->inner.right = node->outer.right;
            node->inner.bottom = node->outer.bottom;
        } else if (key == "orig:") {
            file>>value;
            file>>value;
        } else if (key == "offset:") {
            file>>value;
            file>>value;
        } else if (key == "index:") {
            file>>value;
        } else if (key.length()>0){
            node = &nodes[key];
        }
    }
    
    /*
    for (Nodes::iterator it = nodes.begin(); it!=nodes.end(); ++it) {
        std::cout<<it->first<<" = " <<it->second.inner<<std::endl;
    }
    */
    
    file.close();
    return true;
}






