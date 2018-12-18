#include "driver_base.h"
#include "static_unique_ptr_cast.h"
#include <QSerialPortInfo>
#include <iostream>

DriverBase::DriverBase()
  : baud_rate_(DefaultBaudRate()) {
  buffer_cleaner_from_bytes_.store(kDefaultBufferCleanerBytes);
  LoadDevelModeTasks();
  retrieve_full_measure_stream_.store(false);
  receive_measures_.reset(new std::list<Message>);
}

DriverBase::~DriverBase() {
  Close();
}

bool DriverBase::Open() {
  Close();
  LoadAllParsers(receive_parsers_);
  stop_signal_ = false;
  work_thead_ = std::thread(&DriverBase::WorkThread, this);
  return true;
}

bool DriverBase::Close() {
  stop_signal_ = true;
  if (work_thead_.joinable()) {
    work_thead_.join();
  }
  return true;
}

void DriverBase::LoadAllParsers(std::vector<ReceiveParser> &parsers) {
  parsers.clear();
}

void DriverBase::LoadDevelModeTasks() {
}

void DriverBase::WorkThread() {
  serial_port_.reset(new QSerialPort);
  serial_port_->setBaudRate(baud_rate_);
  serial_port_->setPortName(port_name_);
  if (!serial_port_->open(QIODevice::ReadWrite)) {
    return;
  }

  while (!stop_signal_) {
     HandleIncomingCommandInWorkThread();
    if (serial_port_->waitForReadyRead(100)) {
      buffer_ += serial_port_->readAll();
#ifdef DISPLAY_SERIAL_IO_MESSAGE_
  std::cout << "Received: ";
  for (auto& c : buffer_) {
    std::cout << std::hex << ushort(c) << " ";
  }
  std::cout << std::endl;
#endif
      ProcessBufferInWorkThread(buffer_);
    }
  }
  serial_port_->close();
}

void DriverBase::HandleIncomingCommandInWorkThread() {
  command_queue_mutex_.lock();
  auto queue = command_queue_;
  while (!command_queue_.empty()) {
    command_queue_.pop();
  }
  command_queue_mutex_.unlock();

  while (!queue.empty()) {
    auto command = queue.front();
    command();
    queue.pop();
  }
}

void DriverBase::ProcessBufferInWorkThread(QByteArray &buffer) {
  Message parsed;
  bool full_stream = retrieve_full_measure_stream_.load();
  while (true) {
    int from = 0, to = 0;
    int parsed_cnt = 0;
    for (auto& parser : receive_parsers_) {
      parsed.type = MessageType::unknown;
      if (parser(buffer, parsed, from, to)) {
        ++parsed_cnt;
        buffer = buffer.remove(0, to + 1);
        if (parsed.type == MessageType::measure) {
          if (parsed.data) {
            parsed.data->id = ++measure_id_;
          }
          if (full_stream) {
            Message message;
            message.type = parsed.type;
            message.data = parsed.data->Clone();
            EnqueueReceivedMeasures(std::move(message));
          }
          latest_measure_mutex_.lock();
          latest_measure_ = std::move(parsed);
          if (latest_measure_.data) {
            latest_measure_static_ =
                dynamic_unique_ptr_cast<MeasureBasic>(
                    latest_measure_.data->Clone());
          }
          latest_measure_mutex_.unlock();
        } else if (parsed.type == MessageType::unknown) {
          // Skip
        } else {
          EnqueueReceivedMessages(std::move(parsed));
        }
      }
    }
    if (parsed_cnt == 0) {
      if (buffer.size() > buffer_cleaner_from_bytes_.load()) {
        buffer.clear();
      }
      break;
    }
  }
}

void DriverBase::EnqueueReceivedMessages(Message message) {
  receive_messages_mutex_.lock();
  receive_messages_.emplace_back(std::move(message));
  receive_messages_mutex_.unlock();
}

void DriverBase::EnqueueReceivedMeasures(Message message) {
  receive_measures_mutex_.lock();
  receive_measures_->emplace_back(std::move(message));
  receive_measures_mutex_.unlock();
}

void DriverBase::SetBufferCleanerBytes(const int &bytes) {
  buffer_cleaner_from_bytes_.store(bytes);
}

void DriverBase::SetBufferCleanerBytesDefault() {
  SetBufferCleanerBytes(kDefaultBufferCleanerBytes);
}

void DriverBase::SetPortName(const QString &port) {
  port_name_ = port;
}

void DriverBase::SetBaudRate(const int &baudrate) {
  baud_rate_ = baudrate;
}

bool DriverBase::LastMeasure(MeasureBasic &measure) {
  latest_measure_mutex_.lock();
  auto ptr =
      dynamic_unique_ptr_cast<MeasureBasic>(std::move(latest_measure_.data));
  latest_measure_mutex_.unlock();
  if (ptr) {
    measure = *ptr;
    return true;
  } else {
    return false;
  }
}

std::unique_ptr<MeasureBasic> DriverBase::LastMeasure() {
  latest_measure_mutex_.lock();
  auto measure =
      dynamic_unique_ptr_cast<MeasureBasic>(std::move(latest_measure_.data));
  latest_measure_mutex_.unlock();
  return measure;
}

std::unique_ptr<MeasureBasic> DriverBase::Measure() {
  return LastMeasureStatic();
}

std::unique_ptr<MeasureBasic> DriverBase::LastMeasureStatic() {
  latest_measure_mutex_.lock();
  std::unique_ptr<MeasureBasic> measure;
  if (latest_measure_static_) {
    measure =
        dynamic_unique_ptr_cast<MeasureBasic>(latest_measure_static_->Clone());
  }
  latest_measure_mutex_.unlock();
  return measure;
}

std::vector<int> DriverBase::DriverBaudRates() {
  return BaudRates();
}

std::vector<int> DriverBase::BaudRates() {
  return {
#ifdef SUPPORT_10KHZ_OUTPUT
    1000000,
#endif
    115200, 460800, 256000, 57600, 38400, 19200, 9600, 4800, 2400};
}

std::vector<int> DriverBase::CANBaudRates() {
  return {1000000, 500000, 250000, 125000};
}

int DriverBase::DefaultBaudRate() {
  return 115200;
}

bool DriverBase::DetectAndAutoConnect() {
  auto ports = QSerialPortInfo::availablePorts();
  if (ports.isEmpty()) {
    last_serial_ports_.clear();
  }
  if (ports.size() != 1) {
    return false;
  }
  if (last_serial_ports_.isEmpty()) {
    last_serial_ports_ = ports;
    Close();
    SetPortName(ports.front().portName());
    Open();
    return true;
  }
  if (last_serial_ports_.size() != 1) {
    return false;
  }
  if (last_serial_ports_.front().portName() != ports.front().portName()) {
    last_serial_ports_ = ports;
    Close();
    SetPortName(ports.front().portName());
    Open();
    return true;
  }
  return false;
}

void DriverBase::SwitchOnMeasureStream(const bool &on) {
  receive_measures_mutex_.lock();
  receive_measures_.reset(new std::list<Message>);
  receive_measures_mutex_.unlock();
  retrieve_full_measure_stream_.store(on);
}

void DriverBase::EnqueueCommand(const CommandFunc &command) {
  command_queue_mutex_.lock();
  command_queue_.push(command);
  command_queue_mutex_.unlock();
}

bool DriverBase::SendMessage(const QByteArray &msg) {
  if (!serial_port_) {
    return false;
  }
  if (!serial_port_->isOpen()) {
    return false;
  }
#ifdef DISPLAY_SERIAL_IO_MESSAGE_
  std::cout << "Sent: ";
  for (auto& c : msg) {
    std::cout << std::hex << ushort(c) << " ";
  }
  std::cout << std::endl;
#endif
  serial_port_->write(msg);
  return true;
}

std::vector<Message> DriverBase::GetMessages() {
  receive_messages_mutex_.lock();
  auto messages = std::move(receive_messages_);
  receive_messages_mutex_.unlock();
  return messages;
}

QByteArray DriverBase::CalculateSum(const QByteArray &msg) {
  char sum = 0;
  for (auto& b : msg) {
    sum += b;
  }
  return QByteArray(1, sum);
}

bool DriverBase::CheckSum(
    const QByteArray &buffer, const int &from, const int &to) {
  char sum = 0;
  for (int i = from; i < to; ++i) {
    sum += buffer[i];
  }
  return buffer[to] == sum;
}

std::shared_ptr<std::list<Message>> DriverBase::GetMeasures() {
  receive_measures_mutex_.lock();
  auto ptr = receive_measures_;
  receive_measures_.reset(new std::list<Message>);
  receive_measures_mutex_.unlock();
  return ptr;
}
