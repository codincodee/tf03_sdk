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
  ~MeasureBasic();
  unsigned short dist;
  unsigned short amp;
};

struct MeasureDevel : public MeasureBasic {
  unsigned short raw_dist1;
  unsigned short raw_dist2;
  unsigned short raw_dist3;
  unsigned short dist1;
  unsigned short dist2;
  unsigned short dist3;
  unsigned char apd;
  unsigned short volt;
  unsigned short temp;
};
#endif // PARSED_H
