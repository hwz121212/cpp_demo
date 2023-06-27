#include "output_file.h"

bool output_file::open(LPCWSTR filename)
{
  if (!_M_buf) {
    if ((_M_buf = reinterpret_cast<char*>(malloc(kBufferSize))) == NULL) {
      return false;
    }
  }

  // Open file for writing.
  if ((_M_file = CreateFile(filename,
                            GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_NEW,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL)) == INVALID_HANDLE_VALUE) {
    return false;
  }

  return true;
}

void output_file::close()
{
  if (_M_file != INVALID_HANDLE_VALUE) {
    CloseHandle(_M_file);
    _M_file = INVALID_HANDLE_VALUE;
  }

  _M_used = 0;
}

bool output_file::add(const char* data, size_t len)
{
  if (_M_used + len > kBufferSize) {
    if (!flush()) {
      return false;
    }

    if (len >= kBufferSize) {
      return write(data, len);
    }
  }

  memcpy(_M_buf + _M_used, data, len);
  _M_used += len;

  return true;
}

bool output_file::write(const char* data, size_t len)
{
  while (len > 0) {
    DWORD ret;
    if (!WriteFile(_M_file, data, static_cast<DWORD>(len), &ret, NULL)) {
      return false;
    }

    len -= ret;
  }

  return true;
}
