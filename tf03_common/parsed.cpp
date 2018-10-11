#include "parsed.h"

ParsedBase::~ParsedBase() {

}

MeasureBasic::~MeasureBasic() {

}

float MeasureDevel::Celsius() const {
  return (temp * 3300 / 4096 - 760) / 2.5 + 25;
}

DistanceEcho::~DistanceEcho() {}
