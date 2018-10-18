#ifndef CONNECTION_PAGE_H
#define CONNECTION_PAGE_H

#include "export.h"
#include "page_base.h"
#include <QElapsedTimer>
#include <QStringList>

#define SHOW_PROTOCOL_WIDGETS_ON_CONNECTION_PAGE

class Qlabel;
class QComboBox;
class QPushButton;
class QString;
class Driver;
class QGridLayout;
class RequestVersionWidgets;
class CommandEchoHandler;
#ifdef SHOW_PROTOCOL_WIDGETS_ON_CONNECTION_PAGE
class SetProtocolWidgets;
#endif

class API ConnectionPage : public PageBase
{
  Q_OBJECT
public:
  ConnectionPage();
  bool Initialize() override;
  void SetDistanceDisplayLabel(QLabel* label);
  void SetFrequencyDisplayLabel(QLabel* label);
  void SetPortComboBox(QComboBox* combo);
  void SetBaudRateComboxBox(QComboBox* combo);
  void SetConnectPushButton(QPushButton* button);
  void SetVersionLayout(QGridLayout* layout);
  void SetDriver(std::shared_ptr<Driver> driver);
  void Update() override;
  void OnMeasured(const MeasureBasic& measure) override;
public slots:
  void OnConnectButtonClicked();
private:
  void UpdatePortNameComboBox();

  QLabel* distance_display_label_ = nullptr;
  QLabel* frequency_display_label_ = nullptr;
  QComboBox* port_combo_ = nullptr;
  QComboBox* baud_rate_ = nullptr;
  QPushButton* connect_button_ = nullptr;
  QGridLayout* version_layout_ = nullptr;

  QString kDisconnectPushButtonText;
  QString kConnectPushButtonText;

  std::shared_ptr<Driver> driver_;
  std::shared_ptr<RequestVersionWidgets> version_widgets_;

#ifdef SHOW_PROTOCOL_WIDGETS_ON_CONNECTION_PAGE
  std::shared_ptr<SetProtocolWidgets> protocol_widgets_;
#endif
  QElapsedTimer frequency_timer_;
  unsigned long last_freq_measure_id_ = 0;

  QStringList port_list_;
};

#endif // CONNECTION_PAGE_H
