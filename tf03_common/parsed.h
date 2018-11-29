#ifndef PARSED_H
#define PARSED_H

#include <QString>
#include "lingual.h"
#include <list>
#include "export.h"

#include <QElapsedTimer>

enum class API MessageType {
  unknown,
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
  out_of_range,
};

struct API ParsedBase {
  unsigned long id = 0;
  virtual ~ParsedBase();
  virtual std::unique_ptr<ParsedBase> Clone();
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
  nine_bytes, pix, io, k485, msg
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
  bool too_near;
};

struct API MeasureBasic : public ParsedBase {
  ~MeasureBasic();
  virtual std::unique_ptr<ParsedBase> Clone() override;
  unsigned short dist;
  unsigned short amp;
// #ifdef ATTACH_RAW_MSG_TO_MEASURE
  QByteArray raw_msg;
// #endif
};

struct API MeasureDevel : public MeasureBasic {
  virtual ~MeasureDevel();
  virtual std::unique_ptr<ParsedBase> Clone() override;
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

struct API MiniMeasure29B : public MeasureBasic {
  virtual ~MiniMeasure29B();
  virtual std::unique_ptr<ParsedBase> Clone() override;
  unsigned short raw_dist;
  unsigned short temp;
  unsigned short amb;
  unsigned short temp_d;
  unsigned short amb_d;
  unsigned short ref_d;
  unsigned short cor_d;
  unsigned short table_d;
  unsigned short temp_amp;
  unsigned short amb_amp;
  unsigned short dcs3;
  uint8_t inttime;
};

MeasureBasic ToMeasureBasic(const MeasureDevel& devel);

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

struct API Message {
  MessageType type;
  std::unique_ptr<ParsedBase> data;
};

#endif // PARSED_H
