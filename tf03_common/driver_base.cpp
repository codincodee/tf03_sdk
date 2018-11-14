#include "driver_base.h"

DriverBase::DriverBase()
  : baud_rate_(115200) {
  buffer_cleaner_from_bytes_.store(kDefaultBufferCleanerBytes);
  LoadDevelModeTasks();
  retrieve_full_measure_stream_.store(false);
  receive_measures_.reset(new std::list<Message>);
}

DriverBase::~DriverBase() {

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
      if (parser(buffer, parsed, from, to)) {
        ++parsed_cnt;
        buffer = buffer.remove(0, to + 1);
        if (parsed.type == MessageType::measure) {
          if (full_stream) {
            Message message;
            message.type = parsed.type;
            message.data = parsed.data->Clone();
            EnqueueReceivedMeasures(std::move(message));
          }
          latest_measure_mutex_.lock();
          latest_measure_ = std::move(parsed);
          latest_measure_mutex_.unlock();
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

