#ifndef DRIVER_BASE_H
#define DRIVER_BASE_H

#include "export.h"
#include <atomic>
#include <memory>
#include <vector>
#include <functional>
#include <QByteArray>
#include "parsed.h"
#include <QSerialPort>
#include <queue>

class API DriverBase
{
public:
  DriverBase();
  virtual ~DriverBase();
  bool Open();
  bool Close();
  void SetBufferCleanerBytes(const int &bytes);
  void SetBufferCleanerBytesDefault();
  void SetPortName(const QString &port);
  void SetBaudRate(const int &baudrate);
  bool LastMeasure(MeasureBasic &measure);
  std::unique_ptr<MeasureBasic> LastMeasure();
  static std::vector<int> BaudRates();
  static std::vector<int> CANBaudRates();
  static int DefaultBaudRate();
  bool DetectAndAutoConnect();
  void SwitchOnMeasureStream(const bool &on);
  std::vector<Message> DriverBase::GetMessages();
  std::shared_ptr<std::list<Message>> GetMeasures();

protected:
  using ReceiveParser =
      std::function<bool(
          const QByteArray& buffer, Message& parsed, int& from, int& to)>;
  using CommandFunc = std::function<bool()>;
  virtual void LoadAllParsers(std::vector<ReceiveParser>& parsers);
  virtual void LoadDevelModeTasks();
  void WorkThread();
  void HandleIncomingCommandInWorkThread();
  void ProcessBufferInWorkThread(QByteArray& buffer);
  void EnqueueReceivedMeasures(Message message);
  void EnqueueReceivedMessages(Message message);
  void EnqueueCommand(const CommandFunc &command);
  bool SendMessage(const QByteArray &msg);
  QByteArray CalculateSum(const QByteArray &msg);
  static bool CheckSum(const QByteArray &buffer, const int &from, const int &to);

  std::vector<ReceiveParser> receive_parsers_;
  std::atomic<int> buffer_cleaner_from_bytes_;
  std::atomic_bool retrieve_full_measure_stream_;
  const int kDefaultBufferCleanerBytes = 200;
  int baud_rate_;
  bool stop_signal_;
  std::thread work_thead_;
  std::shared_ptr<QSerialPort> serial_port_;
  QString port_name_;
  QByteArray buffer_;
  std::mutex command_queue_mutex_;
  std::queue<CommandFunc> command_queue_;
  std::mutex latest_measure_mutex_;
  Message latest_measure_;
  std::mutex receive_messages_mutex_;
  std::vector<Message> receive_messages_;
  std::mutex receive_measures_mutex_;
  std::shared_ptr<std::list<Message>> receive_measures_;
  QList<QSerialPortInfo> last_serial_ports_;
};

#endif // DRIVER_BASE_H
