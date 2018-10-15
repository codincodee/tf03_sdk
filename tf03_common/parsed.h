#ifndef PARSED_H
#define PARSED_H

#include <QString>
#include "lingual.h"
#include <list>
#include "export.h"

#include <QElapsedTimer>

enum class API MessageType {
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
  out_of_range
};

struct API ParsedBase {
  unsigned long id = 0;
  virtual ~ParsedBase();
};

struct API StatusEcho : public ParsedBase {
  char cmd_id;
  Lingual command;
  bool success;
};

struct API FrequencyEcho : public ParsedBase {
  unsigned short frequency;
};

struct API SerialNumberEcho : public ParsedBase {
  QString sn;
  bool status;
};

struct API VersionEcho : public ParsedBase {
  int major;
  int minor;
  int patch;
};

struct API OutputSwitchEcho : public ParsedBase {
  bool on;
};

struct API BaudRateEcho : public ParsedBase {
  uint32_t value;
};

enum class API OutputFormat {
  nine_bytes, pix
};

struct API OutputFormatEcho : public ParsedBase {
  OutputFormat format;
};

enum class API FirmwareUpdateStatus {
  ok, sum_check_error, index_error, upgrade_fail
};

struct API UpdateFirmwareEcho : public ParsedBase {
  FirmwareUpdateStatus status;
};

struct API OutOfRangeEcho : public ParsedBase {
  bool out_of_range;
};

struct API MeasureBasic : public ParsedBase {
  ~MeasureBasic();
  unsigned short dist;
  unsigned short amp;
};

struct API MeasureDevel : public MeasureBasic {
  virtual ~MeasureDevel();
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

struct API MeasureDevelStamped : public MeasureDevel {
  MeasureDevelStamped();
  MeasureDevelStamped(const MeasureDevel& devel);
  QElapsedTimer stamp;
};

struct API MeasureDevelStream : public ParsedBase {
  std::list<MeasureDevel> stream;
};

enum class API DistanceType {
  distance_l1, distance_l
};

struct API DistanceEcho : public ParsedBase {
  virtual ~DistanceEcho();
  DistanceType type;
  uint16_t distance;
  bool success;
};
#endif // PARSED_H
