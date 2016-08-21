#include "debugwindow.h"
#include "ui_debugwindow.h"

DebugWindow::DebugWindow(QWidget *parent, Gbmu::Gb *gb) :
    QMainWindow(parent),
    _ui(new Ui::DebugWindow),
    _gb(gb)
{
    qDebug() << "DebugWindow constructor";

    _ui->setupUi(this); // load debugwindow.ui file

     // Forbid header cells resize
    _ui->memory->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->generalRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->disassembler->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->videoRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _ui->otherRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    update(); // start main loop
}

DebugWindow::~DebugWindow()
{
    delete _ui;
}

void DebugWindow::update() {
    qDebug() << "update()";
    _updateRegisters();
}

// TODO: Replace every 0xDEAD with the correct _reg->getXX()
void DebugWindow::_updateRegisters() {
    Gbmu::Registers * regs = _gb->cpu()->regs();
    qDebug() << "_updateRegisters()";

    // Update general registers
    //_ui->generalRegisters->item(DebugWindow::REG_PC, 0)->setData(Qt::DisplayRole, "0x" + QString::number(regs->getPC(), 16).toUpper());
    _ui->generalRegisters->item(DebugWindow::REG_AF, 0)->setData(Qt::DisplayRole, "0x" + QString::number(regs->getAF(), 16).toUpper());
    _ui->generalRegisters->item(DebugWindow::REG_BC, 0)->setData(Qt::DisplayRole, "0x" + QString::number(regs->getBC(), 16).toUpper());
    _ui->generalRegisters->item(DebugWindow::REG_DE, 0)->setData(Qt::DisplayRole, "0x" + QString::number(regs->getDE(), 16).toUpper());
    _ui->generalRegisters->item(DebugWindow::REG_HL, 0)->setData(Qt::DisplayRole, "0x" + QString::number(regs->getHL(), 16).toUpper());
    //_ui->generalRegisters->item(DebugWindow::REG_SP, 0)->setData(Qt::DisplayRole, "0x" + QString::number(regs->getSP(), 16).toUpper());

    // Update video registers
    _ui->videoRegisters->item(DebugWindow::REG_LCDC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_STAT, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_SCY, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_SCX, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_LY, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_LYC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_DMA, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_BGP, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_OBP0, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_OBP1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_WY, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_WX, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_BCPS, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_BCPD, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_OCPS, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->videoRegisters->item(DebugWindow::REG_OCPD, 1)->setData(Qt::DisplayRole, "0xDEAD");

    // Update other registers
    _ui->otherRegisters->item(DebugWindow::REG_P1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_SB, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_SC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_DIV, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_TIME, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_TMA, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_TAC, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_KEY1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_VBK, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_HDMA1, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_HDMA2, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_HDMA3, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_HDMA4, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_HDMA5, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_SVSK, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_IF, 1)->setData(Qt::DisplayRole, "0xDEAD");
    _ui->otherRegisters->item(DebugWindow::REG_IE, 1)->setData(Qt::DisplayRole, "0xDEAD");
}
