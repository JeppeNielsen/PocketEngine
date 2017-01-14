//
//  Font.cpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Font.hpp"
#include <cmath>
#include "StringHelper.hpp"
#include "RectPacker.hpp"

using namespace std;

Font::Font() :
CharacterSetEverySize(12),
buffer(0), bufferWidth(0), bufferHeight(0), library(0),
maxTextureWidth(512), maxTextureHeight(512)
{ }

Font::~Font() {
    if (library) {
        FT_Done_Face    ( face );
        FT_Done_FreeType( library );
    }
    DestroyBuffer();
}

bool Font::LoadTTF(const std::string &path) {
    FT_Error error = FT_Init_FreeType( &library );
    if (error) return false;
    
    error = FT_New_Face( library, path.c_str(), 0, &face );
    if (error) return false;
    
    //error = FT_Set_Char_Size( face, 40 * 64, 0, 50, 0 );
    error = FT_Set_Pixel_Sizes(face, 8,0);
    if (error) return false;
    
    return true;
}

const Font::CharacterSet& Font::GetCharacterSet(float fontSize) const {
    int index = floorf(fontSize / CharacterSetEverySize);
    return characterSets[index];
}

void Font::RequestText(const std::string &text, float fontSize) {
    auto& characterSet = RequestCharacterSet(fontSize);
    for(auto& c : text) {
        RequestCharacter(characterSet, c);
    }
}

Font::CharacterSet& Font::RequestCharacterSet(float fontSize) {
    int index = floorf(fontSize / CharacterSetEverySize);
    if (index<0) index = 0;
    if (index>=characterSets.size()) {
        characterSets.resize(index + 1);
    }
    auto& characterSet = characterSets[index];
    if (!characterSet.enabled) {
        characterSet.enabled = true;
        isDirty = true;
    }
    return characterSet;
}

void Font::RequestCharacter(Font::CharacterSet &set, char c) {
    if (c>=set.characters.size()) {
        set.characters.resize(c + 1);
    }
    Character& character = set.characters[c];
    if (!character.enabled) {
        character.enabled = true;
        isDirty = true;
    }
}

void Font::CreateText(std::vector<Letter>& sentence, std::string text, Vector2 size, float fontSize, HAlignment hAlign, VAlignment vAlign, bool wordWrap, bool flipY) const {

    const CharacterSet& set = GetCharacterSet(fontSize);
    const Characters characters = set.characters;
    
	float space = characters[32].xadvance * fontSize;
	float lineSpace = set.lineHeight * fontSize;
    float lineHeight = lineSpace;
    
    vector<string> words;
	if (wordWrap) {
		StringHelper::split(text, ' ', words);
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
            
            if (id<0 || id>=characters.size()) {
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
            
			if (hAlign == Font::HAlignment::Center) {
				for (size_t i=startLine; i<startIndex; i++) {
					sentence.at(i).x += ((size.x * 0.5f - width * 0.5f) - offset);
				}
			} else if (hAlign == Font::HAlignment::Right) {
				
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
    
	if (hAlign == Font::HAlignment::Center) {
		float width = (sentence.at(sentence.size()-1).x + sentence.at(sentence.size()-1).width) - sentence.at(startLine).x;// - space;
		float offset = sentence.at(startLine).x;
		for (size_t i=startLine; i<(int)sentence.size(); i++) {
			sentence.at(i).x += ((size.x * 0.5f - width * 0.5f) - offset);
		}
	} else if (hAlign == Font::HAlignment::Right) {
		float width = (sentence.at(sentence.size()-1).x + sentence.at(sentence.size()-1).width) - sentence.at(startLine).x;
		float offset = sentence.at(startLine).x;
		for (size_t i=startLine; i<(int)sentence.size(); i++) {
			sentence.at(i).x += ((size.x - width) - offset);
		}
	}
    
	if (vAlign == Font::VAlignment::Middle) {
		float height = (sentence.at(sentence.size()-1).y + sentence.at(sentence.size()-1).height) - sentence.at(0).y;
		float delta = (size.y * 0.5f - height * 0.5f) - sentence.at(0).y;
		for(int i=0; i<(int)sentence.size(); i++) {
			sentence.at(i).y += delta;
		}
	} else if (vAlign == Font::VAlignment::Bottom) {
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

bool Font::UpdateBuffer() {
    if (!isDirty) return false;
    isDirty = false;
    
    int maxWidth = maxTextureWidth;
    int maxHeight = maxTextureHeight;
    
    AllocateBuffer(maxWidth, maxHeight);
    for(int i=0; i<bufferSize; ++i) {
        buffer[i]=0;
    }
    
    RectPacker packer;
    
    for (int s = 0; s<characterSets.size(); ++s) {
        CharacterSet& set = characterSets[s];
        if (!set.enabled) continue;
        int faceSize = CharacterSetEverySize + CharacterSetEverySize * s;
        FT_Error error = FT_Set_Pixel_Sizes(face, faceSize ,0);
        if (error) continue;
        
        set.lineHeight = 0;
        for(int c = 0; c<set.characters.size(); ++c) {
            Character& character = set.characters[c];
            if (!character.enabled) continue;
            
            FT_Error error = FT_Load_Char( face, (unsigned long)c, FT_LOAD_RENDER );
            if (error) continue;
            
            FT_GlyphSlot g = face->glyph;
            
            RectPacker::TRect rect;
            rect.w = g->bitmap.width+2;
            rect.h = g->bitmap.rows+2;
            
            const float scale = 1.0f/faceSize;
            
            if (packer.AddAtEmptySpotAutoGrow(&rect, maxWidth, maxHeight)) {
                character.width = g->bitmap.width * scale;
                character.height = g->bitmap.rows * scale;
                character.xoffset = g->bitmap_left * scale;
                character.yoffset = (-g->bitmap_top) * scale;
                character.xadvance = g->advance.x * scale / 64.0f;
                
                character.textureX = (float)(rect.x+1) / maxWidth;
                character.textureY = (float)(rect.y+1) / maxHeight;
                character.textureWidth = (float)g->bitmap.width / maxWidth;
                character.textureHeight = (float)g->bitmap.rows / maxHeight;
                
                if (character.height>set.lineHeight) {
                    set.lineHeight = character.height;
                }
                WriteCharacter(g->bitmap, rect.x+1, rect.y+1);
            }
        }
    }
    
    
   
    BufferUpdated();
    return true;
}

void Font::WriteCharacter(FT_Bitmap& bitmap, int px, int py) {
    for (int x = 0; x<bitmap.width; ++x) {
        for (int y=0; y<bitmap.rows; ++y) {
            unsigned char pixel = bitmap.buffer[y * bitmap.width + x];
            
            buffer[(px+x) + (y+py) * bufferWidth]=pixel;
            //WritePixel(x, y, pixel);
            
            //buffer[x + y * bufferWidth] = pixel;
        }
    }
}

void Font::WritePixel(int x, int y, unsigned char brightness) {
    int offset = x + y * bufferWidth * 3;
    buffer[offset] = brightness;
    buffer[offset+1] = brightness;
    buffer[offset+2] = brightness;
}

void Font::AllocateBuffer(int width, int height) {
    if (width == bufferWidth && height == bufferHeight && !buffer) return;
    bufferWidth = width;
    bufferHeight = height;
    bufferSize = bufferWidth * bufferHeight;
    buffer = new unsigned char[bufferSize];
}

void Font::DestroyBuffer() {
    delete [] buffer;
    buffer = 0;
}

