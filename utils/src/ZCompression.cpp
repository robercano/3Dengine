#include "ZCompression.hpp"
#include "Logging.hpp"

ZCompression::ZCompression()
{
    _outBuffer = new uint8_t[ChunkSize];
    if (_outBuffer == NULL) {
        Logging::log("ERROR allocating buffer in ZCompression constructor\n");
    }
}

ZCompression::~ZCompression()
{
    delete[] _outBuffer;
}

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

bool ZCompression::write(std::ofstream &file, const void *data, uint32_t size, bool end = false)
{
    _strm.avail_in = size;
    _strm.next_in = (unsigned char*)data;

    do {
        _strm.avail_out = ChunkSize;
        _strm.next_out = _outBuffer;

        if (deflate(&_strm, end) == Z_STREAM_ERROR) {
            Logging::log("ERROR deflating data\n");
            finish();
            return false;
        }

        file.write((char*)_outBuffer, ChunkSize - _strm.avail_out);
    } while (_strm.avail_out == 0);

    if (_strm.avail_in != 0) {
        Logging::log("ERROR compressing data, not all input was used\n");
        finish();
        return false;
    }

    return true;
}

void ZCompression::finish()
{
    (void)deflateEnd(&_strm);
}

ZDecompression::ZDecompression()
{
    _inBuffer = new uint8_t[ChunkSize];
    if (_inBuffer == NULL) {
        Logging::log("ERROR allocating input buffer in ZDecompression constructor\n");
    }
    _outBuffer = new uint8_t[ChunkSize];
    if (_outBuffer == NULL) {
        Logging::log("ERROR allocating output buffer in ZDecompression constructor\n");
    }
    _inBufferBytes = 0;
}

ZDecompression::~ZDecompression()
{
    delete[] _inBuffer;
}

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

bool ZDecompression::read(std::ofstream &file, const void *data, uint32_t size, bool end = false)
{
    uint32_t dataPos = 0;

    do {
        uint32_t start = file.tellg();

        file.read(_inBuffer+_inBufferBytes, ChunkSize - _inBufferBytes);

        _inBufferBytes += file.tellg() - start;
        if (_inBufferBytes == 0) {
            return true;
        }

        _strm.next_in = _inBuffer;
        _strm.avail_in = _inBufferBytes;

        do {
            _strm.avail_out = ChunkSize;
            _strm.next_out = _outBuffer;

            int ret = inflate(&_strm, Z_NO_FLUSH);

            if (ret == Z_STREAM_ERROR) {
                Logging::log("ERROR deflating data\n");
                finish();
                return false;
            }
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
            }

            uint32_t writeBytes = ChunkSize - strm.avail_out;

            if (size < writeBytes) {
                memcpy(data, _outBuffer, size);
                dataPos += size;
            } else {
                memcpy(data, _outBuffer, writeBytes);
                dataPos += writeBytes;
            }
        } while (_strm.avail_out == 0);
    } while (dataPos < size);

    return true;
}

void ZDecompression::finish()
{
    (void)inflateEnd(&_strm);
}

