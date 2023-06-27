#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include <stdlib.h>
#include <windows.h>

class output_file {
  public:
    // Constructor.
    output_file();

    // Destructor.
    ~output_file();

    // Open.
    bool open(LPCWSTR filename);

    // Close.
    void close();

    // Add data.
    bool add(const char* data, size_t len);

    // Flush.
    bool flush();

  private:
    static const size_t kBufferSize = 8 * 1024;

    HANDLE _M_file;

    char* _M_buf;
    size_t _M_used;

    // Write.
    bool write(const char* data, size_t len);
};

inline output_file::output_file()
  : _M_file(INVALID_HANDLE_VALUE),
    _M_buf(NULL),
    _M_used(0)
{
}

inline output_file::~output_file()
{
  close();

  if (_M_buf) {
    free(_M_buf);
  }
}

inline bool output_file::flush()
{
  if (!write(_M_buf, _M_used)) {
    return false;
  }

  _M_used = 0;

  return true;
}

#endif // OUTPUT_FILE_H
