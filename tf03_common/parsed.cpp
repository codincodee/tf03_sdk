#include "parsed.h"

ParsedBase::~ParsedBase() {

}

MeasureBasic::~MeasureBasic() {

}

MeasureDevel::~MeasureDevel() {}

float MeasureDevel::Celsius() const {
  return (temp * 3300 / 4096 - 760) / 2.5 + 25;
}

MeasureDevelStamped::MeasureDevelStamped() {}

MeasureDevelStamped::MeasureDevelStamped(const MeasureDevel &devel)
  : MeasureDevel(devel) {
}

DistanceEcho::~DistanceEcho() {}
