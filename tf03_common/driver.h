#ifndef DRIVER_H
#define DRIVER_H

#include <memory>
#include <thread>
#include <vector>
#include <functional>
#include "parsed.h"
#include <mutex>
#include <queue>
#include <unordered_map>
#include "lingual.h"
#include <QSerialPortInfo>
#include <atomic>
#include "export.h"
#include "driver_base.h"

class QSerialPort;
class QByteArray;
class DevelModeTask;
class APDExpTask;
class RangeDetectTask;

std::unique_ptr<MeasureDevel> API ToMeasureDevel(
    std::unique_ptr<MeasureBasic>& basic);

enum class API Customization {
  common, bl, i13, ql
};

class API Driver : public DriverBase
{
 public:
//  Driver();
//  // Configurations
//  void SetPortName(const QString& port);
//  void SetBaudRate(const int& baudrate);

//  // Initialize
//  bool Open();
//  bool Close();
//  bool LastMeasure(MeasureBasic& measure);
//  std::unique_ptr<MeasureBasic> LastMeasure();
  void SetDevelMode();
  void SetReleaseMode();
  void SetFrequency(const unsigned short& frequency);
  void RequestSerialNumber();
  void SetOutputSwitchOn();
  void SetOutputSwitchOff();
  void TriggerOnce();
  void SaveSettingsToFlash();
  void RestoreFactory();
  void SetDeviceBaudRate(const uint32_t& rate);
  void SetTransTypeSerial();
  void SetTransTypeCAN();
  void SetOutputFormatNineBytes();
  void SetOutputFormatPIX();
  void SetOutputFormatIO();
  void SetCANSendID(const uint32_t& id);
  void SetCANReceiveID(const uint32_t& id);
  void SetDeviceCANBaudRate(const uint32_t& rate);
  void SetOutRangeValue(const uint16_t& value);
  void SendFirmwareSegment(const uint16_t& id, const QByteArray& seg);
  void SendFirmwareLastSegment(const uint16_t& id, const QByteArray& seg);
  void SendFirmwareFirstSegment(const uint16_t& bytes, const QByteArray &seg);
  void SendFirmwareMultiSegment(
      const uint16_t& id, const std::vector<QByteArray>& segments);
  void RequestVersion();
  void WriteSerialNumber(const QString& sn);
  void SetAPD(const uint8_t& voltage);
  void SetAutoGainAdjust(const bool& on);
  void SetAdaptiveAPD(const bool& on);
  void SetAPDClosedLoop(const bool& on);
  void APDExperimentOn();
  void APDExperimentOff();
  void SetDistanceL1(const uint16_t& value);
  void RequestDistanceL1();
  void SetDistanceL(const uint16_t& value);
  void RequestDistanceL();
  void SetCustomization(const Customization& type);
  void RangeDetectionTask(const bool& on);
  void SetHorizontalAngle(const uint8_t& value);
  void SetVdbsAutoAdjust(const bool& on);
  void SetCANFrameTypeExtended();
  void SetCANFrameTypeStandard();
  void SetRangeDetectTaskThreshold(const unsigned short& threshold);
//  void SwitchOnMeasureStream(const bool& on);

//  void SetBufferCleanerBytes(const int& bytes);
//  void SetBufferCleanerBytesDefault();

//  std::vector<Message> GetMessages();
//  std::shared_ptr<std::list<Message>> GetMeasures();
//  bool DetectAndAutoConnect();
//  static std::vector<int> BaudRates();
//  static std::vector<int> CANBaudRates();
//  static int DefaultBaudRate();

 private:
  friend class DevelModeTask;
//  using CommandFunc = std::function<bool()>;

//  bool SendMessage(const QByteArray& msg);
//  void EnqueueCommand(const CommandFunc& command);
//  void EnqueueReceivedMessages(Message message);
//  void EnqueueReceivedMeasures(Message message);
  QByteArray CommonCommand(const char& id, const QByteArray& data);
//  QByteArray CalculateSum(const QByteArray& msg);
//  void WorkThread();

  void HandleDevelMeasureOtherTasks(const MeasureDevel& measure);
  void LoadDevelModeTasks() override;
  std::vector<std::shared_ptr<DevelModeTask>> devel_mode_tasks_;

//  std::thread work_thead_;
//  bool stop_signal_;

//  // In work thread only
//  void HandleIncomingCommandInWorkThread();
//  void ProcessBufferInWorkThread(QByteArray& buffer);
//  QByteArray buffer_;
//  std::shared_ptr<QSerialPort> serial_port_;
  unsigned long measure_id_ = 0;

  // Parsers
  bool ParseNineByteMeasure(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static QByteArray Parse0x5AMessageAtFront(
      const QByteArray& buffer, int& from, int& to);
  static bool ParseStatusEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseFrequencyEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseSerialNumberEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseVersionEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseOutputSwitchEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseBaudRateEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseOutputFormatEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseUpdateFirmwareEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  bool ParseDevelModeMeasure(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static QByteArray ParseNineByteMeasureMessageAtFront(
      const QByteArray& buffer, int& from, int& to);
  bool ParsePixhawkMeasure(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static QByteArray ParsePixhawkMeasureMessageAtFront(
      const QByteArray& buffer, int& from, int& to);
  static bool ParseDistanceL1Echo(
      const QByteArray &buffer, Message &parsed, int &from, int &to);
  static bool ParseDistanceLEcho(
      const QByteArray &buffer, Message &parsed, int &from, int &to);
//  static bool CheckSum(const QByteArray& buffer, const int& from, const int& to);
//  using ReceiveParser =
//      std::function<bool(
//          const QByteArray& buffer, Message& parsed, int& from, int& to)>;
//  std::vector<ReceiveParser> receive_parsers_;

//  std::mutex receive_messages_mutex_;
//  std::vector<Message> receive_messages_;

//  std::mutex latest_measure_mutex_;
//  Message latest_measure_;

//  std::mutex command_queue_mutex_;
//  std::queue<CommandFunc> command_queue_;

//  std::mutex receive_measures_mutex_;
//  std::shared_ptr<std::list<Message>> receive_measures_;

  void LoadAllParsers(std::vector<ReceiveParser>& parsers) override;

  static std::unordered_map<char, Lingual> kEchoStatusIDMap;

//  QList<QSerialPortInfo> last_serial_ports_;

//  QString port_name_;
//  int baud_rate_;

//  std::atomic<int> buffer_cleaner_from_bytes_;
//  const int kDefaultBufferCleanerBytes = 30;

  std::shared_ptr<APDExpTask> apd_exp_task_;
  std::shared_ptr<RangeDetectTask> range_detect_task_;

//  std::atomic_bool retrieve_full_measure_stream_;
};

#endif // DRIVER_H
