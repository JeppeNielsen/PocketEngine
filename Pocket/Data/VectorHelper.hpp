//
//  VectorHelper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <algorithm>

namespace Pocket {
    struct VectorHelper {
    
        template <typename T, typename Compare>
        static std::vector<std::size_t> SortPermutation(const std::vector<T>& vec, Compare&& compare) {
            std::vector<std::size_t> p(vec.size());
            for(size_t i = 0; i<p.size(); ++i) {
                p[i]=i;
            }
            std::sort(p.begin(), p.end(),
                [&](std::size_t i, std::size_t j){ return compare(vec[i], vec[j]); });
            return p;
        }
        
        template <typename T>
        static std::vector<T> ApplyPermutation(const std::vector<T>& vec, const std::vector<std::size_t>& permutation) {
            std::vector<T> sorted(vec.size());
            std::transform(permutation.begin(), permutation.end(), sorted.begin(), [&](std::size_t i){ return vec[i]; });
            return sorted;
        }
    };
}
