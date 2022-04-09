#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include <iostream>
#include <QTimer>
#include <QEventLoop>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    void pinkButtonPressed();
    void purpleButtonPressed();
    QTimer *timer;
    std::vector<int> correctSequence;

signals:
    void disableStart(bool);
    void disableColorButtons(bool);
    void enableColorButtons(bool);
    void pinkButtonGlow(const QString);
    void purpleButtonGlow(const QString);
    void progressLevel(int);
    void newSequenceLength(int);
    void gameOverLabel(bool);
    void disableSickoMode(bool);


public slots:
    void startButtonPressed();
    void revertGlow();
    void sickoModeSelected();

private:
    unsigned long userInputCount;
    void progressLevelSequence();
    void gameover();
    QEventLoop loop;
    int glowTime;
    void speedGlowTime();
};
// do we need to change colors when button pressed???
#endif // MODEL_H
