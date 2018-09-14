#include "measure_manifest.h"

MeasureManifest::MeasureManifest()
{

}

MeasureManifest::~MeasureManifest() {
}

bool MeasureManifest::Initialize() {
  PlotBase::Initialize();
  return true;
}

void MeasureManifest::AddWidget(QWidget *widget) {
  widgets_.push_back(widget);
}
