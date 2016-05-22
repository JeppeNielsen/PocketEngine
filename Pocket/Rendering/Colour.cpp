#include "Colour.hpp"

#include "StringHelper.hpp"

using namespace Pocket;




Colour Colour::Deserialize(const std::string &data) {
    std::vector<int> v;
    StringHelper::splitToInts(data, ',', v);
    return Colour(
                  (Component)(v.size()>0 ? v[0] : 0),
                  (Component)(v.size()>1 ? v[1] : 0),
                  (Component)(v.size()>2 ? v[2] : 0),
                  (Component)(v.size()>3 ? v[3] : 0)
                  );
}