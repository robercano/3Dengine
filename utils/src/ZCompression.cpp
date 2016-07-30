#include "ZCompression.hpp"
#include "Logging.hpp"

ZCompression::ZCompression()
{
    _outBuffer = new uint8_t[ChunkSize];
    if (_outBuffer == NULL) {
        Logging::log("ERROR allocating buffer in ZCompression constructor\n");
    }
}

ZCompression::~ZCompression() { delete[] _outBuffer; }
bool ZCompression::init()
{
    /* allocate deflate state */
    _strm.zalloc = Z_NULL;
    _strm.zfree = Z_NULL;
    _strm.opaque = Z_NULL;

    if (deflateInit(&_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return false;
    }

    return true;
}

bool ZCompression::write(std::ofstream &file, const void *data, uint32_t size, bool end)
{
    _strm.avail_in = size;
    _strm.next_in = (unsigned char *)data;

    do {
        _strm.avail_out = ChunkSize;
        _strm.next_out = _outBuffer;

        if (deflate(&_strm, end) == Z_STREAM_ERROR) {
            Logging::log("ERROR deflating data\n");
            finish();
            return false;
        }

        file.write((char *)_outBuffer, ChunkSize - _strm.avail_out);
    } while (_strm.avail_out == 0);

    if (_strm.avail_in != 0) {
        Logging::log("ERROR compressing data, not all input was used\n");
        finish();
        return false;
    }

    return true;
}

void ZCompression::finish() { (void)deflateEnd(&_strm); }
ZDecompression::ZDecompression()
{
    _inBuffer = new uint8_t[ChunkSize];
    if (_inBuffer == NULL) {
        Logging::log("ERROR allocating input buffer in ZDecompression constructor\n");
    }
}

ZDecompression::~ZDecompression() { delete[] _inBuffer; }
bool ZDecompression::init()
{
    /* allocate deflate state */
    _strm.zalloc = Z_NULL;
    _strm.zfree = Z_NULL;
    _strm.opaque = Z_NULL;
    _strm.avail_in = 0;
    _strm.next_in = Z_NULL;

    if (inflateInit(&_strm) != Z_OK) {
        return false;
    }

    return true;
}

bool ZDecompression::read(std::ifstream &file, void *data, const uint32_t &size)
{
    /* 'size' will be updated but change is lost as size is passed by value */
    uint32_t tmpSize = size;
    return read(file, data, tmpSize);
}

bool ZDecompression::read(std::ifstream &file, void *data, uint32_t &size)
{
    uint32_t initSize = size;

    _strm.avail_out = size;
    _strm.next_out = (unsigned char *)data;

    while (_strm.avail_out != 0) {
        /* Check if we need to read more inptu data */
        if (_strm.avail_in == 0) {
            file.read((char *)_inBuffer, ChunkSize);

            _strm.next_in = _inBuffer;
            _strm.avail_in = (int)file.gcount();

            if (_strm.avail_in == 0) {
                size = initSize - _strm.avail_out;
                return true;
            }
        }

        /* Inflate until we ran out of input data or
         * output data is completed */
        do {
            int ret = inflate(&_strm, Z_NO_FLUSH);

            if (ret == Z_STREAM_ERROR) {
                Logging::log("ERROR deflating data\n");
                finish();
                return false;
            }
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR; /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&_strm);
                    return ret;
            }
        } while (_strm.avail_out != 0 && _strm.avail_in != 0);
    }

    return true;
}

void ZDecompression::finish() { (void)inflateEnd(&_strm); }
