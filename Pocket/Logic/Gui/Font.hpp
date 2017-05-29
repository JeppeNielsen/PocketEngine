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
#include "Texture.hpp"
#include "TypeInfo.hpp"
#include "File.hpp"

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace Pocket {
    class Font {
    public:
        Font();
        ~Font();
        
        bool LoadTTF(const std::string& path);
        void Clear();
        void LoadAsset(const std::string& path);
        
        float CharacterSetEverySize;
        float fontSizeToPixelFactor;
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

        void CreateText(std::vector<Letter>& sentence, const std::string& text, Vector2 size, float fontSize, HAlignment hAlign, VAlignment vAlign, bool wordWrap, bool flipY, bool createSpaces = false) const;

        bool IsDirty();

        void UpdateBuffer(Pocket::Texture& texture);
        
        Event<> BufferUpdated;
        Event<> Cleared;
        
        float GetSpacing(float fontSize);
        float GetLineHeightOffset(float fontSize);
        
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
            float lineHeight;
            bool enabled;
        };
        
        using CharacterSets = std::vector<CharacterSet>;
        CharacterSets characterSets;

         void RequestCharacter(CharacterSet& set, unsigned short c);
        CharacterSet& RequestCharacterSet(float fontSize);
        
        const CharacterSet& GetCharacterSet(float fontSize) const;
        
        bool isDirty;
        
        FT_LibraryRec_*    library;
        FT_FaceRec_*       face;
        
        File file;
        Vector2 bbMin;
        
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(CharacterSetEverySize)
        TYPE_FIELD(fontSizeToPixelFactor)
        TYPE_FIELD(maxTextureWidth)
        TYPE_FIELD(maxTextureHeight)
        TYPE_FIELDS_END
    };
}
