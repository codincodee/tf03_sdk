#include "connection_page.h"
#include <QPushButton>
#include "driver.h"
#include <QComboBox>
#include <QLabel>
#include <QDebug>
#include "command_echo_widgets.h"
#include <QLayout>

ConnectionPage::ConnectionPage()
  : kDisconnectPushButtonText("断开"),
    kConnectPushButtonText("连接")
{

}

bool ConnectionPage::Initialize() {
  if (!distance_display_label_) {
    return false;
  }
  distance_display_label_->setFont(GetLargeBoldFont());
  if (!frequency_display_label_) {
    return false;
  }
  frequency_display_label_->setFont(GetLargeBoldFont());
  if (!port_combo_) {
    return false;
  }
  if (!baud_rate_) {
    return false;
  }
  if (!connect_button_) {
    return false;
  }
  if (!version_layout_) {
    return false;
  }
  if (!driver_) {
    return false;
  }
  if (!EchoHandler()) {
    return false;
  }
  connect(
      connect_button_, SIGNAL(clicked()),
      this, SLOT(OnConnectButtonClicked()));
  frequency_timer_.start();
  connect_button_->setText(kConnectPushButtonText);

  auto baud_rates = Driver::BaudRates();
  for (auto& rate : baud_rates) {
    baud_rate_->addItem(QString::number(rate));
  }

  version_widgets_.reset(new RequestVersionWidgets);
  version_widgets_->driver = driver_;
  version_widgets_->echo_handler = EchoHandler();
  version_widgets_->button->setText("获取版本号");
  version_layout_->addWidget(version_widgets_->button, 0, 0);
  version_layout_->addWidget(version_widgets_->status, 0, 1);
  version_layout_->addWidget(version_widgets_->label, 0, 2);
  return true;
}

void ConnectionPage::SetDistanceDisplayLabel(QLabel *label) {
  distance_display_label_ = label;
}

void ConnectionPage::SetFrequencyDisplayLabel(QLabel *label) {
  frequency_display_label_ = label;
}

void ConnectionPage::SetPortComboBox(QComboBox *combo) {
  port_combo_ = combo;
}

void ConnectionPage::SetBaudRateComboxBox(QComboBox *combo) {
  baud_rate_ = combo;
}

void ConnectionPage::SetConnectPushButton(QPushButton *button) {
  connect_button_ = button;
}

void ConnectionPage::SetVersionLayout(QGridLayout *layout) {
  version_layout_ = layout;
}

void ConnectionPage::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void ConnectionPage::Update() {
  UpdatePortNameComboBox();

  if (driver_->DetectAndAutoConnect()) {
    connect_button_->setText(kDisconnectPushButtonText);
  }

  if (frequency_timer_.elapsed() > 2000) {
    frequency_display_label_->clear();
    distance_display_label_->clear();
    version_widgets_->status->clear();
    version_widgets_->label->clear();
  }

  if (version_widgets_) {
    version_widgets_->Update();
    if (lingual_equal(version_widgets_->status->text(), version_widgets_->kSuccessLingual)) {
      version_widgets_->status->setVisible(false);
    } else {
      version_widgets_->status->setVisible(true);
    }
  }
}

void ConnectionPage::OnMeasured(const MeasureBasic &measure) {
  auto elapse = frequency_timer_.elapsed();

  distance_display_label_->setText(QString::number(measure.dist));
  if (elapse > 1000) {
    auto id_diff = measure.id - last_freq_measure_id_;
    float frequency = id_diff / (elapse / 1000.0f);
    frequency_display_label_->setText(QString::number(std::ceil(frequency)));
    last_freq_measure_id_ = measure.id;
    frequency_timer_.restart();
  }
}

void ConnectionPage::OnConnectButtonClicked() {
  if (connect_button_->text() == kDisconnectPushButtonText) {
    driver_->Close();
    connect_button_->setText(kConnectPushButtonText);
    return;
  }
  driver_->SetPortName(port_combo_->currentText());
  bool ok;
  driver_->SetBaudRate(baud_rate_->currentText().toInt(&ok));
  if (!ok) {
    return;
  }
  driver_->Open();
  connect_button_->setText(kDisconnectPushButtonText);
}

void ConnectionPage::UpdatePortNameComboBox() {
  auto ports = QSerialPortInfo::availablePorts();
  auto update = [this, ports](){
    port_combo_->clear();
    for (auto& port : ports) {
      port_combo_->addItem(port.portName());
    }
    port_list_.clear();
    for (auto& port : ports) {
      port_list_.push_back(port.portName());
    }
  };
  if (ports.size() != port_list_.size()) {
    update();
    return;
  }
  bool should_update = false;
  for (auto& port : ports) {
    if (!port_list_.contains(port.portName())) {
      should_update = true;
    }
  }
  if (should_update) {
    update();
  }
}
