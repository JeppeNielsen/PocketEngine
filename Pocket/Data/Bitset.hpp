//
//  Bitset.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 19/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>

namespace Pocket {
  class Bitset {
  public:
        bool operator[] (int index) const;
        void Set(int index, bool value);
        bool Contains(const Bitset& bitset) const;
        void Resize(int index);
        void Reset();
        int Size();
  private:
      using Bits = std::vector<bool>;
      Bits bits;
  };
}