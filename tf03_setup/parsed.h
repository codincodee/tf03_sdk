#ifndef PARSED_H
#define PARSED_H

#include <QString>
#include "lingual.h"
#include <list>

enum class MessageType {
  measure,
  status,
  frequency,
  serial_number,
  output_switch,
  baud_rate,
  output_format,
  firmware_update,
  version,
  measure_devel_stream,
  distance_echo,
};

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
  float Celsius() const;
};

struct MeasureDevelStream : public ParsedBase {
  std::list<MeasureDevel> stream;
};

enum class DistanceType {
  distance_l1, distance_l
};

struct DistanceEcho : public ParsedBase {
  virtual ~DistanceEcho();
  DistanceType type;
  uint16_t distance;
  bool success;
};
#endif // PARSED_H
