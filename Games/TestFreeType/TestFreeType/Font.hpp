//
//  Font.hpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Vector2.hpp"
#include "Event.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Texture.hpp"

using namespace Pocket;

class Font {
public:
    Font();
    ~Font();
    
    bool LoadTTF(const std::string& path);
    void Clear();
    
    float CharacterSetEverySize;
    int maxTextureWidth;
    int maxTextureHeight;
    
    struct Letter {
        float x;
        float y;
        float width;
        float height;

        float u1;
        float v1;
        float u2;
        float v2;
    };

    enum class HAlignment { Left, Center, Right };
    enum class VAlignment { Top, Middle, Bottom };
    
    void RequestText(const std::string& text, float fontSize);

    void CreateText(std::vector<Letter>& sentence, std::string text, Vector2 size, float fontSize, HAlignment hAlign, VAlignment vAlign, bool wordWrap, bool flipY) const;

    bool IsDirty();

    void UpdateBuffer(Pocket::Texture& texture);
    
    Event<> BufferUpdated;
    Event<> Cleared;
    
private:
    struct Character {
        Character() : enabled(false) {}
        float textureX;
        float textureY;
        float textureWidth;
        float textureHeight;
        float width;
        float height;
        float xoffset;
        float yoffset;
        float xadvance;
        bool enabled;
    };
    
    using Characters = std::vector<Character>;
    
    struct CharacterSet {
        CharacterSet() : enabled(false) { }
        Characters characters;
        float spacing;
        float lineHeight;
        bool enabled;
        const Character& GetCharacter(char c);
    };
    
    using CharacterSets = std::vector<CharacterSet>;
    CharacterSets characterSets;

     void RequestCharacter(CharacterSet& set, char c);
    CharacterSet& RequestCharacterSet(float fontSize);
    
    const CharacterSet& GetCharacterSet(float fontSize) const;
    
    bool isDirty;
    
    FT_Library    library;
    FT_Face       face;
};
