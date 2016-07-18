/**
 * Misc funtions to aid different parts of the engine
 *
 * @author Roberto Cano
 */
#include "MathUtils.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include "Logging.hpp"

using namespace Logging;

uint32_t MathUtils::clp2(uint32_t x)
{
    x--;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x + 1;
}

float MathUtils::restrictAngle(float angle)
{
    if (angle >= 360.0)
        angle -= 360.0;
    else if (angle < 0.0)
        angle += 360.0;
    return angle;
}

/**
 * Perlin noise implementation
 */
MathUtils::Perlin::Perlin()
{
    uint8_t perlinPerms[] = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240,
        21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
        237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111,
        229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,
        209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217,
        226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,
        223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
        108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241,
        81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
        150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180};

    _perms = new uint8_t[2 * sizeof perlinPerms / sizeof *perlinPerms];

    for (int i = 0; i < sizeof perlinPerms / sizeof *perlinPerms; ++i) {
        _perms[i] = _perms[i + 256] = perlinPerms[i];
    }
}

double MathUtils::Perlin::noise(double x, double y, double z)
{
    int32_t xi = (int32_t)x & 255;
    int32_t yi = (int32_t)y & 255;
    int32_t zi = (int32_t)z & 255;

    x -= xi;
    y -= yi;
    z -= zi;

    double u = _fade(x);
    double v = _fade(y);
    double w = _fade(z);

    uint8_t px0 = _perms[xi] + yi;
    uint8_t px1 = _perms[xi + 1] + yi;
    uint8_t px0y0 = _perms[px0] + zi;
    uint8_t px0y1 = _perms[px0 + 1] + zi;
    uint8_t px1y0 = _perms[px1] + zi;
    uint8_t px1y1 = _perms[px1 + 1] + zi;

    return (glm::lerp(
                glm::lerp(glm::lerp(_grad(_perms[px0y0], x, y, z), _grad(_perms[px0y0 + 1], x, y, z - 1.0), w),
                          glm::lerp(_grad(_perms[px0y1], x, y - 1.0, z), _grad(_perms[px0y1 + 1], x, y - 1.0, z - 1.0), w), v),
                glm::lerp(glm::lerp(_grad(_perms[px1y0], x - 1.0, y, z), _grad(_perms[px1y0 + 1], x - 1.0, y, z - 1.0), w),
                          glm::lerp(_grad(_perms[px1y1], x - 1.0, y - 1.0, z), _grad(_perms[px1y1 + 1], x - 1.0, y - 1.0, z - 1.0), w), v),
                u) +
            1.0) /
           2.0;
}

double MathUtils::Perlin::octave(double x, double y, double z, uint8_t octaves, double persistence)
{
    double frequency = 1.0;
    double amplitude = 1.0;
    double maxValue = 0.0;

    double v = 0.0;

    for (int i = 0; i < octaves; ++i) {
        v += amplitude * noise(x * frequency, y * frequency, z * frequency);

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2.0;
    }

    return v / maxValue;
}

double MathUtils::Perlin::_grad(uint8_t hash, double x, double y, double z)
{
    // clang-format off
    switch (hash & 0xF) {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0;  // never happens
    }
    // clang-format on
}

double MathUtils::Perlin::_fade(double v) { return v * v * v * (v * (v * 6 - 15) + 10); }
