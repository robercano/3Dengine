/**
 * @file    ZCompression.hpp
 * @brief	Contains classes to perform compression/decompression using zlib
 */
#pragma once

#include <fstream>
#include "zlib.h"

/**
 * @class   ZCompression
 * @brief   Compresses and writes the given input to the given output stream
 */
class ZCompression
{
  public:
    /**
     * Size of the output buffer provided to zlib to
     * write the compressed output
     */
    static const uint32_t ChunkSize = 16384;

    /**
     * Constructor
     */
    ZCompression();

    /**
     * Destructor
     */
    ~ZCompression();

    /**
     * Initializes the compression
     */
    bool init();

    /**
     * Compresses and writes 'size' bytes from 'data' to 'file'
     *
     * @param file  Output file to write data to
     * @param data  Data to compress and write to 'file'
     * @param size  Number of bytes in 'data' to compress and write to 'file'
     * @param end   True if this is the last chunk of data to be compressed
     *
     * @return true if the data was compressed and written correctly, false
     *         otherwise
     */
    bool write(std::ofstream &file, const void *data, uint32_t size, bool end = false);

    /**
     * Finishes the compression. To use the compression
     * object again, init must be called
     */
    void finish();

  private:
    z_stream _strm;      /**< ZLib compression state */
    uint8_t *_outBuffer; /**< Output buffer provided to ZLib */
};

/**
 * @class   ZDecompression
 * @brief   Reads and decompresses data from the given input stream
 */
class ZDecompression
{
  public:
    /**
     * Size of the output buffer provided to zlib to
     * write the compressed output
     */
    static const uint32_t ChunkSize = 16384;

    /**
     * Constructor
     */
    ZDecompression();

    /**
     * Destructor
     */
    ~ZDecompression();

    /**
     * Initializes the decompression
     */
    bool init();

    /**
     * Reads bytes from 'file' decompressing them until a maximum of
     * 'size' bytes have been decompressed, then writes those bytes to 'data'
     *
     * @param file  Input file to read data from
     * @param data  Buffer where the decompressed data will be written to
     * @param size  Maximum number of decompressed bytes of data to obtain from 'file'
     *
     * @return true if the data was read and decompressed correctly, false
     *         otherwise
     */
    bool read(std::ifstream &file, void *data, const uint32_t &size);
    bool read(std::ifstream &file, void *data, uint32_t &size);

    /**
     * Finishes the decompression. To use the decompression
     * object again, init must be called
     */
    void finish();

  private:
    z_stream _strm;     /**< ZLib compression state */
    uint8_t *_inBuffer; /**< Input buffer provided to ZLib */
};
