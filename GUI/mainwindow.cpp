#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow constructor";
    _ui->setupUi(this); // load mainwindow.ui file

     // Forbid header cells resize
    _ui->memory->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->generalRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->disassembler->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->videoRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->otherRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    update(); // start main loop
}

MainWindow::~MainWindow() {
    delete _ui;
}

void MainWindow::update() {
    qDebug() << "update()";
    _updateRegisters();
}

// TODO: Replace every 0xDEAD with the correct _cpu->getXX
void MainWindow::_updateRegisters() {
    qDebug() << "_updateRegisters()";

    // Update general registers
    _ui->generalRegisters->item(MainWindow::REG_PC, 0)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->generalRegisters->item(MainWindow::REG_AF, 0)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->generalRegisters->item(MainWindow::REG_AF, 0)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->generalRegisters->item(MainWindow::REG_BC, 0)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->generalRegisters->item(MainWindow::REG_DE, 0)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->generalRegisters->item(MainWindow::REG_HL, 0)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->generalRegisters->item(MainWindow::REG_SP, 0)->setData(Qt::DisplayRole, "0xDEAD");


    // Update video registers
    _ui->videoRegisters->item(MainWindow::REG_LCDC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_STAT, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_SCY, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_SCX, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_LY, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_LYC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_DMA, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_BGP, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_OBP0, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_OBP1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_WY, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_WX, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_BCPS, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_BCPD, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_OCPS, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(MainWindow::REG_OCPD, 1)->setData(Qt::DisplayRole, "0xDEAD");

    // Update other registers
    _ui->otherRegisters->item(MainWindow::REG_P1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_SB, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_SC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_DIV, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_TIME, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_TMA, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_TAC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_KEY1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_VBK, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_HDMA1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_HDMA2, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_HDMA3, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_HDMA4, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_HDMA5, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_SVSK, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_IF, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(MainWindow::REG_IE, 1)->setData(Qt::DisplayRole, "0xDEAD");
}
