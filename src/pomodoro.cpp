#include "pomodoro.h"
#include "ui_pomodoro.h"

Pomodoro::Pomodoro(QWidget *parent) : QDialog(parent), ui(new Ui::Pomodoro) {
  ui->setupUi(this);
  ui->cntLbl->setText("Done: " + QString::number(doneTimes));
  setFixedSize(500, 300);

  connect(ui->startBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
  connect(ui->pauseBtn, SIGNAL(clicked()), this, SLOT(onPauseBtnClicked()));
  connect(ui->resumeBtn, SIGNAL(clicked()), this, SLOT(onResumeBtnClicked()));
  connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));

  manager(IDLE);
}

Pomodoro::~Pomodoro() { delete ui; }

void Pomodoro::onStartBtnClicked() { manager(RUNNING); }

void Pomodoro::onPauseBtnClicked() { manager(PAUSE); }

void Pomodoro::onResumeBtnClicked() { manager(RUNNING); }

void Pomodoro::onCancelBtnClicked() { manager(IDLE); }

QString Pomodoro::formatSeconds(int secs) {
  return QString(tr("%1:%2")
                     .arg(secs / 60, 2, 10, QChar('0'))
                     .arg(secs % 60, 2, 10, QChar('0')));
}

QString Pomodoro::enumToString(Turns turn) {
  if (turn == WORK) {
    return QString("<font color='red'>Work</font>");
  } else if (turn == SHORTBREAK) {
    return QString("<font color='green'>Short Break</font>");
  } else {
    return QString("<font color='green'>Long Break</font>");
  }
}

void Pomodoro::updateTime() {
  currentTicks--;
  ui->timerLbl->setText(formatSeconds(currentTicks));

  if (currentTicks <= 0) {
    if (turn == WORK) {
      doneTimes++;
      ui->cntLbl->setText("Done: " + QString::number(doneTimes));
    }

    if (turn == SHORTBREAK || turn == LONGBREAK) {
      turn = WORK;
    } else {
      if (doneTimes % 4 == 0) {
        turn = LONGBREAK;
      } else {
        turn = SHORTBREAK;
      }
    }

    manager(IDLE);
  }
}

void Pomodoro::manager(States state) {
  switch (state) {
  case IDLE:
    if (turn == WORK) {
      currentTicks = workTime;
    } else if (turn == SHORTBREAK) {
      currentTicks = shortBreak;
    } else {
      currentTicks = longBreak;
    }

    timer->stop();

    ui->curMode->setText(enumToString(turn));
    ui->timerLbl->setText(formatSeconds(currentTicks));
    ui->startBtn->show();
    ui->pauseBtn->hide();
    ui->resumeBtn->hide();
    ui->cancelBtn->hide();

    break;
  case RUNNING:
    timer->start(1000);

    ui->pauseBtn->show();
    ui->cancelBtn->show();
    ui->startBtn->hide();
    ui->resumeBtn->hide();

    break;
  case PAUSE:
    timer->stop();

    ui->resumeBtn->show();
    ui->cancelBtn->show();
    ui->startBtn->hide();
    ui->pauseBtn->hide();

    break;
  }
}
