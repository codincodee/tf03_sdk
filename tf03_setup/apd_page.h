#ifndef APD_PAGE_H
#define APD_PAGE_H

#include <QObject>

struct MeasureDevel;
// #include <QLayout>
class QLayout;
class DistanceOverTimeChart;
namespace QtCharts {
  class QChartView;
}
class QLabel;
class QPushButton;
class QProgressBar;
class QLineEdit;

class APDPage : public QObject
{
  Q_OBJECT
public:
  APDPage();
  ~APDPage();
  void SetPlotLayout(QLayout* layout);
  void SetAPDDisplayLabel(QLabel* label);
  void SetTemperatureDisplayLabel(QLabel* label);
  void SetStartPushButton(QPushButton* button);
  void SetProgressBar(QProgressBar* progress);
  void SetAPDFromLineEdit(QLineEdit* edit);
  void SetAPDToLineEdit(QLineEdit* edit);
  void SetThresholdLineEdit(QLineEdit* edit);
  bool Initialize();
  void IncomingMeasure(const MeasureDevel& measure);
  void Update();
public slots:
  void OnStartButtonClicked();
private:
  QLayout* plot_layout_ = nullptr;
  QtCharts::QChartView* main_chart_view_ = nullptr;
  DistanceOverTimeChart* main_chart_ = nullptr;
  QLabel* apd_label_ = nullptr;
  QLabel* temp_label_ = nullptr;
  QPushButton* start_button_ = nullptr;
  QProgressBar* progress_bar_ = nullptr;
  QLineEdit* apd_from_edit_ = nullptr;
  QLineEdit* apd_to_edit_ = nullptr;
  QLineEdit* threshold_edit_ = nullptr;
  const QString kStartButtonStart = "Start";
  const QString kStartButtonStop = "Stop";
  int apd_from_;
  int apd_to_;
  int threshold_;
};

#endif // APD_PAGE_H
