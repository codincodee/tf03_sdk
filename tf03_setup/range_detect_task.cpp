#include "range_detect_task.h"
#include <QDebug>

RangeDetectTask::RangeDetectTask()
{
}

void RangeDetectTask::IncomingMeasure(const MeasureDevel &measure) {
  if (!IsStarted()) {
    return;
  }
  if (!stream_) {
    stream_.reset(new std::list<MeasureDevel>);
  }
  constexpr int kSampleNum = 100;
  if (stream_->size() > kSampleNum) {
    Message message;
    message.type = MessageType::out_of_range;
    std::unique_ptr<OutOfRangeEcho> echo(new OutOfRangeEcho);
    int cnt = 0;
    for (auto& measure : *stream_) {
      if (measure.dist >= 18000) {
        ++cnt;
      }
    }
    auto out_of_range = echo->out_of_range = ((cnt * 1.0f / kSampleNum) > 0.5f);
    message.data = std::move(echo);
    if (out_of_range) {
      EnqueueReceivedMessages(std::move(message));
    }
    stream_.reset(new std::list<MeasureDevel>);
  }
  stream_->push_back(measure);
}
