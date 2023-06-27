#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>
#include "dos2unix.h"

static void usage(LPCWSTR program);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  int argc;
  LPWSTR* argv;
  if ((argv = CommandLineToArgvW(GetCommandLineW(), &argc)) == NULL) {
    return -1;
  }

  LPCWSTR command;
  LPCWSTR filename;
  bool backup;

  // Check usage.
  switch (argc) {
    case 2:
      command = argv[1];
      filename = NULL;
      backup = false;

      break;
    case 3:
      command = argv[1];
      filename = argv[2];
      backup = false;

      break;
    case 4:
      if (_wcsicmp(argv[1], L"--backup") != 0) {
        usage(argv[0]);

        LocalFree(argv);
        return -1;
      }

      command = argv[2];
      filename = argv[3];
      backup = true;

      break;
    default:
      usage(argv[0]);

      LocalFree(argv);
      return -1;
  }

  // Install?
  if (_wcsicmp(command, L"install") == 0) {
    if (!filename) {
      usage(argv[0]);

      LocalFree(argv);
      return -1;
    }

    dos2unix dos2unix;
    bool ret = dos2unix.install(filename);

    LocalFree(argv);

    return ret ? 0 : -1;
  } else if (_wcsicmp(command, L"uninstall") == 0) {
    if (filename) {
      usage(argv[0]);

      LocalFree(argv);
      return -1;
    }

    dos2unix dos2unix;
    bool ret = dos2unix.uninstall();

    LocalFree(argv);

    return ret ? 0 : -1;
  }

  if (!filename) {
    usage(argv[0]);

    LocalFree(argv);
    return -1;
  }

  dos2unix::operation op;
  if (_wcsicmp(command, L"check") == 0) {
    op = dos2unix::operation::kCheck;
  } else if (_wcsicmp(command, L"dos2unix") == 0) {
    op = dos2unix::operation::kConvertToUnix;
  } else if (_wcsicmp(command, L"unix2dos") == 0) {
    op = dos2unix::operation::kConvertToDos;
  } else {
    usage(argv[0]);

    LocalFree(argv);
    return -1;
  }

  struct _stat sbuf;
  if (_wstat(filename, &sbuf) < 0) {
    wchar_t msg[128 + MAX_PATH];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"'%s' doesn't exist.", filename);

    MessageBox(NULL, msg, L"Error", MB_OK);

    LocalFree(argv);
    return -1;
  }

  if ((sbuf.st_mode & (_S_IFREG | _S_IFDIR)) == 0) {
    wchar_t msg[128 + MAX_PATH];
    _snwprintf_s(msg,
                 ARRAYSIZE(msg),
                 _TRUNCATE,
                 L"'%s' is neither a file nor a directory.",
                 filename);

    MessageBox(NULL, msg, L"Error", MB_OK);

    LocalFree(argv);
    return -1;
  }

  dos2unix dos2unix;

  // Directory?
  if ((sbuf.st_mode & _S_IFDIR) != 0) {
    if (!dos2unix.load_file_extensions_from_registry()) {
      LocalFree(argv);
      return -1;
    }
  }

  bool ret = true;

  switch (op) {
    case dos2unix::operation::kCheck:
      ret = dos2unix.check(filename);
      break;
    case dos2unix::operation::kConvertToUnix:
      ret = dos2unix.to_unix(filename, backup);
      break;
    case dos2unix::operation::kConvertToDos:
      ret = dos2unix.to_dos(filename, backup);
      break;
  }

  MessageBox(NULL, dos2unix.log().data(), L"Result", MB_OK);

  LocalFree(argv);

  return ret ? 0 : -1;
}

void usage(LPCWSTR program)
{
  wchar_t msg[MAX_PATH + (2 * 1024)];
  _snwprintf_s(
    msg,
    ARRAYSIZE(msg),
    _TRUNCATE,
    L"Usage: %s [<options>] [<command> [<arguments>]]\r\n"
    "\r\n"
    "Options:\r\n"
    "--backup\tGenerates a backup before converting the files.\r\n"
    "\r\n"
    "\r\n"
    "Commands:\r\n"
    "install\tInstalls the program.\r\n"
           "\tArgument: file containing file extensions.\r\n"
    "\r\n"
    "uninstall\tUninstalls the program.\r\n"
    "\r\n"
    "check\tChecks line endings.\r\n"
         "\tArgument: file or directory.\r\n"
    "\r\n"
    "dos2unix\tConverts from DOS to UNIX (CRLF -> LF).\r\n"
            "\tArgument: file or directory.\r\n"
    "\r\n"
    "unix2dos\tConverts from UNIX to DOS (LF -> CRLF).\r\n"
            "\tArgument: file or directory.\r\n",
    program
  );

  MessageBox(NULL, msg, L"Usage", MB_OK);
}
