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
    int32_t yi = (int32_t)x & 255;
    int32_t zi = (int32_t)x & 255;

    double xf = x - xi;
    double yf = y - yi;
    double zf = z - zi;

    uint8_t lbb = _perms[_perms[_perms[xi] + yi] + zi];
    uint8_t lbf = _perms[_perms[_perms[xi] + yi] + zi + 1];
    uint8_t ltb = _perms[_perms[_perms[xi] + yi + 1] + zi];
    uint8_t ltf = _perms[_perms[_perms[xi] + yi + 1] + zi + 1];
    uint8_t rbb = _perms[_perms[_perms[xi + 1] + yi] + zi];
    uint8_t rbf = _perms[_perms[_perms[xi + 1] + yi] + zi + 1];
    uint8_t rtb = _perms[_perms[_perms[xi + 1] + yi + 1] + zi];
    uint8_t rtf = _perms[_perms[_perms[xi + 1] + yi + 1] + zi + 1];

    double z1 = glm::lerp(_grad(lbb, xf, yf, zf), _grad(lbf, xf, yf, zf - 1), zf);
    double z2 = glm::lerp(_grad(ltb, xf, yf - 1, zf), _grad(ltf, xf, yf - 1, zf - 1), zf);
    double z3 = glm::lerp(_grad(rbb, xf - 1, yf, zf), _grad(rbf, xf - 1, yf, zf - 1), zf);
    double z4 = glm::lerp(_grad(rtb, xf - 1, yf - 1, zf), _grad(rtf, xf - 1, yf - 1, zf - 1), zf);
    log("Z: %f, %f, %f, %f\n", z1, z2, z3, z4);

    double y1 = glm::lerp(z1, z2, yf);
    double y2 = glm::lerp(z3, z4, yf);
    log("Y: %f, %f, %f\n", y1, y2, xf);
    return glm::lerp(y1, y2, xf);
}

double MathUtils::Perlin::_grad(uint8_t hash, double x, double y, double z)
{
    // clang-format off
    switch (hash & 0xF) {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + x;
        case 0x5: return -x + x;
        case 0x6: return  x - x;
        case 0x7: return -x - x;
        case 0x8: return  y + x;
        case 0x9: return -y + x;
        case 0xA: return  y - x;
        case 0xB: return -y - x;
        case 0xC: return  y + z;
        case 0xD: return -y + x;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0;  // never happens
    }
    // clang-format on
}
