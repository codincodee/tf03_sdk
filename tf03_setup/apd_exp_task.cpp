#include "apd_exp_task.h"
#include "parsed.h"
#include <QDebug>
#include "driver.h"

APDExpTask::APDExpTask()
{

}

void APDExpTask::IncomingMeasure(const MeasureDevel &measure) {
  if (!is_on_) {
    return;
  }
  if (!stream_) {
    stream_.reset(new std::list<MeasureDevel>);
  }
  if (stream_->size() > 100) {
    Message message;
    message.type = MessageType::measure_devel_stream;
    std::unique_ptr<MeasureDevelStream> msg_stream(new MeasureDevelStream);
    msg_stream->stream = std::move(*stream_);
    message.data = std::move(msg_stream);
    EnqueueReceivedMessages(std::move(message));
    stream_.reset(new std::list<MeasureDevel>);
  }
  stream_->push_back(measure);
}

void APDExpTask::Start() {
  is_on_ = true;
}

void APDExpTask::Stop() {
  is_on_ = false;
}
