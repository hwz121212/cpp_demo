#include <sys/stat.h>
#include "input_file.h"

bool input_file::open(const wchar_t* filename)
{
  struct _stat sbuf;
  if ((_wstat(filename, &sbuf) < 0) || ((sbuf.st_mode & _S_IFREG) == 0)) {
    return false;
  }

  return open(filename, sbuf.st_size);
}

bool input_file::open(const wchar_t* filename, _off_t filesize)
{
  // If the file is empty...
  if (filesize == 0) {
    return true;
  }

  // Open file for reading.
  if ((_M_file = CreateFile(filename,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL)) == INVALID_HANDLE_VALUE) {
    return false;
  }

  // Create file mapping object.
  if ((_M_mapping = CreateFileMapping(_M_file,
                                      NULL,
                                      PAGE_READONLY,
                                      0,
                                      0,
                                      NULL)) == NULL) {
    return false;
  }

  // Map view.
  if ((_M_data = MapViewOfFile(_M_mapping, FILE_MAP_READ, 0, 0, 0)) == NULL) {
    return false;
  }

  _M_size = filesize;
  _M_end = reinterpret_cast<uint8_t*>(_M_data) + _M_size;

  return true;
}

void input_file::close()
{
  if (_M_data) {
    UnmapViewOfFile(_M_data);
    _M_data = NULL;
  }

  if (_M_mapping) {
    CloseHandle(_M_mapping);
    _M_mapping = NULL;
  }

  if (_M_file != INVALID_HANDLE_VALUE) {
    CloseHandle(_M_file);
    _M_file = INVALID_HANDLE_VALUE;
  }
}
