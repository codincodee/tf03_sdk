#include "driver.h"
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QElapsedTimer>
#include "static_unique_ptr_cast.h"

std::unordered_map<char, Lingual> Driver::kEchoStatusIDMap{
  {0x44, {"Communication Protocol", "通信协议"}},
  {0x11, {"Save Settings to Flash", "写入设置"}},
  {0x10, {"Restore Factory", "恢复出厂设置"}},
  {0x45, {"Trans Type", "传输类型"}},
  {0x50, {"CAN Send ID", "CAN发送ID"}},
  {0x51, {"CAN Receive ID", "CAN接收ID"}},
  {0x52, {"CAN Baud Rate", "CAN波特率"}},
  {0x4f, {"Out-range Value", "超量程输出值"}},
  {0x56, {"Write Serial Number", "写入序列号"}}
};

Driver::Driver()
  : baud_rate_(115200) {
  buffer_cleaner_from_bytes_.store(kDefaultBufferCleanerBytes);
}

void Driver::SetBufferCleanerBytes(const int &bytes) {
  buffer_cleaner_from_bytes_.store(bytes);
}

void Driver::SetBufferCleanerBytesDefault() {
  SetBufferCleanerBytes(kDefaultBufferCleanerBytes);
}

void Driver::SetPortName(const QString &port) {
  port_name_ = port;
}

void Driver::SetBaudRate(const int &baudrate) {
  baud_rate_ = baudrate;
}

bool Driver::Open() {
  Close();
  LoadAllParsers(receive_parsers_);
  stop_signal_ = false;
  work_thead_ = std::thread(&Driver::WorkThread, this);
  return true;
}

bool Driver::Close() {
  stop_signal_ = true;
  if (work_thead_.joinable()) {
    work_thead_.join();
  }
  return true;
}

bool Driver::LastMeasure(MeasureBasic &measure) {
  latest_measure_mutex_.lock();
  auto ptr =
      static_unique_ptr_cast<MeasureBasic>(std::move(latest_measure_.data));
  latest_measure_mutex_.unlock();
  if (ptr) {
    measure = *ptr;
    return true;
  } else {
    return false;
  }
}

void Driver::WorkThread() {
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
      ProcessBufferInWorkThread(buffer_);
    }
  }
  serial_port_->close();
}

void Driver::HandleIncomingCommandInWorkThread() {
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

void Driver::ProcessBufferInWorkThread(QByteArray &buffer) {
  Message parsed;
  while (true) {
    int from = 0, to = 0;
    int parsed_cnt = 0;
    for (auto& parser : receive_parsers_) {
      if (parser(buffer, parsed, from, to)) {
        ++parsed_cnt;
        buffer = buffer.remove(0, to + 1);
        if (parsed.type == MessageType::measure) {
          latest_measure_mutex_.lock();
          latest_measure_ = std::move(parsed);
          latest_measure_mutex_.unlock();
        } else {
          receive_messages_mutex_.lock();
          receive_messages_.emplace_back(std::move(parsed));
          receive_messages_mutex_.unlock();
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

#include <iostream>

bool Driver::SendMessage(const QByteArray &msg) {
  if (!serial_port_) {
    return false;
  }
  if (!serial_port_->isOpen()) {
    return false;
  }
//  for (auto& c : msg) {
//    std::cout << std::hex << ushort(c) << " ";
//  }
//  std::cout << std::endl;
  serial_port_->write(msg);
  return true;
}

std::vector<int> Driver::BaudRates() {
  return {115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200};
}

std::vector<int> Driver::CANBaudRates() {
  return {1000000, 500000, 250000, 125000};
}

int Driver::DefaultBaudRate() {
  return 115200;
}

bool Driver::DetectAndAutoConnect() {
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
