#ifndef PARSED_H
#define PARSED_H

#include <QString>
#include "lingual.h"

struct ParsedBase {
  unsigned long id = 0;
  virtual ~ParsedBase();
};

struct StatusEcho : public ParsedBase {
  char cmd_id;
  Lingual command;
  bool success;
};

struct FrequencyEcho : public ParsedBase {
  unsigned short frequency;
};

struct SerialNumberEcho : public ParsedBase {
  QString sn;
  bool status;
};

struct VersionEcho : public ParsedBase {
  int major;
  int minor;
  int patch;
};

struct OutputSwitchEcho : public ParsedBase {
  bool on;
};

struct BaudRateEcho : public ParsedBase {
  uint32_t value;
};

enum class OutputFormat {
  nine_bytes, pix
};

struct OutputFormatEcho : public ParsedBase {
  OutputFormat format;
};

enum class FirmwareUpdateStatus {
  ok, sum_check_error, index_error, upgrade_fail
};

struct UpdateFirmwareEcho : public ParsedBase {
  FirmwareUpdateStatus status;
};

struct MeasureBasic : public ParsedBase {
  unsigned short dist;
  unsigned short amp;
};

#endif // PARSED_H
