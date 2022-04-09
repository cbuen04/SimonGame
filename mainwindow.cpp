#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

/**
 * @brief this is the cpp file that creates the window and sets up the ui
 * graphics. connects buttons and communicates through slots and signals
 * to update information in the model
 *
 * @authors Emma Kerr, Charly Bueno
 */

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initializing the starting state of the game ui
    ui->pinkButton->setStyleSheet( QString("QPushButton {background-color: rgb(255, 0, 255);}"));
    ui->purpleButton->setStyleSheet( QString("QPushButton {background-color: rgb(127, 0, 255);}"));
    ui->pinkButton->setDisabled(true);
    ui->purpleButton->setDisabled(true);
    ui->progressBar->reset();
    ui->youLoseLabel->setVisible(false);
    ui->tryAgainLabel->setVisible(false);

    //connecting all of the buttons from model logic
    connect(ui->pinkButton,
            &QPushButton::clicked,
            &model,
            &Model::pinkButtonPressed);
    connect(ui->purpleButton,
            &QPushButton::clicked,
            &model,
            &Model::purpleButtonPressed);
    connect(ui->startButton,
            &QPushButton::clicked,
            &model,
            &Model::startButtonPressed);
    connect(ui->sickoModeButton,
            &QPushButton::clicked,
            &model,
            &Model::sickoModeSelected);

    // connecting player moves with the progress bar
    connect(&model,
            &Model::progressLevel,
            ui->progressBar,
            &QProgressBar::setValue);
    connect(&model,
            &Model::newSequenceLength,
            ui->progressBar,
            &QProgressBar::setMaximum);


    // connects to disable start button when pressed
    connect(&model,
            &Model::disableStart,
            ui->startButton,
            &QPushButton::setDisabled);
    connect(&model,
            &Model::disableSickoMode,
            ui->sickoModeButton,
            &QPushButton::setDisabled);

    // connects to disable and enable buttons when the sequence is being displayed
    connect(&model,
            &Model::disableColorButtons,
            ui->pinkButton,
            &QPushButton::setDisabled);
    connect(&model,
            &Model::disableColorButtons,
            ui->purpleButton,
            &QPushButton::setDisabled);

   // connects glow commands to original game buttons
    connect(&model,
            &Model::pinkButtonGlow,
            ui->pinkButton,
            &QPushButton::setStyleSheet);
    connect(&model,
            &Model::purpleButtonGlow,
            ui->purpleButton,
            &QPushButton::setStyleSheet);

    // connects the game over labels on the ui to the losing sequence
    connect(&model,
            &Model::gameOverLabel,
            ui->youLoseLabel,
            &QLabel::setVisible);
    connect(&model,
            &Model::gameOverLabel,
            ui->tryAgainLabel,
            &QLabel::setVisible);
}

MainWindow::~MainWindow()
{
    delete ui;
}



