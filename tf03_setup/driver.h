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

class QSerialPort;
class QByteArray;

template<typename T>
QByteArray to_bytes(const T& value) {
  QByteArray buffer(sizeof(value), 0);
  memcpy(buffer.data(), &value, sizeof(value));
  return buffer;
}

std::unique_ptr<MeasureDevel> ToMeasureDevel(
    std::unique_ptr<MeasureBasic>& basic);

enum class MessageType {
  measure,
  status,
  frequency,
  serial_number,
  output_switch,
  baud_rate,
  output_format,
  firmware_update,
  version
};

struct Message {
  MessageType type;
  std::unique_ptr<ParsedBase> data;
};

class Driver
{
 public:
  Driver();
  // Configurations
  void SetPortName(const QString& port);
  void SetBaudRate(const int& baudrate);

  // Initialize
  bool Open();
  bool Close();
  bool LastMeasure(MeasureBasic& measure);
  std::unique_ptr<MeasureBasic> LastMeasure();
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

  void SetBufferCleanerBytes(const int& bytes);
  void SetBufferCleanerBytesDefault();

  std::vector<Message> GetMessages();
  bool DetectAndAutoConnect();
  static std::vector<int> BaudRates();
  static std::vector<int> CANBaudRates();
  static int DefaultBaudRate();

 private:
  using CommandFunc = std::function<bool()>;

  bool SendMessage(const QByteArray& msg);
  void EnqueueCommand(const CommandFunc& command);
  QByteArray CommonCommand(const char& id, const QByteArray& data);
  QByteArray CalculateSum(const QByteArray& msg);
  void WorkThread();

  std::thread work_thead_;
  bool stop_signal_;

  // In work thread only
  void HandleIncomingCommandInWorkThread();
  void ProcessBufferInWorkThread(QByteArray& buffer);
  QByteArray buffer_;
  std::shared_ptr<QSerialPort> serial_port_;
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
  static bool CheckSum(const QByteArray& buffer, const int& from, const int& to);
  using ReceiveParser =
      std::function<bool(
          const QByteArray& buffer, Message& parsed, int& from, int& to)>;
  std::vector<ReceiveParser> receive_parsers_;

  std::mutex receive_messages_mutex_;
  std::vector<Message> receive_messages_;

  std::mutex latest_measure_mutex_;
  Message latest_measure_;

  std::mutex command_queue_mutex_;
  std::queue<CommandFunc> command_queue_;

  void LoadAllParsers(std::vector<ReceiveParser>& parsers);

  static std::unordered_map<char, Lingual> kEchoStatusIDMap;

  QList<QSerialPortInfo> last_serial_ports_;

  QString port_name_;
  int baud_rate_;

  std::atomic<int> buffer_cleaner_from_bytes_;
  const int kDefaultBufferCleanerBytes = 30;
};

#endif // DRIVER_H
