#include "tfmini_driver.h"

TFMiniDriver::TFMiniDriver()
{

}

void TFMiniDriver::LoadAllParsers(std::vector<ReceiveParser> &parsers) {
  DriverBase::LoadAllParsers(parsers);
  parsers.push_back(
      std::bind(
          &TFMiniDriver::ParseDevelModeMeasure,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
}

bool TFMiniDriver::ParseDevelModeMeasure(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse29BytesMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 29) {
    return false;
  }
  parsed.type = MessageType::measure;
  std::unique_ptr<MiniMeasure29B> data(new MiniMeasure29B);
  memcpy(&data->raw_dist, msg.data() + 2, 2);
  memcpy(&data->amp, msg.data() + 4, 2);
  memcpy(&data->temp, msg.data() + 6, 2);
  memcpy(&data->amb, msg.data() + 8, 2);
  memcpy(&data->temp_d, msg.data() + 10, 2);
  memcpy(&data->amb_d, msg.data() + 12, 2);
  memcpy(&data->ref_d, msg.data() + 14, 2);
  memcpy(&data->cor_d, msg.data() + 16, 2);
  memcpy(&data->table_d, msg.data() + 18, 2);
  memcpy(&data->temp_amp, msg.data() + 20, 2);
  memcpy(&data->amb_amp, msg.data() + 22, 2);
  memcpy(&data->dcs3, msg.data() + 24, 2);
  memcpy(&data->inttime, msg.data() + 26, 1);
  data->dist = data->cor_d;
  parsed.data = std::move(data);
  return true;
}

QByteArray TFMiniDriver::Parse29BytesMessageAtFront(
    const QByteArray &buffer, int &from, int &to) {
  constexpr int len = 29;
  if (buffer.size() < len) {
    return QByteArray();
  }
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == 0x59) {
      auto ii = i + 1;
      if (ii < buffer.size()) {
        if (buffer[ii] == 0x59) {
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
  }
  return result;
}
