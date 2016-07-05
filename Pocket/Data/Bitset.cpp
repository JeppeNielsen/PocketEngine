//
//  Bitset.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 19/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Bitset.hpp"

using namespace Pocket;

bool Bitset::operator[](int index) const {
    return bits[index];
}

void Bitset::Set(int index, bool value) {
    bits[index] = value;
}

void Bitset::Resize(int index) {
    bits.resize(index, false);
}

bool Bitset::Contains(const Pocket::Bitset &bitset) const {
    for(int i=0; i<bits.size();++i) {
        if (bits[i] && !bitset.bits[i]) {
            return false;
        }
    }
    return true;
}

void Bitset::Reset() {
    for(int i=0; i<bits.size();++i){
        bits[i]=false;
    }
}

int Bitset::Size() {
    return (int)bits.size();
}