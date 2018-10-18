#include "parsed.h"

ParsedBase::~ParsedBase() {

}

MeasureBasic::~MeasureBasic() {

}

MeasureDevel::~MeasureDevel() {}

float MeasureDevel::Celsius() const {
  return (temp * 3300 / 4096 - 760) / 2.5 + 25;
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
