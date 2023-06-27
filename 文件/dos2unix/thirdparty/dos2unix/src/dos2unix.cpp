#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>
#include "dos2unix.h"
#include "input_file.h"
#include "output_file.h"

dos2unix::~dos2unix()
{
  if (_M_file_extensions) {
    for (size_t i = 0; i < _M_used; i++) {
      free(_M_file_extensions[i]);
    }

    free(_M_file_extensions);
  }
}

bool dos2unix::add_file_extension(const wchar_t* ext)
{
  size_t len;
  if ((!ext) || ((len = wcslen(ext)) == 0) || (len > kFileExtensionMaxLength)) {
    return false;
  }

  wchar_t* extension;
  if ((extension = _wcsdup(ext)) == NULL) {
    return false;
  }

  if (_M_used == _M_size) {
    size_t size = (_M_size == 0) ? 8 : (_M_size * 2);

    wchar_t** file_extensions;
    if ((file_extensions = reinterpret_cast<wchar_t**>(
                             realloc(_M_file_extensions,
                                     size * sizeof(wchar_t*)
                                    )
                           )) == NULL) {
      free(extension);
      return false;
    }

    _M_file_extensions = file_extensions;
    _M_size = size;
  }

  _M_file_extensions[_M_used++] = extension;

  return true;
}

bool dos2unix::install(const wchar_t* filename)
{
  if (!register_file_extensions(filename)) {
    return false;
  }

  // If no extensions have been defined...
  if (_M_used == 0) {
    // Nothing to do.
    MessageBox(NULL, L"No extensions found.", L"Result", MB_OK);
    return true;
  }

  if ((!register_folder()) || (!register_program())) {
    return false;
  }

  return generate_log_file(L"installation.log");
}

bool dos2unix::uninstall()
{
  if (!load_file_extensions_from_registry()) {
    return false;
  }

  // If there are file extensions...
  if (_M_used > 0) {
    if (!unregister_file_extensions()) {
      return false;
    }
  }

  if ((!unregister_folder()) || (!unregister_program())) {
    return false;
  }

  return generate_log_file(L"uninstallation.log");
}

bool dos2unix::load_file_extensions_from_registry()
{
  wchar_t key[512];
  _snwprintf_s(key, ARRAYSIZE(key), _TRUNCATE, L"SOFTWARE\\%s", kProgram);

  HKEY hKey;
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                   key,
                   0,
                   KEY_QUERY_VALUE,
                   &hKey) != ERROR_SUCCESS) {
    wchar_t msg[512];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"Error opening registry key HKEY_LOCAL_MACHINE\\%s",
                 key);

    MessageBox(NULL, msg, L"Error", MB_OK);
    return false;
  }

  // Read length.
  DWORD len;
  if (RegQueryValueEx(hKey,
                      L"Extensions",
                      NULL,
                      NULL,
                      NULL,
                      &len) != ERROR_SUCCESS) {
    wchar_t msg[512];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"Value HKEY_LOCAL_MACHINE\\%s\\Extensions not found.",
                 key);

    MessageBox(NULL, msg, L"Error", MB_OK);

    RegCloseKey(hKey);
    return false;
  }

  wchar_t* ext;
  if ((ext = reinterpret_cast<wchar_t*>(malloc(len))) == NULL) {
    MessageBox(NULL, L"Cannot allocate memory.", L"Error", MB_OK);

    RegCloseKey(hKey);
    return false;
  }

  // Read value.
  if (RegQueryValueEx(hKey,
                      L"Extensions",
                      NULL,
                      NULL,
                      reinterpret_cast<BYTE*>(ext),
                      &len) != ERROR_SUCCESS) {
    wchar_t msg[512];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"Error getting value of key "
                  "HKEY_LOCAL_MACHINE\\%s\\Extensions",
                 key);

    MessageBox(NULL, msg, L"Error", MB_OK);

    free(ext);
    RegCloseKey(hKey);

    return false;
  }

  RegCloseKey(hKey);

  wchar_t* extension = ext;

  do {
    wchar_t* end;
    if ((end = wcschr(extension, L';')) != NULL) {
      *end = L'\0';
    }

    if (!add_file_extension(extension)) {
      MessageBox(NULL, L"Cannot allocate memory.", L"Error", MB_OK);

      free(ext);
      return false;
    }

    if (!end) {
      break;
    }

    extension = end + 1;
  } while (true);

  free(ext);

  return true;
}

bool dos2unix::check_file(const wchar_t* filename, _off_t filesize)
{
  // Open file for reading.
  input_file file;
  if (!file.open(filename, filesize)) {
    _M_log.format(L"%s: error opening input file.\r\n", filename);
    return false;
  }

  type t;

  // If the file is not empty...
  if (file.size() > 0) {
    t = type::kBinary;
    bool lf = false;

    const uint8_t* data = file.data();

    while (data < file.end()) {
      if (*data < ' ') {
        switch (*data) {
          case '\t':
            break;
          case '\n':
            switch (t) {
              case type::kBinary:
                t = type::kUnix;
                break;
              case type::kDos:
                if (lf) {
                  lf = false;
                } else {
                  t = type::kMixed;
                }

                break;
            }

            break;
          case '\r':
            switch (t) {
              case type::kBinary:
                t = type::kDos;
                lf = true;
                break;
              case type::kUnix:
                t = type::kMixed;
                break;
              case type::kDos:
                if (!lf) {
                  lf = true;
                } else {
                  t = type::kMixed;
                }

                break;
            }

            break;
          default:
            _M_log.format(L"%s: %s\r\n",
                          filename,
                          type_to_string(type::kBinary));

            return true;
        }
      } else {
        // Line feed as end-of-line?
        if (lf) {
          t = type::kMixed;
        }
      }

      data++;
    }
  } else {
    t = type::kEmpty;
  }

  _M_log.format(L"%s: %s\r\n", filename, type_to_string(t));

  return true;
}

bool dos2unix::process(const wchar_t* name, operation op, bool backup)
{
  struct _stat sbuf;
  if (_wstat(name, &sbuf) < 0) {
    _M_log.format(L"%s: doesn't exist.\r\n", name);
    return false;
  }

  // File?
  if ((sbuf.st_mode & _S_IFREG) != 0) {
    switch (op) {
      case operation::kCheck:
        return check_file(name, sbuf.st_size);
      case operation::kConvertToUnix:
        return convert_file(name, sbuf.st_size, type::kUnix, backup);
      case operation::kConvertToDos:
        return convert_file(name, sbuf.st_size, type::kDos, backup);
      default:
        _M_log.format(L"Invalid operation %d.\r\n", static_cast<int>(op));
        return false;
    }
  } else if ((sbuf.st_mode & _S_IFDIR) != 0) {
    // Directory.
    return walk_directory(name, op, backup);
  } else {
    _M_log.format(L"%s: neither a file nor a directory.\r\n", name);
    return false;
  }
}

bool dos2unix::walk_directory(const wchar_t* dirname, operation op, bool backup)
{
  size_t len;
  if (((len = wcslen(dirname)) == 0) || (len + 3 >= MAX_PATH)) {
    return false;
  }

  // Remove trailing '\' or '/' (if any).
  wchar_t dir[MAX_PATH];
  while ((len > 0) &&
         ((dirname[len - 1] == L'\\') || (dirname[len - 1] == L'/'))) {
    len--;
  }

  wmemcpy(dir, dirname, len);
  dir[len++] = L'\\';
  dir[len++] = L'*';
  dir[len] = L'\0';

  HANDLE hFind;
  WIN32_FIND_DATA find_data;
  if ((hFind = FindFirstFile(dir, &find_data)) == INVALID_HANDLE_VALUE) {
    return false;
  }

  do {
    // Current directory or parent directory?
    if ((wcscmp(find_data.cFileName, L".") == 0) ||
        (wcscmp(find_data.cFileName, L"..") == 0)) {
      continue;
    }

    // Compose file name.
    wchar_t filename[MAX_PATH];
    _snwprintf_s(filename,
                ARRAYSIZE(filename),
                _TRUNCATE,
                L"%s\\%s",
                dirname,
                find_data.cFileName);

    // File?
    if ((find_data.dwFileAttributes == FILE_ATTRIBUTE_NORMAL) ||
        ((find_data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0)) {
      // If the file should be processed...
      if (target(find_data.cFileName)) {
        // If the file is not too big...
        if (find_data.nFileSizeHigh == 0) {
          bool ret;

          switch (op) {
            case operation::kCheck:
              ret = check_file(filename, find_data.nFileSizeLow);
              break;
            case operation::kConvertToUnix:
              ret = convert_file(filename,
                                 find_data.nFileSizeLow,
                                 type::kUnix,
                                 backup);

              break;
            case operation::kConvertToDos:
              ret = convert_file(filename,
                                 find_data.nFileSizeLow,
                                 type::kDos,
                                 backup);

              break;
          }

          if (!ret) {
            FindClose(hFind);
            return false;
          }
        }
      }
    } else if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
      if (!walk_directory(filename, op, backup)) {
        FindClose(hFind);
        return false;
      }
    }
  } while (FindNextFile(hFind, &find_data));

  FindClose(hFind);

  return true;
}

bool dos2unix::convert_file(const wchar_t* filename,
                            _off_t filesize,
                            type final_type,
                            bool backup)
{
  const char* eol;
  size_t eollen;

  switch (final_type) {
    case type::kDos:
      eol = "\r\n";
      eollen = 2;

      break;
    case type::kUnix:
      eol = "\n";
      eollen = 1;

      break;
    default:
      _M_log.format(L"%s: unsupported output type %s.\r\n",
                    filename,
                    type_to_string(final_type));

      return false;
  }

  // Open file for reading.
  input_file in;
  if (!in.open(filename, filesize)) {
    _M_log.format(L"%s: error opening input file.\r\n", filename);
    return false;
  }

  // If the file is empty...
  if (in.size() == 0) {
    _M_log.format(L"%s: %s\r\n", filename, type_to_string(type::kEmpty));
    return true;
  }

  // Generate filename for the output file.
  wchar_t tmpout[MAX_PATH];
  unsigned count = 0;
  do {
    if (count == 0) {
      _snwprintf_s(tmpout, ARRAYSIZE(tmpout), L"%s.new", filename);
    } else {
      _snwprintf_s(tmpout, ARRAYSIZE(tmpout), L"%s.new.%u", filename, count);
    }

    struct _stat sbuf;
    if (_wstat(tmpout, &sbuf) < 0) {
      break;
    }

    if (++count > kMaxFilenameSearches) {
      _M_log.format(L"%s: error generating filename for the output file.\r\n",
                    filename);

      return false;
    }
  } while (true);

  // Open output file for writing.
  output_file out;
  if (!out.open(tmpout)) {
    _M_log.format(L"%s: error opening output file '%s'.\r\n", filename, tmpout);
    return false;
  }

  type t = type::kBinary;
  bool lf = false;

  const uint8_t* data = in.data();
  const uint8_t* begin = data;

  while (data < in.end()) {
    if (*data < ' ') {
      switch (*data) {
        case '\t':
          break;
        case '\n':
          if (begin != data) {
            if (!out.add(reinterpret_cast<const char*>(begin), data - begin)) {
              out.close();
              _wunlink(tmpout);

              _M_log.format(L"%s: error writing to output file '%s'\r\n",
                            filename,
                            tmpout);

              return false;
            }
          }

          if (!out.add(eol, eollen)) {
            out.close();
            _wunlink(tmpout);

            _M_log.format(L"%s: error writing to output file '%s'\r\n",
                          filename,
                          tmpout);

            return false;
          }

          begin = data + 1;

          switch (t) {
            case type::kBinary:
              t = type::kUnix;
              break;
            case type::kDos:
              if (lf) {
                lf = false;
              } else {
                t = type::kMixed;
              }

              break;
          }

          break;
        case '\r':
          if (begin != data) {
            if (!out.add(reinterpret_cast<const char*>(begin), data - begin)) {
              out.close();
              _wunlink(tmpout);

              _M_log.format(L"%s: error writing to output file '%s'\r\n",
                            filename,
                            tmpout);

              return false;
            }
          }

          begin = data + 1;

          switch (t) {
            case type::kBinary:
              t = type::kDos;
              lf = true;
              break;
            case type::kUnix:
              t = type::kMixed;
              break;
            case type::kDos:
              if (!lf) {
                lf = true;
              } else {
                t = type::kMixed;
              }

              break;
          }

          break;
        default:
          out.close();
          _wunlink(tmpout);

          _M_log.format(L"%s: %s\r\n", filename, type_to_string(type::kBinary));

          return true;
      }
    } else {
      // Line feed as end-of-line?
      if (lf) {
        t = type::kMixed;
      }
    }

    data++;
  }

  if (t == final_type) {
    out.close();
    _wunlink(tmpout);

    _M_log.format(L"%s: is already a %s file.\r\n",
                  filename,
                  type_to_string(final_type));

    return true;
  }

  if (begin != data) {
    if (!out.add(reinterpret_cast<const char*>(begin), data - begin)) {
      out.close();
      _wunlink(tmpout);

      _M_log.format(L"%s: error writing to output file '%s'\r\n",
                    filename,
                    tmpout);

      return false;
    }
  }

  if (!out.flush()) {
    out.close();
    _wunlink(tmpout);

    _M_log.format(L"%s: error writing to output file '%s'\r\n",
                  filename,
                  tmpout);

    return false;
  }

  in.close();
  out.close();

  if (backup) {
    // Generate filename for the backup file.
    wchar_t bak[MAX_PATH];
    count = 0;
    do {
      if (count == 0) {
        _snwprintf_s(bak, ARRAYSIZE(bak), L"%s.bak", filename);
      } else {
        _snwprintf_s(bak, ARRAYSIZE(bak), L"%s.bak.%u", filename, count);
      }

      struct _stat sbuf;
      if (_wstat(bak, &sbuf) < 0) {
        break;
      }

      if (++count > kMaxFilenameSearches) {
        _M_log.format(L"%s: error generating filename for the backup file.\r\n",
                      filename);

        _wunlink(tmpout);

        return false;
      }
    } while (true);

    if (!MoveFile(filename, bak)) {
      _M_log.format(L"%s: error renaming '%s' -> '%s'.\r\n",
                    filename,
                    filename,
                    bak);

      _wunlink(tmpout);

      return false;
    }
  }

  if (!MoveFileEx(tmpout, filename, MOVEFILE_REPLACE_EXISTING)) {
    _M_log.format(L"%s: error renaming '%s' -> '%s'.\r\n",
                  filename,
                  tmpout,
                  filename);

    _wunlink(tmpout);

    return false;
  }

  _M_log.format(L"%s: converted to %s\r\n",
                filename,
                type_to_string(final_type));

  return true;
}

bool dos2unix::target(const wchar_t* filename) const
{
  // If no file extensions have been defined...
  if (_M_used == 0) {
    return true;
  }

  // Get pointer to the file extension.
  const wchar_t* ext;
  if ((ext = wcsrchr(filename, L'.')) == NULL) {
    return false;
  }

  ++ext;

  // Search file extension.
  for (size_t i = 0; i < _M_used; i++) {
    if (_wcsicmp(ext, _M_file_extensions[i]) == 0) {
      return true;
    }
  }

  return false;
}

bool dos2unix::register_file_extensions(const wchar_t* filename)
{
  // Load file extensions from file.
  if (!load_file_extensions_from_file(filename)) {
    wchar_t msg[MAX_PATH + 128];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"Error opening file '%s'.",
                 filename);

    MessageBox(NULL, msg, L"Error", MB_OK);
    return false;
  }

  // Register extensions.
  for (size_t i = 0; i < _M_used; i++) {
    wchar_t ext[kFileExtensionMaxLength + 2];
    ext[0] = L'.';
    wcscpy_s(ext + 1, ARRAYSIZE(ext) - 1, _M_file_extensions[i]);

    // Create key HKEY_CLASSES_ROOT\<extension> (if not present).
    HKEY hKey;
    DWORD disposition;
    if (RegCreateKeyEx(HKEY_CLASSES_ROOT, // Key.
                       ext,               // Subkey.
                       0,                 // Reserved.
                       NULL,              // Class.
                       0,                 // Options.
                       KEY_ALL_ACCESS,    // Access rights.
                       NULL,              // Security attributes.
                       &hKey,             // Result.
                       &disposition) != ERROR_SUCCESS) {
      wchar_t msg[512];
      _snwprintf_s(msg,
                   ARRAYSIZE(msg),
                   _TRUNCATE,
                   L"Error creating/opening registry key HKEY_CLASSES_ROOT\\%s",
                   ext);

      MessageBox(NULL, msg, L"Error", MB_OK);
      return false;
    }

    // If the key already exists...
    wchar_t type[512];
    if (disposition == REG_OPENED_EXISTING_KEY) {
      _M_log.format(L"Opened key HKEY_CLASSES_ROOT\\%s\r\n", ext);

      // Read default value.
      DWORD len = sizeof(type);
      if (RegQueryValueEx(hKey,
                          NULL,
                          NULL,
                          NULL,
                          reinterpret_cast<BYTE*>(type),
                          &len) != ERROR_SUCCESS) {
        wchar_t msg[512];
        _snwprintf_s(msg,
                     ARRAYSIZE(msg),
                     _TRUNCATE,
                     L"Error getting default value of key "
                      "HKEY_CLASSES_ROOT\\%s",
                     ext);

        MessageBox(NULL, msg, L"Error", MB_OK);

        RegCloseKey(hKey);
        return false;
      }
    } else {
      _M_log.format(L"Created key HKEY_CLASSES_ROOT\\%s\r\n", ext);

      // Generate file type.
      int len = _snwprintf_s(type,
                             ARRAYSIZE(type),
                             _TRUNCATE,
                             L"%s%s",
                             _M_file_extensions[i],
                             kLineEndingsFileType);

      static const wchar_t* subkeys[] = {
        NULL,
        L"Content Type",
        L"Perceived Type"
      };

      static const wchar_t* values[] = {
        type,
        L"text/plain",
        L"text"
      };

      if (!set_values(hKey, ext, subkeys, values, 3)) {
        RegCloseKey(hKey);
        return false;
      }
    }

    RegCloseKey(hKey);

    // Compose subkey.
    wchar_t subkey[512];
    _snwprintf_s(subkey,
                 ARRAYSIZE(subkey),
                 _TRUNCATE,
                 L"%s\\shell\\%s",
                 type,
                 kLineEndingsShell);

    if (!register_line_endings(subkey)) {
      return false;
    }
  }

  return true;
}

bool dos2unix::unregister_file_extensions()
{
  size_t l = wcslen(kLineEndingsFileType);

  // For each file extension...
  for (size_t i = 0; i < _M_used; i++) {
    wchar_t ext[kFileExtensionMaxLength + 2];
    ext[0] = L'.';
    wcscpy_s(ext + 1, ARRAYSIZE(ext) - 1, _M_file_extensions[i]);

    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,
                     ext,
                     0,
                     KEY_QUERY_VALUE,
                     &hKey) != ERROR_SUCCESS) {
      continue;
    }

    // Read default value.
    wchar_t type[512];
    DWORD len = sizeof(type);
    if (RegQueryValueEx(hKey,
                        NULL,
                        NULL,
                        NULL,
                        reinterpret_cast<BYTE*>(type),
                        &len) != ERROR_SUCCESS) {
      wchar_t msg[512];
      _snwprintf_s(msg,
                   ARRAYSIZE(msg),
                   _TRUNCATE,
                   L"Error getting default value of key "
                    "HKEY_CLASSES_ROOT\\%s",
                    ext);

      MessageBox(NULL, msg, L"Error", MB_OK);

      RegCloseKey(hKey);
      return false;
    }

    RegCloseKey(hKey);

    if (len < (2 * sizeof(wchar_t))) {
      continue;
    }

    len = (len / sizeof(wchar_t)) - 1;

    if ((len > l) && (wcscmp(type + len - l, kLineEndingsFileType) == 0)) {
      if (RegDeleteTree(HKEY_CLASSES_ROOT, type) == ERROR_SUCCESS) {
        _M_log.format(L"Deleted tree HKEY_CLASSES_ROOT\\%s\r\n", type);
      } else {
        _M_log.format(L"Error deleting tree HKEY_CLASSES_ROOT\\%s\r\n", type);
      }

      if (RegDeleteTree(HKEY_CLASSES_ROOT, ext) == ERROR_SUCCESS) {
        _M_log.format(L"Deleted tree HKEY_CLASSES_ROOT\\%s\r\n", ext);
      } else {
        _M_log.format(L"Error deleting tree HKEY_CLASSES_ROOT\\%s\r\n", ext);
      }
    } else {
      wchar_t subkey[512];
      _snwprintf_s(subkey,
                   ARRAYSIZE(subkey),
                   _TRUNCATE,
                   L"%s\\shell\\%s",
                   type,
                   kLineEndingsShell);

      if (RegDeleteTree(HKEY_CLASSES_ROOT, subkey) == ERROR_SUCCESS) {
        _M_log.format(L"Deleted tree HKEY_CLASSES_ROOT\\%s\r\n", subkey);
      } else {
        _M_log.format(L"Error deleting tree HKEY_CLASSES_ROOT\\%s\r\n", subkey);
      }
    }
  }

  return true;
}

bool dos2unix::register_folder()
{
  wchar_t subkey[512];
  _snwprintf_s(subkey,
               ARRAYSIZE(subkey),
               _TRUNCATE,
               L"Folder\\shell\\%s",
               kLineEndingsShell);

  return register_line_endings(subkey);
}

bool dos2unix::unregister_folder()
{
  wchar_t subkey[512];
  _snwprintf_s(subkey,
               ARRAYSIZE(subkey),
               _TRUNCATE,
               L"Folder\\shell\\%s",
               kLineEndingsShell);

  if (RegDeleteTree(HKEY_CLASSES_ROOT, subkey) == ERROR_SUCCESS) {
    _M_log.format(L"Deleted tree HKEY_CLASSES_ROOT\\%s\r\n", subkey);
  } else {
    _M_log.format(L"Error deleting tree HKEY_CLASSES_ROOT\\%s\r\n", subkey);
  }

  return true;
}

bool dos2unix::register_program()
{
  wchar_t subkey[512];
  _snwprintf_s(subkey, ARRAYSIZE(subkey), _TRUNCATE, L"SOFTWARE\\%s", kProgram);

  HKEY hSubkey;
  DWORD disposition;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, // Key.
                     subkey,             // Subkey.
                     0,                  // Reserved.
                     NULL,               // Class.
                     0,                  // Options.
                     KEY_ALL_ACCESS,     // Access rights.
                     NULL,               // Security attributes.
                     &hSubkey,           // Result.
                     &disposition) != ERROR_SUCCESS) {
    wchar_t msg[512];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"Error creating/opening registry key HKEY_LOCAL_MACHINE\\%s",
                 subkey);

    MessageBox(NULL, msg, L"Error", MB_OK);
    return false;
  }

  if (disposition == REG_OPENED_EXISTING_KEY) {
    _M_log.format(L"Opened key HKEY_LOCAL_MACHINE\\%s\r\n", subkey);
  } else {
    _M_log.format(L"Created key HKEY_LOCAL_MACHINE\\%s\r\n", subkey);
  }

  // Calculate space needed for the file extensions.
  size_t len = 0;
  for (size_t i = 0; i < _M_used; i++) {
    len += wcslen(_M_file_extensions[i]);
  }

  // Add space for the separators.
  len += (_M_used - 1);

  // Add space for the terminating '\0'.
  len++;

  // Compose list of extensions with the format:
  // <ext>[;<ext>]*
  wchar_t* ext;
  if ((ext = reinterpret_cast<wchar_t*>(
               malloc(len * sizeof(wchar_t))
             )) == NULL) {
    MessageBox(NULL, L"Cannot allocate memory.", L"Error", MB_OK);

    RegCloseKey(hSubkey);
    return false;
  }

  size_t l = 0;
  for (size_t i = 0; i < _M_used; i++) {
    if (i > 0) {
      ext[l++] = L';';
    }

    wcscpy_s(ext + l, len - l, _M_file_extensions[i]);
    l += wcslen(_M_file_extensions[i]);
  }

  // Set subkey and value.
  if (RegSetKeyValue(hSubkey,
                     NULL,
                     L"Extensions",
                     REG_SZ,
                     ext,
                     len * sizeof(wchar_t))
      != ERROR_SUCCESS) {
    wchar_t msg[512];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"Error setting value of key "
                  "HKEY_LOCAL_MACHINE\\%s\\Extensions",
                 subkey);

    MessageBox(NULL, msg, L"Error", MB_OK);

    free(ext);
    RegCloseKey(hSubkey);

    return false;
  }

  _M_log.format(L"HKEY_LOCAL_MACHINE\\%s\\Extensions = %s\r\n", subkey, ext);

  free(ext);
  RegCloseKey(hSubkey);

  return true;
}

bool dos2unix::unregister_program()
{
  wchar_t subkey[512];
  _snwprintf_s(subkey, ARRAYSIZE(subkey), _TRUNCATE, L"SOFTWARE\\%s", kProgram);

  if (RegDeleteTree(HKEY_LOCAL_MACHINE, subkey) == ERROR_SUCCESS) {
    _M_log.format(L"Deleted tree HKEY_LOCAL_MACHINE\\%s\r\n", subkey);
  } else {
    _M_log.format(L"Error deleting tree HKEY_LOCAL_MACHINE\\%s\r\n", subkey);
  }

  return true;
}

bool dos2unix::register_line_endings(const wchar_t* subkey)
{
  HKEY hSubkey;
  DWORD disposition;
  if (RegCreateKeyEx(HKEY_CLASSES_ROOT, // Key.
                     subkey,            // Subkey.
                     0,                 // Reserved.
                     NULL,              // Class.
                     0,                 // Options.
                     KEY_ALL_ACCESS,    // Access rights.
                     NULL,              // Security attributes.
                     &hSubkey,          // Result.
                     &disposition) != ERROR_SUCCESS) {
    wchar_t msg[512];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"Error creating/opening registry key HKEY_CLASSES_ROOT\\%s",
                 subkey);

    MessageBox(NULL, msg, L"Error", MB_OK);
    return false;
  }

  if (disposition == REG_OPENED_EXISTING_KEY) {
    _M_log.format(L"Opened key HKEY_CLASSES_ROOT\\%s\r\n", subkey);
  } else {
    _M_log.format(L"Created key HKEY_CLASSES_ROOT\\%s\r\n", subkey);
  }

  {
    static const wchar_t* subkeys[] = {
      L"MUIVerb",
      L"SubCommands"
    };

    static const wchar_t* values[] = {
      L"Line endings",
      L""
    };

    if (!set_values(hSubkey, subkey, subkeys, values, 2)) {
      RegCloseKey(hSubkey);
      return false;
    }
  }

  static const wchar_t* const verbs[] = {
    L"check",
    L"dos2unix",
    L"dos2unixbackup",
    L"unix2dos",
    L"unix2dosbackup"
  };

  static const wchar_t* const menu[] = {
    L"Check",
    L"To UNIX",
    L"To UNIX (backup)",
    L"To DOS",
    L"To DOS (backup)"
  };

  // For each verb...
  for (size_t i = 0; i < ARRAYSIZE(verbs); i++) {
    // Compose subkey.
    wchar_t str[256];
    _snwprintf_s(str, ARRAYSIZE(str), _TRUNCATE, L"shell\\%s", verbs[i]);

    HKEY hVerb;
    if (RegCreateKeyEx(hSubkey,           // Key.
                       str,               // Subkey.
                       0,                 // Reserved.
                       NULL,              // Class.
                       0,                 // Options.
                       KEY_ALL_ACCESS,    // Access rights.
                       NULL,              // Security attributes.
                       &hVerb,            // Result.
                       &disposition) != ERROR_SUCCESS) {
      wchar_t msg[512];
      _snwprintf_s(msg,
                   ARRAYSIZE(msg),
                   _TRUNCATE,
                   L"Error creating/opening registry key "
                    "HKEY_CLASSES_ROOT\\%s\\%s",
                   subkey,
                   str);

      MessageBox(NULL, msg, L"Error", MB_OK);

      RegCloseKey(hSubkey);
      return false;
    }

    if (disposition == REG_OPENED_EXISTING_KEY) {
      _M_log.format(L"Opened key HKEY_CLASSES_ROOT\\%s\\%s\r\n", subkey, str);
    } else {
      _M_log.format(L"Created key HKEY_CLASSES_ROOT\\%s\\%s\r\n", subkey, str);
    }

    // Set default value.
    if (RegSetKeyValue(hVerb,
                       NULL,
                       NULL,
                       REG_SZ,
                       menu[i],
                       (wcslen(menu[i]) + 1) * sizeof(wchar_t))
        != ERROR_SUCCESS) {
      wchar_t msg[512];
      _snwprintf_s(msg,
                   ARRAYSIZE(msg),
                   _TRUNCATE,
                   L"Error setting default value of key "
                    "HKEY_CLASSES_ROOT\\%s\\%s",
                   subkey,
                   str);

      MessageBox(NULL, msg, L"Error", MB_OK);

      RegCloseKey(hVerb);
      RegCloseKey(hSubkey);

      return false;
    }

    _M_log.format(L"HKEY_CLASSES_ROOT\\%s\\%s\\(Default) = %s\r\n",
                  subkey,
                  str,
                  menu[i]);

    HKEY hCommand;
    if (RegCreateKeyEx(hVerb,             // Key.
                       L"command",        // Subkey.
                       0,                 // Reserved.
                       NULL,              // Class.
                       0,                 // Options.
                       KEY_ALL_ACCESS,    // Access rights.
                       NULL,              // Security attributes.
                       &hCommand,         // Result.
                       &disposition) != ERROR_SUCCESS) {
      wchar_t msg[512];
      _snwprintf_s(msg,
                   ARRAYSIZE(msg),
                   _TRUNCATE,
                   L"Error creating/opening registry key "
                    "HKEY_CLASSES_ROOT\\%s\\%s\\command",
                   subkey,
                   str);

      MessageBox(NULL, msg, L"Error", MB_OK);

      RegCloseKey(hVerb);
      RegCloseKey(hSubkey);

      return false;
    }

    if (disposition == REG_OPENED_EXISTING_KEY) {
      _M_log.format(L"Opened key HKEY_CLASSES_ROOT\\%s\\%s\\command\r\n",
                    subkey,
                    str);
    } else {
      _M_log.format(L"Created key HKEY_CLASSES_ROOT\\%s\\%s\\command\r\n",
                    subkey,
                    str);
    }

    static const wchar_t* kBinary = L"C:\\Users\\IEUser\\Desktop\\dos2unix\\Debug\\dos2unix.exe";

    const wchar_t* option;

    size_t verblen;
    if ((verblen = wcslen(verbs[i])) > 6) {
      if (wcscmp(verbs[i] + (verblen - 6), L"backup") != 0) {
        option = L"";
      } else {
        verblen -= 6;
        option = L"--backup ";
      }
    } else {
      option = L"";
    }

    wchar_t def[MAX_PATH + 128];
    int len = _snwprintf_s(def,
                           ARRAYSIZE(def),
                           _TRUNCATE,
                           L"\"%s\" %s%.*s \"%%1\"",
                           kBinary,
                           option,
                           static_cast<int>(verblen), verbs[i]);

    // Set default value.
    if (RegSetKeyValue(hCommand,
                       NULL,
                       NULL,
                       REG_SZ,
                       def,
                       (len + 1) * sizeof(wchar_t)) != ERROR_SUCCESS) {
      wchar_t msg[512];
      _snwprintf_s(msg,
                   ARRAYSIZE(msg),
                   _TRUNCATE,
                   L"Error setting default value of key "
                    "HKEY_CLASSES_ROOT\\%s\\%s\\command",
                   subkey,
                   str);

      MessageBox(NULL, msg, L"Error", MB_OK);

      RegCloseKey(hCommand);
      RegCloseKey(hVerb);
      RegCloseKey(hSubkey);

      return false;
    }

    _M_log.format(L"HKEY_CLASSES_ROOT\\%s\\%s\\command\\(Default) = %s\r\n",
                  subkey,
                  str,
                  def);

    RegCloseKey(hCommand);
    RegCloseKey(hVerb);
  }

  RegCloseKey(hSubkey);

  return true;
}

bool dos2unix::load_file_extensions_from_file(const wchar_t* filename)
{
  FILE* file;
  if (_wfopen_s(&file, filename, L"r") != 0) {
    return false;
  }

  wchar_t line[256];
  while (fgetws(line, ARRAYSIZE(line), file)) {
    wchar_t* begin = line;
    while ((*begin) && (*begin <= L' ')) {
      begin++;
    }

    // Empty line?
    if (!*begin) {
      continue;
    }

    // Comment?
    if (*begin == L'#') {
      continue;
    }

    wchar_t* end = begin + 1;
    while (*end > L' ') {
      end++;
    }

    size_t extlen;
    if ((extlen = end - begin) > kFileExtensionMaxLength) {
      fclose(file);
      return false;
    }

    *end = L'\0';

    if (!add_file_extension(begin)) {
      fclose(file);
      return false;
    }
  }

  fclose(file);

  return true;
}

bool dos2unix::set_values(HKEY hKey,
                          const wchar_t* const key,
                          const wchar_t** subkeys,
                          const wchar_t** values,
                          size_t count)
{
  // Set subkeys with their respective values.
  for (size_t i = 0; i < count; i++) {
    // Set subkey and value.
    if (RegSetKeyValue(hKey,
                       NULL,
                       subkeys[i],
                       REG_SZ,
                       values[i],
                       (wcslen(values[i]) + 1) * sizeof(wchar_t))
        != ERROR_SUCCESS) {
      wchar_t msg[512];
      _snwprintf_s(msg,
                   ARRAYSIZE(msg),
                   _TRUNCATE,
                   L"Error setting value of key HKEY_CLASSES_ROOT\\%s\\%s",
                   key,
                   subkeys[i] ? subkeys[i] : L"(Default)");

      MessageBox(NULL, msg, L"Error", MB_OK);
      return false;
    }

    _M_log.format(L"HKEY_CLASSES_ROOT\\%s\\%s = %s\r\n",
                  key,
                  subkeys[i] ? subkeys[i] : L"(Default)",
                  values[i]);
  }

  return true;
}

bool dos2unix::generate_log_file(const wchar_t* filename) const
{
  // If the log is empty...
  if (_M_log.empty()) {
    // Nothing to do.
    return true;
  }

  // Open file for appending.
  FILE* file;
  if (_wfopen_s(&file, filename, L"a+b, ccs=UNICODE") != 0) {
    return false;
  }

  static const wchar_t* const kMonths[] = {
    L"Jan",
    L"Feb",
    L"Mar",
    L"Apr",
    L"May",
    L"Jun",
    L"Jul",
    L"Aug",
    L"Sep",
    L"Oct",
    L"Nov",
    L"Dec",
  };

  SYSTEMTIME now;
  GetLocalTime(&now);
  if (fwprintf(file,
               L"==== %02u %s %04u %02u:%02u:%02u ====\r\n",
               now.wDay,
               kMonths[now.wMonth - 1],
               now.wYear,
               now.wHour,
               now.wMinute,
               now.wSecond) != 32) {
    fclose(file);
    return false;
  }

  if (fwrite(_M_log.data(),
             sizeof(wchar_t),
             _M_log.length(),
             file) != _M_log.length()) {
    fclose(file);
    return false;
  }

  if (fwprintf(file, L"==============================\r\n") != 32) {
    fclose(file);
    return false;
  }

  fclose(file);

  return true;
}
