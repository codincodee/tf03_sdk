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

QString MiniMeasure29B::Manifest() {
  QString s;
  s =
      "Raw Dist: " + QString::number(raw_dist) + " "
      "Temp: " + QString::number(temp) + " "
      "Amb: " + QString::number(amb) + " "
      "Temp Dist: " + QString::number(temp_d) + " "
      "Amb Dist: " + QString::number(amb_d) + " "
      "Ref Dist: " + QString::number(ref_d) + " "
      "Corr Dist: " + QString::number(cor_d) + " "
      "Table Dist: " + QString::number(table_d) + " "
      "Temp Amp: " + QString::number(temp_amp) + " "
      "Amb Amp: " + QString::number(amb_amp) + " "
      "DCS3: " + QString::number(dcs3) + " "
      "Int-time: " + QString::number(short(inttime));
  return s;
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
