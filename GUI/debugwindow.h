#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include "../includes/Gb.class.hpp"
#include "../includes/Registers.class.hpp"
#include "../includes/Cpu.class.hpp"

#include <sstream>

class QTableWidgetItem;
namespace Ui {
class DebugWindow;
}

class DebugWindow : public QMainWindow
{
	Q_OBJECT

	// TODO: Move to Registers
	enum generalRegisters {
		REG_PC,
		REG_AF,
		REG_BC,
		REG_DE,
		REG_HL,
		REG_SP
	};

	enum videoRegisters {
		REG_LCDC,
		REG_STAT,
		REG_SCY,
		REG_SCX,
		REG_LY,
		REG_LYC,
		REG_DMA,
		REG_BGP,
		REG_OBP0,
		REG_OBP1,
		REG_WY,
		REG_WX,
		REG_BCPS,
		REG_BCPD,
		REG_OCPS,
		REG_OCPD
	};

	enum otherRegisters {
		REG_P1,
		REG_SB,
		REG_SC,
		REG_DIV,
		REG_TIME,
		REG_TMA,
		REG_TAC,
		REG_KEY1,
		REG_VBK,
		REG_HDMA1,
		REG_HDMA2,
		REG_HDMA3,
		REG_HDMA4,
		REG_HDMA5,
		REG_SVSK,
		REG_IF,
		REG_IE
	};

public:
	explicit DebugWindow(QWidget *parent = 0, Gbmu::Gb *gb = 0);
	void updateUI();
	~DebugWindow();

public slots:
	void generalRegisterChanged(QTableWidgetItem* item);

private:
	Ui::DebugWindow *_ui;
	Gbmu::Gb *_gb;

	void _updateRegisters();
	void _updateMemory();
};

#endif // DEBUGWINDOW_H
