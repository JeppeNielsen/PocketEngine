#include "Font.hpp"
#include "tinyxml.h"
#include <sstream>
#include "FileReader.hpp"
#include "StringHelper.hpp"

using namespace Pocket;
using namespace std;

Font::Font(){
}

Font::~Font(){
}

void Font::Load(std::string filename, std::string atlasNode) {

    FontAtlasNode = atlasNode;
    
	std::stringstream ss(filename);
    std::string item;
    while(std::getline(ss, item, '/')) {
    }
	
    filename = FileReader::GetFile(filename);
    
    

    /*
	TiXmlDocument doc(filename.c_str());

	bool succes = doc.LoadFile();

	string error = doc.ErrorDesc();

	if (!succes) return;
     */
    
    //const char* file = CCFileUtils::fullPathFromRelativePath(filename.c_str());
    
    TiXmlDocument a;
    
    
	TiXmlDocument doc(filename.c_str());
    
	bool succes = doc.LoadFile();
    
	string error = doc.ErrorDesc();
    
	if (!succes) return;



	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	
	{

		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) return;

		// save this for later
		hRoot=TiXmlHandle(pElem);

		TiXmlElement* commonNode = hRoot.FirstChild( "common" ).Element();

		
		baseSize = (float)atoi(commonNode->Attribute("base"));
		lineHeight = atoi(commonNode->Attribute("lineHeight")) / baseSize;

		float textureWidth = (float)atoi(commonNode->Attribute("scaleW"));
		float textureHeight = (float)atoi(commonNode->Attribute("scaleH"));

		//.FirstChild("Image").Element();
		
		for( TiXmlElement* pWindowNode = hRoot.FirstChild( "chars" ).FirstChild("char").Element();
            pWindowNode; pWindowNode=pWindowNode->NextSiblingElement())
		{

			int id = atoi(pWindowNode->Attribute("id"));

			if (id<0 || id>=MaxCharacters) continue;

			characters[id].textureX = ((float)atoi(pWindowNode->Attribute("x"))) / textureWidth;
			characters[id].textureY = ((float)atoi(pWindowNode->Attribute("y"))) / textureHeight;
			characters[id].textureWidth = ((float)atoi(pWindowNode->Attribute("width"))) / textureWidth;
			characters[id].textureHeight = ((float)atoi(pWindowNode->Attribute("height"))) / textureHeight;
			characters[id].width = ((float)atoi(pWindowNode->Attribute("width"))) / baseSize;
			characters[id].height = ((float)atoi(pWindowNode->Attribute("height"))) / baseSize;
			characters[id].xoffset = ((float)atoi(pWindowNode->Attribute("xoffset"))) / baseSize;
			characters[id].yoffset = ((float)atoi(pWindowNode->Attribute("yoffset"))) / baseSize;
			characters[id].xadvance = ((float)atoi(pWindowNode->Attribute("xadvance"))) / baseSize;
					
			//pWindowNode->QueryIntAttribute("x", &w.x); // If this fails, original value is left as-is
			//pWindowNode->QueryIntAttribute("y", &w.y);
			//pWindowNode->QueryIntAttribute("w", &w.w);
			//pWindowNode->QueryIntAttribute("hh", &w.h);

			//m_windows.push_back(w);

		}
	}


}

Font::Character::Character() {
	textureX=0;
	textureY=0;
	textureWidth=0;
	textureHeight=0;
	width=0;
	height=0;
	xoffset=0;
	yoffset=0;
	xadvance=0;
}

void Font::CreateText(std::vector<Letter>& sentence, std::string text, Vector2 size, float fontSize, HAlignment hAlign, VAlignment vAlign, bool wordWrap, bool flipY) const {
    
	float space = characters[32].xadvance * fontSize;
	float lineSpace = lineHeight * fontSize;
    
    vector<string> words;
	if (wordWrap) {
		split(text, ' ', words);
	} else {
		words.push_back(text);
	}
    
	size_t startLine = 0;
	Vector2 p(0,0);
    const size_t wordCount = words.size();
	for(int wordNo=0; wordNo<wordCount; wordNo++) {
		//string& word = words[wordNo];
        
        vector<unsigned short> word = StringHelper::GetUTF(words[wordNo]);
        
        /*
        wchar_t* wideBuffer = new wchar_t[ word.length() ];
        size_t num_chars = mbstowcs( wideBuffer, word.c_str(), word.length() );
        wstring wideWord( wideBuffer, num_chars );
        delete[] wideBuffer;
        */
        
        
        
		size_t startIndex = sentence.size();
		float wordStartX = p.x;
		//const int wordSize = wideWord.size();
		for (unsigned i=0; i<word.size(); i++) {
            //int id = (int)word[i];
            unsigned short id = word[i];
                       
            if (id==10) {
                p.x = 0;
                p.y += lineSpace;
                continue;
            }
            
            if (id<0 || id>=MaxCharacters) {
                continue;
            }
			if (id==32) {
				p.x+=space;
				continue;
			}
            
			Letter l;
			l.u1 = characters[id].textureX;
			l.v1 = characters[id].textureY;
			l.u2 = l.u1 + characters[id].textureWidth;
			l.v2 = l.v1 + characters[id].textureHeight;
            
			l.x = p.x + characters[id].xoffset * fontSize;
			l.y = p.y + characters[id].yoffset * fontSize;
			l.width = characters[id].width * fontSize;
			l.height = characters[id].height * fontSize;
			p.x += characters[id].xadvance * fontSize;
			sentence.push_back(l);
		}
        
		if (p.x>size.x && startIndex>0) {
			
			float width = (sentence.at(startIndex - 1).x + sentence.at(startIndex - 1).width) - sentence.at(startLine).x;
			float offset = sentence.at(startLine).x;
            
			for (size_t i=startIndex; i<sentence.size(); i++) {
				sentence.at(i).x -= (wordStartX);
				sentence.at(i).y += lineHeight * fontSize;
			}
            
			p.x = p.x - wordStartX + space;
			p.y += lineSpace;
            
			if (hAlign == Center) {
				for (size_t i=startLine; i<startIndex; i++) {
					sentence.at(i).x += ((size.x * 0.5f - width * 0.5f) - offset);
				}
			} else if (hAlign == Right) {
				
				for (size_t i=startLine; i<startIndex; i++) {
					sentence.at(i).x += ((size.x - width) - offset);
				}
			}
            
			startLine = startIndex;	
			
		}
		else {
			p.x += space;
		}
	}
    
	if (sentence.size()<=0) return;
    
	if (hAlign == Center) {
		float width = (sentence.at(sentence.size()-1).x + sentence.at(sentence.size()-1).width) - sentence.at(startLine).x;// - space;
		float offset = sentence.at(startLine).x;
		for (size_t i=startLine; i<(int)sentence.size(); i++) {
			sentence.at(i).x += ((size.x * 0.5f - width * 0.5f) - offset);
		}
	} else if (hAlign == Right) {
		float width = (sentence.at(sentence.size()-1).x + sentence.at(sentence.size()-1).width) - sentence.at(startLine).x;
		float offset = sentence.at(startLine).x;
		for (size_t i=startLine; i<(int)sentence.size(); i++) {
			sentence.at(i).x += ((size.x - width) - offset);
		}
	}
    
	if (vAlign == Middle) {
		float height = (sentence.at(sentence.size()-1).y + sentence.at(sentence.size()-1).height) - sentence.at(0).y;
		float delta = (size.y * 0.5f - height * 0.5f) - sentence.at(0).y;
		for(int i=0; i<(int)sentence.size(); i++) {
			sentence.at(i).y += delta;
		}
	} else if (vAlign == Bottom) {
		float height = (sentence.at(sentence.size()-1).y + sentence.at(sentence.size()-1).height) - sentence.at(0).y;
		float delta = (size.y - height) - sentence.at(0).y;
		for(int i=0; i<(int)sentence.size(); i++) {
			sentence.at(i).y += delta;
		}
	}
    
    if (flipY) {
        for (unsigned i=0; i<sentence.size(); i++) {
            sentence.at(i).y = size.y - (sentence.at(i).y + sentence.at(i).height);
        }
    }
    
}

/*
void Font::CreateText_old(vector<Letter>* sentence, string text, Vector2 size, float fontSize, HAlignment hAlign, VAlignment vAlign, bool wordWrap) {

	float space = characters[32].xadvance * fontSize;
	float lineSpace = lineHeight * fontSize;

	vector<string> words;
	if (wordWrap) {
		split(text, ' ', words);
	} else {
		words.push_back(text);
	}

	int startLine = 0;
	Vector2 p(0,0);
    const int wordCount = words.size();
	for(int wordNo=0; wordNo<wordCount; wordNo++) {
		string word = words[wordNo];

		int startIndex = sentence->size();
		float wordStartX = p.x;
		const int wordSize = word.size();
		for (int i=0; i<wordSize; i++) {
			int id = (int)word[i];
			if (id<0 || id>=MaxCharacters) continue;
			if (id==32) {
				p.x+=baseSize;
				continue;
			}

			Letter l;
			l.u1 = characters[id].textureX;
			l.v1 = characters[id].textureY;
			l.u2 = l.u1 + characters[id].textureWidth;
			l.v2 = l.v1 + characters[id].textureHeight;

			l.x = p.x + characters[id].xoffset * fontSize;
			l.y = p.y + characters[id].yoffset * fontSize;
			l.width = characters[id].width * fontSize;
			l.height = characters[id].height * fontSize;
			p.x += characters[id].xadvance * fontSize;
			sentence->push_back(l);
		}

		if (p.x>size.x && startIndex>0) {
			
			float width = (sentence->at(startIndex - 1).x + sentence->at(startIndex - 1).width) - sentence->at(startLine).x;
			float offset = sentence->at(startLine).x;

			for (int i=startIndex; i<(int)sentence->size(); i++) {
				sentence->at(i).x -= (wordStartX);
				sentence->at(i).y += lineHeight * fontSize;
			}

			p.x = p.x - wordStartX + space;
			p.y += lineSpace;

			if (hAlign == Center) {
				for (int i=startLine; i<startIndex; i++) {
					sentence->at(i).x += ((size.x * 0.5f - width * 0.5f) - offset);
				}
			} else if (hAlign == Right) {
				
				for (int i=startLine; i<startIndex; i++) {
					sentence->at(i).x += ((size.x - width) - offset);
				}
			}

			startLine = startIndex;	
			
		}
		else {
			p.x += space;
		}
	}

	if (sentence->size()<=0) return;
				
	if (hAlign == Center) {
		float width = (sentence->at(sentence->size()-1).x + sentence->at(sentence->size()-1).width) - sentence->at(startLine).x;// - space;
		float offset = sentence->at(startLine).x;
		for (int i=startLine; i<(int)sentence->size(); i++) {
			sentence->at(i).x += ((size.x * 0.5f - width * 0.5f) - offset);
		}
	} else if (hAlign == Right) {
		float width = (sentence->at(sentence->size()-1).x + sentence->at(sentence->size()-1).width) - sentence->at(startLine).x;
		float offset = sentence->at(startLine).x;
		for (int i=startLine; i<(int)sentence->size(); i++) {
			sentence->at(i).x += ((size.x - width) - offset);
		}
	}

	if (vAlign == Middle) {
		float height = (sentence->at(sentence->size()-1).y + sentence->at(sentence->size()-1).height) - sentence->at(0).y;
		float delta = (size.y * 0.5f - height * 0.5f) - sentence->at(0).y;
		for(int i=0; i<(int)sentence->size(); i++) {
			sentence->at(i).y += delta;
		}
	} else if (vAlign == Bottom) {
		float height = (sentence->at(sentence->size()-1).y + sentence->at(sentence->size()-1).height) - sentence->at(0).y;
		float delta = (size.y - height) - sentence->at(0).y;
		for(int i=0; i<(int)sentence->size(); i++) {
			sentence->at(i).y += delta;
		}
	}

    for (int i=0; i<sentence->size(); i++) {
        sentence->at(i).y = size.y - (sentence->at(i).y + sentence->at(i).height);
    }

}
 */

void Font::split(const std::string &s, char delim, std::vector<std::string> &elems) const {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

void Font::Reset() { }
void Font::Clone(const Pocket::Font &source) { }
void Font::Load() {}
void Font::Save() {}
