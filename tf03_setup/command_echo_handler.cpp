#include "command_echo_handler.h"
#include "driver.h"
#include "static_unique_ptr_cast.h"
#include <QDebug>

CommandEchoHandler::CommandEchoHandler()
{

}

void CommandEchoHandler::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void CommandEchoHandler::Probe() {
  if (!driver_) {
    return;
  }
  echo_map_.clear();
  frequencies_.clear();
  serial_numbers_.clear();
  output_status_.clear();
  baud_rates_.clear();
  output_formats_.clear();
  firmware_update_status_.clear();
  versions_.clear();
#ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
  measure_devel_stream_.clear();
  out_of_range_.clear();
#endif
#ifdef CLIENT_BL_CUSTOMIZATION
  distance_l1_.clear();
  distance_l_.clear();
#endif

  auto message = driver_->GetMessages();
  for (auto& msg : message) {
    if (msg.type == MessageType::status) {
      auto status = dynamic_unique_ptr_cast<StatusEcho>(std::move(msg.data));
      if (status) {
        echo_map_[status->cmd_id] = status->success;
      }
    } else if (msg.type == MessageType::frequency) {
      auto freq = dynamic_unique_ptr_cast<FrequencyEcho>(std::move(msg.data));
      if (freq) {
        frequencies_.push_back(freq->frequency);
      }
    } else if (msg.type == MessageType::serial_number) {
      auto sn = dynamic_unique_ptr_cast<SerialNumberEcho>(std::move(msg.data));
      if (sn) {
        serial_numbers_.push_back({sn->status, sn->sn});
      }
    } else if (msg.type == MessageType::output_switch) {
      auto status =
          dynamic_unique_ptr_cast<OutputSwitchEcho>(std::move(msg.data));
      if (status) {
        output_status_.push_back(status->on);
      }
    } else if (msg.type == MessageType::baud_rate) {
      auto baud_rate = dynamic_unique_ptr_cast<BaudRateEcho>(std::move(msg.data));
      if (baud_rate) {
        baud_rates_.push_back(baud_rate->value);
      }
    } else if (msg.type == MessageType::output_format) {
      auto format =
          dynamic_unique_ptr_cast<OutputFormatEcho>(std::move(msg.data));
      if (format) {
        output_formats_.push_back(format->format);
      }
    } else if (msg.type == MessageType::firmware_update) {
      auto status =
          dynamic_unique_ptr_cast<UpdateFirmwareEcho>(std::move(msg.data));
      if (status) {
        firmware_update_status_.push_back(status->status);
      }
    } else if (msg.type == MessageType::version) {
      auto version = dynamic_unique_ptr_cast<VersionEcho>(std::move(msg.data));
      if (version) {
        versions_.push_back(*version);
      }
    }
#ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
    else if (msg.type == MessageType::measure_devel_stream) {
      auto stream =
          dynamic_unique_ptr_cast<MeasureDevelStream>(std::move(msg.data));
      if (stream) {
        measure_devel_stream_.emplace_back(std::move(*stream));
      }
    }
    else if (msg.type == MessageType::out_of_range) {
      auto echo = dynamic_unique_ptr_cast<OutOfRangeEcho>(std::move(msg.data));
      if (echo) {
        out_of_range_.push_back(echo->out_of_range);
      }
    }
#endif
#ifdef CLIENT_BL_CUSTOMIZATION
    else if (msg.type == MessageType::distance_echo) {
      auto distance = dynamic_unique_ptr_cast<DistanceEcho>(std::move(msg.data));
      if (distance) {
        if (distance->type == DistanceType::distance_l1) {
          distance_l1_.push_back(*distance);
        } else if (distance->type == DistanceType::distance_l) {
          distance_l_.push_back(*distance);
        }
      }
    }
#endif
  }
}

bool CommandEchoHandler::IsCommandEchoed(const char &id) {
  return echo_map_.count(id);
}

bool CommandEchoHandler::IsCommandSucceeded(const char &id) {
  if (!IsCommandEchoed(id)) {
    return false;
  }
  return echo_map_[id];
}

bool CommandEchoHandler::IsFrequencyEchoed() {
  return !frequencies_.empty();
}

unsigned short CommandEchoHandler::Frequency() {
  if (!IsFrequencyEchoed()) {
    return 0;
  }
  return *frequencies_.rbegin();
}

bool CommandEchoHandler::IsSerialNumberEchoed() {
  return !serial_numbers_.empty();
}

QString CommandEchoHandler::SerialNumber() {
  if (!IsSerialNumberEchoed()) {
    return "";
  }
  if (!serial_numbers_.rbegin()->first) {
    return "";
  }
  return serial_numbers_.rbegin()->second;
}

bool CommandEchoHandler::IsOutputSwitchEchoed() {
  return !output_status_.empty();
}

bool CommandEchoHandler::IsOutputOn() {
  if (!IsOutputSwitchEchoed()) {
    return false;
  }
  return *output_status_.rbegin();
}

bool CommandEchoHandler::IsBaudRateEchoed() {
  return !baud_rates_.empty();
}

int CommandEchoHandler::BaudRate() {
  if (!IsBaudRateEchoed()) {
    return 0;
  }
  return *baud_rates_.rbegin();
}

bool CommandEchoHandler::IsOutputFormatEchoed() {
  return !output_formats_.empty();
}

OutputFormat CommandEchoHandler::GetOutputFormat() {
  return *output_formats_.rbegin();
}

bool CommandEchoHandler::IsFirmwareUpdateEchoed() {
  return !firmware_update_status_.empty();
}

FirmwareUpdateStatus CommandEchoHandler::GetFirmwareUpdateStatus() {
  return *firmware_update_status_.rbegin();
}

bool CommandEchoHandler::IsVersionEchoed() {
  return !versions_.empty();
}

VersionEcho CommandEchoHandler::Version() {
  return *versions_.rbegin();
}

bool CommandEchoHandler::IsMeasureDevelStreamEchoed() {
  return !measure_devel_stream_.empty();
}

MeasureDevelStream CommandEchoHandler::GetMeasureDevelStream() {
  return std::move(*measure_devel_stream_.rbegin());
}

bool CommandEchoHandler::IsDistanceL1Echoed() {
  return !distance_l1_.empty();
}

DistanceEcho CommandEchoHandler::GetDistanceL1() {
  return *distance_l1_.rbegin();
}

bool CommandEchoHandler::IsDistanceLEchoed() {
  return !distance_l_.empty();
}

DistanceEcho CommandEchoHandler::GetDistanceL() {
  return *distance_l_.rbegin();
}

bool CommandEchoHandler::IsRangeDetectEchoed() {
  return !out_of_range_.empty();
}

bool CommandEchoHandler::IsOutOfRange() {
  if (!IsRangeDetectEchoed()) {
    return false;
  }
  return *out_of_range_.rbegin();
}
