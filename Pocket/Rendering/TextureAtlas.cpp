#include "TextureAtlas.hpp"
#include <sstream>

#include "tinyxml.h"
#include "FileReader.hpp"

using namespace Pocket;
using namespace std;

TextureAtlas::TextureAtlas() {
}

TextureAtlas::~TextureAtlas() {
	Free();
}

void TextureAtlas::Free(){
	rectangles.clear();
}

void TextureAtlas::Load(std::string pngFile, std::string textureFile) {
	LoadFromFile(pngFile);

	Vector2 textureSize(GetWidth(), GetHeight());

    textureFile = FileReader::GetFile(textureFile);
    
	TiXmlDocument doc(textureFile.c_str());

	std::stringstream s;
	s<<textureSize.x<< " : "<<textureSize.y;
	//OutputDebugStringA(s.str().c_str());

	bool succes = doc.LoadFile();

	string error = doc.ErrorDesc();

	if (!succes) return;



	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	
	// block: windows
	{

		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) return;

		// save this for later
		hRoot=TiXmlHandle(pElem);

        Vector2 halfPixel = Vector2(0.5f / textureSize.x, 0.5f / textureSize.y);

		TiXmlElement* pWindowNode=hRoot.FirstChild( "Image" ).Element();//.FirstChild("Image").Element();
		for( ; pWindowNode; pWindowNode=pWindowNode->NextSiblingElement())
		{
			string name = pWindowNode->Attribute("name");

			string positionStr = pWindowNode->Attribute("position");
			
			string sizeStr = pWindowNode->Attribute("size");

			Vector2 position = String2Vector(positionStr);
			Vector2 size = String2Vector(sizeStr);

			
			Box box;
			box.left = position.x / textureSize.x - halfPixel.x;
			box.top = position.y / textureSize.y - halfPixel.y;
			box.right = (position.x + size.x) / textureSize.x - halfPixel.x;
			box.bottom = (position.y + size.y) / textureSize.y- halfPixel.y;

			rectangles[name] = box;


			//pWindowNode->QueryIntAttribute("x", &w.x); // If this fails, original value is left as-is
			//pWindowNode->QueryIntAttribute("y", &w.y);
			//pWindowNode->QueryIntAttribute("w", &w.w);
			//pWindowNode->QueryIntAttribute("hh", &w.h);

			//m_windows.push_back(w);

		}
	}
}

Vector2 TextureAtlas::String2Vector(string s) {
	Vector2 v;
	
	size_t l = s.find(",");

	string left = s.substr(0,l);
	string right = s.substr(l+1,s.size() - (l+1));
	
	v.x = atoi(left.c_str());
	v.y = atoi(right.c_str());

	return v;
}

Box TextureAtlas::GetTextureCoords(string name) {
	Rectangles::iterator it = rectangles.find(name);
	if (it!=rectangles.end()) {
		return it->second;
	}
	return Box(0,0,1,1);
}
