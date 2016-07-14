/**
 * Misc funtions to aid different parts of the engine
 *
 * @author Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>

namespace MathUtils
{
    uint32_t clp2(uint32_t x);
    float restrictAngle(float angle);

    class Perlin {
        public:
            Perlin();
            double noise(double x, double y, double z);

        private:
            double _grad(uint8_t hash, double x, double y, double z);
            uint8_t *_perms;
    };
}
