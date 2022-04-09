#include "model.h"
#include <QTimer>
#include <QDebug>
#include <QEventLoop>
/**
 * @brief This holds all the backend data to make the game and progress the levels
 * @authors Emma Kerr, Charly Bueno
 */

Model::Model(QObject *parent)
    : QObject{parent}
{
    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
            &loop, SLOT(quit()));

    connect(timer, SIGNAL(timeout()),
          this, SLOT(revertGlow()));


    glowTime = 1500;
}

/**
 * @brief Model::startButtonPressed
 *  This is the basic level logic which starts the sequence at one
 */
void Model::startButtonPressed(){
    emit disableStart(true);
    emit gameOverLabel(false);
    emit disableSickoMode(true);
    progressLevelSequence();
}

/**
 * @brief Model::sickoModeSelected
 * This is the extra feature which starts the game sequence length at 10 and shortens the glow time
 */
void Model::sickoModeSelected(){
    emit disableStart(true);
    emit gameOverLabel(false);
    emit disableSickoMode(true);

    for(int i = 0; i < 9; i++){ // this creates the sequence of 9 which will be increased to 10 in the call
                                // to progressLevelSequence()
        correctSequence.push_back(arc4random() % 2);
    }
    glowTime = 40;
    progressLevelSequence();
}

/**
 * @brief Model::pinkButtonPressed
 * This is the slot method which is kicked off when the pink button is pressed
 */
void Model::pinkButtonPressed(){ // pink corresponds to 0
    userInputCount++;
    emit progressLevel(userInputCount);

    // this checks for correctness at each button press, not just correctness of entire sequence at end
    if(correctSequence.at(userInputCount - 1) != 0){
        gameover();
        return;
    }
    if(userInputCount == correctSequence.size()){
        progressLevelSequence();
    }
}

/**
 * @brief Model::purpleButtonPressed
 * This is the slot kicked off when the purple button is pressed
 */
void Model::purpleButtonPressed(){ // purple corresponds to 1
    userInputCount++;
    emit progressLevel(userInputCount);
    if(correctSequence.at(userInputCount - 1) != 1){
        gameover();
        return;
    }
    if(userInputCount == correctSequence.size()){
        progressLevelSequence();
    }
}
/**
 * @brief Model::gameover
 * This is the helper method which restarts the game
 */
void Model::gameover(){
    userInputCount = 0;
    correctSequence.clear();
    emit progressLevel(0);
    emit newSequenceLength(0);

    emit disableStart(false);
    emit disableColorButtons(true);
    emit gameOverLabel(true);
    emit disableSickoMode(false);
    glowTime = 1500;

}

/**
 * @brief Model::progressLevelSequence
 * This is the helper method which progressed the level by displaying the appended color sequence
 */
void Model::progressLevelSequence(){
   timer->start(1000);
   timer->setSingleShot(true);
   loop.exec();

   emit newSequenceLength(0);

   userInputCount = 0;
   correctSequence.push_back(arc4random() % 2);

   // disable buttons while simon is displayed
   emit disableColorButtons(true);

   // set new maximum for this level in the progress bar
   emit newSequenceLength(correctSequence.size());

   for(unsigned long i = 0; i < correctSequence.size(); i++){ // emit the correct sequence to user
       if(correctSequence.at(i) == 0){
           timer->start(200);
           timer->setSingleShot(true);
           loop.exec();
           emit pinkButtonGlow(QString("QPushButton {background-color: rgb(255, 153, 255);}"));
       }
       else{
           timer->start(200);
           timer->setSingleShot(true);
           loop.exec();
           emit purpleButtonGlow(QString("QPushButton {background-color: rgb(204, 153, 255);}"));
       }
       timer->start(glowTime);
       timer->setSingleShot(true);
       loop.exec();
   }
   speedGlowTime(); // speeding up the glow time every round
   // enable buttons after sequence has been displayed
   emit disableColorButtons(false);
}

/**
 * @brief Model::revertGlow
 * This is the method called by the timer to disable the color glowing and display the old color
 */
void Model::revertGlow(){
    //reverts pink glow (darker)
    emit pinkButtonGlow(QString("QPushButton {background-color: rgb(255, 0, 255);}"));

    //reverts purple glow (darker)
    emit purpleButtonGlow(QString("QPushButton {background-color: rgb(127, 0, 255);}"));
}

/**
 * @brief Model::speedGlowTime
 * Helper method to speed up how long the button glows for. It is decreased every round
 */
void Model::speedGlowTime(){
    glowTime *= .9;
    if(glowTime <= 20){
        glowTime = 20;
    }
}


