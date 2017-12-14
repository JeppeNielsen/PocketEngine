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
#include "OpenGL.hpp"
#include "File.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace Pocket;

Font::Font() :
CharacterSetEverySize(4),
library(0),
maxTextureWidth(512), maxTextureHeight(512), fontSizeToPixelFactor(2.0f), face(0)
{ }

Font::~Font() {
    if (library) {
        FT_Done_Face    ( face );
        FT_Done_FreeType( library );
    }
}

bool Font::LoadTTF(const std::string &path) {
    FT_Error error = FT_Init_FreeType( &library );
    if (error) return false;
    
    //error = FT_New_Face( library, path.c_str(), 0, &face );
    //if (error) return false;
    
     /* FT_New_Memory_Face( FT_Library      library,
                      const FT_Byte*  file_base,
                      FT_Long         file_size,
                      FT_Long         face_index,
                      FT_Face        *aface )
                      */
    
    
    if (!file.Load(path)) {
        return false;
    }
    
    error = FT_New_Memory_Face(library, (const FT_Byte*)file.Data(), (FT_Long)file.Size(), 0, &face);
    if (error) return false;
    
    
    //error = FT_Set_Char_Size( face, 40 * 64, 0, 50, 0 );
    error = FT_Set_Pixel_Sizes(face, 8,0);
    if (error) return false;
    
    bbMin.x = face->bbox.yMin / 64.0f / 24.0f;
    bbMin.y = face->bbox.yMax / 64.0f / 24.0f;
    
    return true;
}

void Font::Clear() {
    characterSets.clear();
    isDirty = true;
    Cleared();
}

void Font::LoadAsset(const std::string &path) {
    LoadTTF(path);
}

const Font::CharacterSet& Font::GetCharacterSet(float fontSize) const {
    if (fontSize<0) fontSize = -fontSize;
    int index = floorf(fontSize / CharacterSetEverySize);
    return characterSets[index];
}

void Font::RequestText(const std::string &text, float fontSize) {
    auto& characterSet = RequestCharacterSet(fontSize);
    auto utf = StringHelper::GetUTF(text);
    for(auto s : utf) {
        RequestCharacter(characterSet, s);
    }
}

float Font::GetSpacing(float fontSize) {
    RequestText(" ", fontSize);
    return GetCharacterSet(fontSize).characters[32].xadvance * fontSize;
}

float Font::GetLineHeightOffset(float fontSize) {
    return bbMin.x * fontSize;
}

Font::CharacterSet& Font::RequestCharacterSet(float fontSize) {
    if (fontSize<0) fontSize = -fontSize;
    int index = floorf(fontSize / CharacterSetEverySize);
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

void Font::RequestCharacter(Font::CharacterSet &set, unsigned short c) {
    if (c>=set.characters.size()) {
        set.characters.resize(c + 1);
    }
    Character& character = set.characters[c];
    if (!character.enabled) {
        character.enabled = true;
        isDirty = true;
    }
}

void Font::CreateText(std::vector<Letter>& sentence, const std::string& text, Vector2 size, float fontSize, HAlignment hAlign, VAlignment vAlign, bool wordWrap, bool flipY, bool createSpaces) const {

    if (text == "") return;

    const CharacterSet& set = GetCharacterSet(fontSize);
    const Characters& characters = set.characters;
    
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
                if (createSpaces) {
                    sentence.push_back({
                        0,0,0,0, 0,0,0,0
                    });
                }
            
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
				sentence.at(i).y += lineHeight;
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

bool Font::IsDirty() { return isDirty; }

void Font::UpdateBuffer(Pocket::Texture& texture) {
    if (!face) return;
    isDirty = false;
    
    int maxWidth = maxTextureWidth;
    int maxHeight = maxTextureHeight;
    
    RectPacker packer;
    
    for (int s = 0; s<characterSets.size(); ++s) {
        CharacterSet& set = characterSets[s];
        if (!set.enabled) continue;
        int faceSize = (CharacterSetEverySize + CharacterSetEverySize * s) * fontSizeToPixelFactor;
        FT_Error error = FT_Set_Pixel_Sizes(face, faceSize ,0);
        if (error) continue;
        
        set.lineHeight = 0;
        for(int c = 0; c<set.characters.size(); ++c) {
            Character& character = set.characters[c];
            if (!character.enabled) continue;
            
            FT_Error error = FT_Load_Char( face, (unsigned long)c, FT_LOAD_NO_BITMAP );
            if (error) continue;
            
            FT_GlyphSlot g = face->glyph;
            
            int width = (int)g->metrics.width / 64;
            int height = (int)g->metrics.height / 64;
            int offsetX = (int)g->metrics.horiBearingX / 64;
            int offsetY = (int)g->metrics.horiBearingY / 64;
            int advanceX = (int)g->metrics.horiAdvance / 64;
            
            RectPacker::TRect rect;
            rect.w = width+2;
            rect.h = height+2;
            
            const float scale = 1.0f/faceSize;
            
            if (packer.AddAtEmptySpotAutoGrow(&rect, maxWidth, maxHeight)) {
                character.width = width * scale;
                character.height = height * scale;
                character.xoffset = offsetX * scale;
                character.yoffset = (-offsetY) * scale;
                character.xadvance = (advanceX - 1) * scale;
                
                character.textureX = rect.x;
                character.textureY = rect.y;
                character.textureWidth = rect.w;
                character.textureHeight = rect.h;
                
                if (character.height>set.lineHeight) {
                    set.lineHeight = character.height;
                }
                //WriteCharacter(g->bitmap, rect.x+1, rect.y+1);
            } else {
                character.enabled = false;
            }
        }
    }
    
    int actualWidth = packer.GetW();
    int actualHeight = packer.GetH();
  
    texture.CreateFromBuffer(0, actualWidth, actualHeight, GL_RGBA);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (int s = 0; s<characterSets.size(); ++s) {
        CharacterSet& set = characterSets[s];
        if (!set.enabled) continue;
        int faceSize = (CharacterSetEverySize + CharacterSetEverySize * s) * fontSizeToPixelFactor;
        FT_Error error = FT_Set_Pixel_Sizes(face, faceSize ,0);
        if (error) continue;

        for(int c = 0; c<set.characters.size(); ++c) {
            Character& character = set.characters[c];
            if (!character.enabled) continue;
            
            FT_Error error = FT_Load_Char( face, (unsigned long)c, FT_LOAD_RENDER );
            if (error) continue;

            FT_GlyphSlot g = face->glyph;
            
            int bitmapWidth = g->bitmap.width;
            
            int bitmapSize = g->bitmap.width * g->bitmap.rows;
            int bufferWidth = g->bitmap.width + 2;
            int bufferHeight = g->bitmap.rows + 2;
            unsigned char* buffer = new unsigned char[bufferWidth*bufferHeight * 4];
            
            int px = 0;
            int py = 0;
            
            for(int i=0; i<bufferWidth; ++i) {
                buffer[px] = 255;
                buffer[px+1] = 255;
                buffer[px+2] = 255;
                buffer[px+3] = 0;
                px+=4;
            }
            px = bufferWidth * (bufferHeight - 1) * 4;
            for(int i=0; i<bufferWidth; ++i) {
                buffer[px] = 255;
                buffer[px+1] = 255;
                buffer[px+2] = 255;
                buffer[px+3] = 0;
                px+=4;
            }
            
            px = 0;
            py = bufferWidth*4;
            for(int i=0; i<bufferHeight; ++i) {
                buffer[px] = 255;
                buffer[px+1] = 255;
                buffer[px+2] = 255;
                buffer[px+3] = 0;
                px+=py;
            }
            
            px = (bufferWidth - 1)*4;
            py = bufferWidth*4;
            for(int i=0; i<bufferHeight; ++i) {
                buffer[px] = 255;
                buffer[px+1] = 255;
                buffer[px+2] = 255;
                buffer[px+3] = 0;
                px+=py;
            }
            
            int baseIndex = (bufferWidth + 1) * 4;
            px = 0;
            py = 0;
            
            for(int i=0; i<bitmapSize; ++i) {
                int index = baseIndex + px * 4 + py * bufferWidth * 4;
                buffer[index] = 255;
                buffer[index+1] = 255;
                buffer[index+2] = 255;
                buffer[index+3] = g->bitmap.buffer[i];
                
                px++;
                if (px==bitmapWidth) {
                    px = 0;
                    py++;
                }
             }
            
            ASSERT_GL(glTexSubImage2D(GL_TEXTURE_2D, 0,character.textureX, character.textureY, character.textureWidth, character.textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
            delete[] buffer;
        
            character.textureX = (character.textureX + 1) / actualWidth;
            character.textureY = (character.textureY + 1) / actualHeight;
            character.textureWidth = (character.textureWidth - 2 ) / actualWidth;
            character.textureHeight = (character.textureHeight - 2) / actualHeight;
        }
    }
    //texture.GenerateMipmaps();
    //texture.SaveToTga("/Users/Jeppe/Downloads/ImageSaverCode/FontTexture.tga", GL_RGBA);
   
    BufferUpdated();
}
