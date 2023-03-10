#ifndef POMODORO_H
#define POMODORO_H

#include <QDialog>
#include <QTimer>

const int workTime = 25 * 60;
const int shortBreak = 5 * 60;
const int longBreak = 10 * 60;

enum States { IDLE, RUNNING, PAUSE };
enum Turns { WORK, SHORTBREAK, LONGBREAK };

QT_BEGIN_NAMESPACE
namespace Ui {
class Pomodoro;
}
QT_END_NAMESPACE

class Pomodoro : public QDialog {
  Q_OBJECT

public:
  Pomodoro(QWidget *parent = nullptr);
  ~Pomodoro();

private slots:
  void updateTime();

  void onStartBtnClicked();
  void onPauseBtnClicked();
  void onResumeBtnClicked();
  void onCancelBtnClicked();

private:
  Ui::Pomodoro *ui;

  int currentTicks = 0;
  int doneTimes = 0;
  Turns turn = WORK;

  QTimer *timer;

  void manager(States);
  QString formatSeconds(int);
  QString enumToString(Turns);
};
#endif // POMODORO_H
