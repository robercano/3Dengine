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

/**
 * Perlin noise implementation mostly based on:
 *
 * https://flafla2.github.io/2014/08/09/perlinnoise.html
 *
 * including octaves noise
 */
class Perlin
{
  public:
    /**
     * Returns the perlin noise at the specified coordinate
     *
     * A 2D slice can be obtained by fixing one of the coordinates
     *
     * @param x  X coordinate
     * @param y  Y coordinate
     * @param z  Z coordinate
     *
     * @return Value between 0.0 and 1.0
     */
    static double Noise(double x, double y, double z);

    /**
     * Returns the perlin noise at the specified location after sampling
     * different octaves for the same coordinate. The number of octaves to
     * be samples are indicated by 'octaves' parameter. The persistence of each
     * octave is determined by the 'persistence' parameter. If 'persistence' is less than
     * 1.0 higher frequency octaves will have less impact in the resulting noise. If
     * 'persistence' is greater than 1.0 then lower frequency octaves will have less
     * impact in the resulting noise
     *
     * @param x            X coordinate
     * @param y            Y coordinate
     * @param z            Z coordinate
     * @param octaves      Number of octaves to sample, up to 255
     * @param persistence  Persistance of each subsequent octave being sampled,
     *                     with a value greater than 0.0
     *
     * @return Value between 0.0 and 1.0
     */
    static double Octave(double x, double y, double z, uint8_t octaves, double persistence);

  private:
    /**
     * Singleton accessor
     */
    static Perlin *_getPerlin();

    /**
     * Constructor
     */
    Perlin();

    double _noise(double x, double y, double z);
    double _octave(double x, double y, double z, uint8_t octaves, double persistence);
    double _grad(uint8_t hash, double x, double y, double z);
    double _fade(double v);
    uint8_t *_perms;
};
}
