#include "measure_manifest.h"
#include <QLabel>
#include <QGridLayout>
#include "parsed.h"
#include "utils.h"
#include <QDebug>

ManifestBase::~ManifestBase() {}

void ManifestBase::SetFormat() {
  for (auto& label : labels) {
    QFont font = label.first->font();
    font.setPointSize(8);
    font.setBold(true);
    label.first->setFont(font);
    label.second = new QLabel;
    label.second->setAlignment(Qt::AlignCenter);
  }
}

void ManifestBase::Clear() {
  for (auto& widget : labels) {
    delete widget.first;
    delete widget.second;
  }
  labels.clear();
}

void ManifestBase::SetVisible(const bool &visible) {
  for (auto& w : labels) {
    w.first->setVisible(visible);
    w.second->setVisible(visible);
  }
}

ManifestDevel::~ManifestDevel() {
  Clear();
}


void ManifestDevel::Setup(QGridLayout *layout) {
  if (!layout) {
    return;
  }

  ClearLayout(layout);
  labels.clear();

  QLabel* label;

  label = new QLabel;
  label->setText("Dist 1 (m)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("Dist 2 (m)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("Dist 3 (m)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("Raw Dist 1 (m)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("Raw Dist 2 (m)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("Raw Dist 3 (m)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("APD (V)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("Temp (C)");
  labels.push_back({label, nullptr});

  label = new QLabel;
  label->setText("Volt (m)");
  labels.push_back({label, nullptr});

  SetFormat();

  for (int i = 0; i < 6; ++i) {
    layout->addWidget(labels[i].first, 0, i);
    layout->addWidget(labels[i].second, 1, i);
  }

  for (int i = 6; i < 9; ++i) {
    layout->addWidget(labels[i].first, 2, i - 6);
    layout->addWidget(labels[i].second, 3, i - 6);
  }
}

void ManifestDevel::IncomingMeasure(const MeasureDevel &measure) {
  labels[0].second->setText(QString::number(measure.dist1 / 100.0, 'f', 2));
  labels[1].second->setText(QString::number(measure.dist2 / 100.0, 'f', 2));
  labels[2].second->setText(QString::number(measure.dist3 / 100.0, 'f', 2));
  labels[3].second->setText(QString::number(measure.raw_dist1 / 100.0, 'f', 2));
  labels[4].second->setText(QString::number(measure.raw_dist2 / 100.0, 'f', 2));
  labels[5].second->setText(QString::number(measure.raw_dist3 / 100.0, 'f', 2));
  labels[6].second->setText(QString::number(measure.apd));
  labels[7].second->setText(QString::number(measure.Celsius()));
  labels[8].second->setText(QString::number(measure.volt));
}

void ManifestBasic::Setup(QGridLayout *layout) {
  if (!layout) {
    return;
  }

  ClearLayout(layout);
  labels.clear();
}

MeasureManifest::MeasureManifest()
{

}

MeasureManifest::~MeasureManifest() {
}

bool MeasureManifest::Initialize() {
  if (!manifest_grid_) {
    return false;
  }

  current_manifest_ = manifest_devel_;
  PlotBase::Initialize();
  return true;
}

void MeasureManifest::AddWidget(QWidget *widget) {
  widgets_.push_back(widget);
}

void MeasureManifest::IncomingMeasure(const MeasureDevel &measure) {
  PlotBase::IncomingMeasure(measure);
  if (!manifest_devel_) {
    manifest_devel_.reset(new ManifestDevel);
    manifest_devel_->Setup(manifest_grid_);
  }
  if (current_manifest_ != manifest_devel_) {
    manifest_devel_->Setup(manifest_grid_);
    current_manifest_ = manifest_devel_;
  }
  manifest_devel_->IncomingMeasure(measure);
}

void MeasureManifest::IncomingMeasure(const MeasureBasic &measure) {
  PlotBase::IncomingMeasure(measure);
  if (!manifest_basic_) {
    manifest_basic_.reset(new ManifestBasic);
    manifest_basic_->Setup(manifest_grid_);
  }
  if (current_manifest_ != manifest_basic_) {
    manifest_basic_->Setup(manifest_grid_);
    current_manifest_ = manifest_basic_;
  }
}

void MeasureManifest::SetManifestGrid(QGridLayout *layout) {
  manifest_grid_ = layout;
}

void MeasureManifest::SetVisible(const bool &visible) {
  PlotBase::SetVisible(visible);
  if (current_manifest_) {
    current_manifest_->SetVisible(visible);
  }
}
