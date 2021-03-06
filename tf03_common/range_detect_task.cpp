#include "range_detect_task.h"
#include <QDebug>

RangeDetectTask::RangeDetectTask()
{
  threshold_.store(100);
//  threshold_.store(1300);
}

void RangeDetectTask::SetThreshold(const unsigned short &threshold) {
  threshold_.store(threshold);
}

void RangeDetectTask::IncomingMeasure(const MeasureDevel &measure) {
  if (!IsStarted()) {
    return;
  }
  if (!stream_) {
    stream_.reset(new std::list<MeasureDevel>);
  }
  constexpr int kSampleNum = 200;
  if (stream_->size() > kSampleNum) {
    Message message;
    message.type = MessageType::out_of_range;
    std::unique_ptr<OutOfRangeEcho> echo(new OutOfRangeEcho);
    int cnt = 0;
    echo->too_near = false;
    unsigned short threshold = threshold_.load();
    for (auto& measure : *stream_) {
//      if (measure.dist >= 18000 || measure.dist == 0) {
//        ++cnt;
//      }
#ifdef APD_EXPERIMENT_USE_RAWDIST2
      if (measure.raw_dist2 >= 18000 || measure.raw_dist2 <= threshold) {
        ++cnt;
      }
      if (measure.raw_dist2 <= threshold && measure.raw_dist2 > 0) {
        echo->too_near = true;
      }
#else
      if (measure.dist >= 18000 || measure.dist <= threshold) {
        ++cnt;
      }
      if (measure.dist <= threshold && measure.dist > 0) {
        echo->too_near = true;
      }
#endif
    }
    echo->out_of_range = ((cnt * 1.0f / kSampleNum) > 0.8f);
    message.data = std::move(echo);
    EnqueueReceivedMessages(std::move(message));
    stream_.reset(new std::list<MeasureDevel>);
  }
  stream_->push_back(measure);
}
