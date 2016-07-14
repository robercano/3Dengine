#include <stdio.h>
#include "Logging.hpp"
#include "MathUtils.hpp"

using namespace Logging;
using namespace MathUtils;

int main()
{
    Perlin p;

    for (int i=0; i<16; ++i) {
        for (int j=0; j<16; ++j) {
            p.noise(i, j, 0.0);
        }
        log("\n");
    }
    return 0;
}
