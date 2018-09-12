#include "driver.h"
#include <QDebug>
#include <iostream>
#include <QElapsedTimer>

void Driver::LoadAllParsers(std::vector<ReceiveParser> &parsers) {
  parsers.clear();
  parsers.push_back(
      std::bind(
          Driver::ParseStatusEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseFrequencyEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseSerialNumberEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseOutputSwitchEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseBaudRateEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseOutputFormatEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseVersionEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          &Driver::ParseNineByteMeasure,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          &Driver::ParsePixhawkMeasure,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseUpdateFirmwareEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
#ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
  parsers.push_back(
      std::bind(
          &Driver::ParseDevelModeMeasure,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
#endif
}

bool Driver::ParseFrequencyEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 6) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x03)) {
    return false;
  }
  parsed.type = MessageType::frequency;
  std::unique_ptr<FrequencyEcho> data(new FrequencyEcho);
  memcpy(&data->frequency, msg.data() + 3, 2);
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseSerialNumberEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() < 6) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x56)) {
    return false;
  }
  parsed.type = MessageType::serial_number;
  std::unique_ptr<SerialNumberEcho> data(new SerialNumberEcho);
  auto len = msg[1] - 5;
  auto sn = QByteArray(len, 0);
  data->status = (msg[3] == char(0x00));
  memcpy(sn.data(), msg.data() + 4, len);
  data->sn = sn;
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseVersionEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 7) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x01)) {
    return false;
  }
  parsed.type = MessageType::version;
  std::unique_ptr<VersionEcho> data(new VersionEcho);
  data->patch = msg[3];
  data->minor = msg[4];
  data->major = msg[5];
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseOutputSwitchEcho(
    const QByteArray& buffer, Message& parsed, int& from, int& to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 5) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x07)) {
    return false;
  }
  parsed.type = MessageType::output_switch;
  std::unique_ptr<OutputSwitchEcho> data(new OutputSwitchEcho);
  data->on = (msg[3] == char(1));
  parsed.data = std::move(data);
  return true;
}


bool Driver::ParseBaudRateEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 8) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x06)) {
    return false;
  }
  parsed.type = MessageType::baud_rate;
  std::unique_ptr<BaudRateEcho> data(new BaudRateEcho);
  memcpy(&data->value, msg.data() + 3, 4);
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseOutputFormatEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 5) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x05)) {
    return false;
  }
  parsed.type = MessageType::output_format;
  std::unique_ptr<OutputFormatEcho> data(new OutputFormatEcho);
  if (msg[3] == char(0x01)) {
    data->format = OutputFormat::nine_bytes;
  } else if (msg[3] = char(0x02)) {
    data->format = OutputFormat::pix;
  } else {
    return false;
  }
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseUpdateFirmwareEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 5) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x49)) {
    return false;
  }
  parsed.type = MessageType::firmware_update;
  std::unique_ptr<UpdateFirmwareEcho> data(new UpdateFirmwareEcho);
  if (msg[3] == char(0x00)) {
    data->status = FirmwareUpdateStatus::ok;
  } else if (msg[3] == char(0x01)) {
    data->status = FirmwareUpdateStatus::sum_check_error;
  } else if (msg[3] == char(0x02)) {
    data->status = FirmwareUpdateStatus::index_error;
  } else if (msg[3] == char(0x03)) {
    data->status = FirmwareUpdateStatus::upgrade_fail;
  } else {
    return false;
  }
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseDevelModeMeasure(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 22) {
    return false;
  }
  parsed.type = MessageType::measure;
  std::unique_ptr<MeasureDevel> data(new MeasureDevel);
  memcpy(&data->raw_dist1, msg.data() + 4, 2);
  memcpy(&data->raw_dist2, msg.data() + 6, 2);
  memcpy(&data->raw_dist3, msg.data() + 8, 2);
  memcpy(&data->dist1, msg.data() + 10, 2);
  memcpy(&data->dist2, msg.data() + 12, 2);
  memcpy(&data->dist3, msg.data() + 14, 2);
  memcpy(&data->apd, msg.data() + 16, 1);
  memcpy(&data->volt, msg.data() + 17, 2);
  memcpy(&data->temp, msg.data() + 19, 2);
  data->dist = data->dist1;
  data->amp = 0;
  data->id = measure_id_++;
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseStatusEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 5)  {
    return false;
  }
  auto id = msg[2];
  // qDebug() << short(id);
  if (kEchoStatusIDMap.count(id) == 0) {
    return false;
  }
  parsed.type = MessageType::status;
  std::unique_ptr<StatusEcho> data(new StatusEcho);
  data->command = kEchoStatusIDMap[id];
  data->success = (msg.at(3) == 0);
  data->cmd_id = id;
  parsed.data = std::move(data);
  return true;
}

QByteArray Driver::Parse0x5AMessageAtFront(
    const QByteArray &buffer, int &from, int &to) {
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == 0x5A) {
      auto ii = i + 1;
      if (ii < buffer.size()) {
        auto len = buffer[ii];
        auto end = ii + len - 2;
        if (end < buffer.size()) {
          if (CheckSum(buffer, i, end)) {
            from = i;
            to = end;
            result.reserve(to - from + 1);
            for (int j = from; j <= to; ++j) {
              result.push_back(buffer[j]);
            }
            break;
          }
        }
      }
    }
  }
  return result;
}

bool Driver::ParseNineByteMeasure(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = ParseNineByteMeasureMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  std::unique_ptr<MeasureBasic> measure(new MeasureBasic);
  ++measure_id_;
  measure->id = measure_id_;
  memcpy(&measure->dist, msg.data() + 2, 2);
  memcpy(&measure->amp, msg.data() + 4, 2);
  parsed.type = MessageType::measure;
  parsed.data = std::move(measure);

//  static QElapsedTimer timer;
//  static bool init = true;
//  static int cnt = 0;
//  if (init) {
//    init = false;
//    timer.start();
//  }
//  ++cnt;
//  auto elapse = timer.elapsed();
//  if (elapse > 1000) {
//    qDebug() << cnt * 1000.0 / elapse;
//    cnt = 0;
//    timer.restart();
//  }
  return true;
}

QByteArray Driver::ParseNineByteMeasureMessageAtFront(
    const QByteArray &buffer, int &from, int &to) {
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == 0x59) {
      auto ii = i + 1;
      if (ii < buffer.size()) {
        if (buffer[ii] == 0x59) {
          auto end = ii + 7;
          if (end < buffer.size()) {
            if (CheckSum(buffer, i, end)) {
              from = i;
              to = end;
              result.reserve(to - from + 1);
              for (int j = from; j <= to; ++j) {
                result.push_back(buffer[j]);
              }
              break;
            }
          }
        }
      }
    }
  }
  return result;
}

bool Driver::ParsePixhawkMeasure(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = ParsePixhawkMeasureMessageAtFront(buffer, from, to);
  msg.remove(msg.size() - 2, 2);
  bool ok;
  auto dist = msg.toFloat(&ok);
  if (!ok) {
    return false;
  }
  std::unique_ptr<MeasureBasic> measure(new MeasureBasic);
  ++measure_id_;
  measure->id = measure_id_;
  measure->dist = std::round(dist * 100);
  parsed.type = MessageType::measure;
  parsed.data = std::move(measure);
  return true;
}

QByteArray Driver::ParsePixhawkMeasureMessageAtFront(
    const QByteArray &buffer, int &from, int &to) {
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == '\r') {
      auto ii = i + 1;
      if (ii < buffer.size()) {
        if (buffer[ii] == '\n') {
          auto _iiii = i - 3;
          if (_iiii >= 0) {
            if (buffer[_iiii] == '.') {
              int j = _iiii;
              while (j >= 0) {
                --j;
                if (!isdigit(buffer[j])) {
                  break;
                }
              }
              from = j + 1;
              to = ii;
              result.reserve(to - from + 1);
              for (int k = from; k <= to; ++k) {
                result.push_back(buffer[k]);
              }
              break;
            }
          }
        }
      }
    }
  }
  return result;
}

bool Driver::CheckSum(const QByteArray &buffer, const int &from, const int &to) {
  char sum = 0;
  for (int i = from; i < to; ++i) {
    sum += buffer[i];
  }
  return buffer[to] == sum;
}
