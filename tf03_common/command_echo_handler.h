#ifndef COMMAND_ECHO_HANDLER_H
#define COMMAND_ECHO_HANDLER_H

#include <memory>
#include <unordered_map>
#include <vector>
#include "export.h"

class Driver;
class QString;
enum class OutputFormat;
enum class FirmwareUpdateStatus;
struct VersionEcho;
struct MeasureDevelStream;
struct DistanceEcho;

class API CommandEchoHandler
{
public:
  CommandEchoHandler();
  void SetDriver(std::shared_ptr<Driver> driver);
  void Probe();
  bool IsCommandEchoed(const char& id);
  bool IsCommandSucceeded(const char& id);
  bool IsFrequencyEchoed();
  unsigned short Frequency();
  bool IsSerialNumberEchoed();
  QString SerialNumber();
  bool IsOutputSwitchEchoed();
  bool IsOutputOn();
  bool IsBaudRateEchoed();
  int BaudRate();
  bool IsOutputFormatEchoed();
  OutputFormat GetOutputFormat();
  bool IsFirmwareUpdateEchoed();
  FirmwareUpdateStatus GetFirmwareUpdateStatus();
  bool IsVersionEchoed();
  VersionEcho Version();
  bool IsMeasureDevelStreamEchoed();
  MeasureDevelStream GetMeasureDevelStream();
  bool IsDistanceL1Echoed();
  DistanceEcho GetDistanceL1();
  bool IsDistanceLEchoed();
  DistanceEcho GetDistanceL();
  bool IsRangeDetectEchoed();
  bool IsOutOfRange();
  bool IsRangeDetectTooNear();
private:
  std::shared_ptr<Driver> driver_;
  std::unordered_map<char, bool> echo_map_;
  std::vector<unsigned short> frequencies_;
  std::vector<std::pair<bool, QString>> serial_numbers_;
  std::vector<bool> output_status_;
  std::vector<int> baud_rates_;
  std::vector<OutputFormat> output_formats_;
  std::vector<FirmwareUpdateStatus> firmware_update_status_;
  std::vector<VersionEcho> versions_;
  std::vector<MeasureDevelStream> measure_devel_stream_;
  std::vector<DistanceEcho> distance_l1_;
  std::vector<DistanceEcho> distance_l_;
  std::vector<bool> out_of_range_;
  std::vector<bool> range_detect_too_near_;
};

#endif // COMMAND_ECHO_HANDLER_H
