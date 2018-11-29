#include "parsed.h"

ParsedBase::~ParsedBase() {

}

std::unique_ptr<ParsedBase> ParsedBase::Clone() {
  return nullptr;
}

MeasureBasic::~MeasureBasic() {

}

std::unique_ptr<ParsedBase> MeasureBasic::Clone() {
  std::unique_ptr<MeasureBasic> ptr(new MeasureBasic);
  *ptr = *this;
  return ptr;
}

MeasureDevel::~MeasureDevel() {}

std::unique_ptr<ParsedBase> MeasureDevel::Clone() {
  std::unique_ptr<MeasureDevel> ptr(new MeasureDevel);
  *ptr = *this;
  return ptr;
}

float MeasureDevel::Celsius() const {
  return (temp * 3300 / 4096 - 760) / 2.5 + 25;
}

MiniMeasure29B::~MiniMeasure29B() {}

std::unique_ptr<ParsedBase> MiniMeasure29B::Clone() {
  std::unique_ptr<MiniMeasure29B> ptr(new MiniMeasure29B);
  *ptr = *this;
  return ptr;
}

MeasureBasic ToMeasureBasic(const MeasureDevel& devel) {
  MeasureBasic basic;
  basic.amp = devel.amp;
  basic.dist = devel.dist;
  basic.id = devel.id;
  return basic;
}

MeasureDevelStamped::MeasureDevelStamped() {}

MeasureDevelStamped::MeasureDevelStamped(const MeasureDevel &devel)
  : MeasureDevel(devel) {
}

DistanceEcho::~DistanceEcho() {}
