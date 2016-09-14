#include "debugwindow.h"
#include "ui_debugwindow.h"

/**
  * DebugWindow constructor
  * Load and setup UI (forbid resize, number of memory rows, connect signals/slot..)
  */
DebugWindow::DebugWindow(QWidget *parent, Gbmu::Gb *gb) :
	QMainWindow(parent),
	_ui(new Ui::DebugWindow),
	_gb(gb)
{
	_ui->setupUi(this); // load debugwindow.ui file (Forms/debugwindow.ui)
	_ui->memory->setRowCount(GB_MEM_SIZE / 16); // each row is 16 bytes long

	// Forbid header cells resize
#if QT_VERSION >= 0x050000
	_ui->memory->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->generalRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->disassembler->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->videoRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->otherRegisters->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->memory->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->generalRegisters->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->disassembler->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->videoRegisters->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	_ui->otherRegisters->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else // Qt < 5 doesn't have setSectionResizeMode
	_ui->memory->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->generalRegisters->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->disassembler->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->videoRegisters->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->otherRegisters->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->memory->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->generalRegisters->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->disassembler->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->videoRegisters->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_ui->otherRegisters->verticalHeader()->setResizeMode(QHeaderView::Fixed);
#endif

	updateUI(); // update UI once

	// Test signals/slot (button that quits debugger)
	// qApp is a global defined in <QApplication> (pointer to QApplication declared in main)
	connect(_ui->generalRegisters, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(generalRegisterChanged(QTableWidgetItem*)));
}

/**
  * DebugWindow destructor
 */
DebugWindow::~DebugWindow()
{
	delete _ui;
}

/**
  * Calls all UI related update functions (memory, registers, disassembler)
 */
void DebugWindow::updateUI() {
	_updateRegisters();
	_updateMemory();
}

/**
  * Visually update memory table
  * Values are not modified in this function
  */
void DebugWindow::_updateMemory() {
	uint8_t byte;

	for (int i = 0; i < _ui->memory->rowCount(); i++) {
		_ui->memory->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i * 16, 16).rightJustified(4, '0'))); // address on the left
		for (int j = 0; j < 16; j++) {
			byte = _gb->cpu()->memory()->getByteAt(i * 16 + j); // read byte from memory
			_ui->memory->setItem(i, j, new QTableWidgetItem(QString::number(byte, 16))); // display read byte in the UI
		}
	}
}

/**
  * Visually update all register values
  * Values are not modified in this function
  */
void DebugWindow::_updateRegisters() {
	Gbmu::Registers * regs = _gb->cpu()->regs(); // get registers from attached gameboy

	// TODO: Replace every 0xDEAD with the correct _reg->getXX()

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

/**
  * Slot function executed when generalRegister cell changed
  * Reads new value from cell and set the corresponding register (based on the DebugWindow::REG_xx enum)
  * @param[in]  item  The changed cell
  */
void DebugWindow::generalRegisterChanged(QTableWidgetItem *item) {
	std::stringstream ss; // used to get uint16_t from string (the cell contains text)
	uint16_t x; // converted user input

	// TODO: Check if x is a valid value
	ss << std::hex << item->data(Qt::DisplayRole).toString().toStdString(); // get cell data (QVariant) to QString (toString) then to stdstring (toStdString)
	ss >> x; // get stringstream value as uint16_t
	switch (item->row()) { // find which register was changed based on its row index
	case DebugWindow::REG_AF:
		_gb->cpu()->regs()->setAF(x);
		break;
	case DebugWindow::REG_BC:
		_gb->cpu()->regs()->setBC(x);
		break;
	case DebugWindow::REG_DE:
		_gb->cpu()->regs()->setDE(x);
		break;
	case DebugWindow::REG_HL:
		_gb->cpu()->regs()->setHL(x);
		break;
	};
	update(); // update ui (maybe not needed)
}
