// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TOOLBARFRAME_H
#define TOOLBARFRAME_H

#include <dtkwidget_global.h>

#include <QFrame>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QPushButton;
class QSlider;
class QLabel;
class QMediaPlayer;
class QTimer;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE
class DSlider;
DWIDGET_END_NAMESPACE

class ToolBarFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ToolBarFrame(const QString &uri, QWidget *parent = nullptr);

private:
    void initUI();
    void initConnections();

signals:

public slots:
    void onPlayStateChanged(const QMediaPlayer::State &state);
    void onPlayStatusChanged(const QMediaPlayer::MediaStatus &status);
    void onPlayDurationChanged(qint64 duration);
    void onPlayControlButtonClicked();
    void updateProgress();
    void seekPosition(const int &pos);
    void play();
    void pause();
    void stop();

private:
    void durationToLabel(qint64 duration);

private:
    QMediaPlayer *m_player;
    QPushButton *m_playControlButton;
    DTK_WIDGET_NAMESPACE::DSlider *m_progressSlider;
    QLabel *m_durationLabel;
    QTimer *m_updateProgressTimer;
};

#endif // TOOLBARFRAME_H
