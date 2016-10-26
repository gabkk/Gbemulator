#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Gbmu::Gb *gb) :
	QMainWindow(parent),
	_ui(new Ui::MainWindow),
	_dw(new DebugWindow(parent, gb)),
	_gb(gb)
{
	_ui->setupUi(this); // load mainwindow.ui (Forms/mainwindow.ui)

	connect(_ui->actionLoad, SIGNAL(triggered()), this, SLOT(_onMenuLoad())); // File->Load action
	connect(_ui->actionDebugger, SIGNAL(triggered()), _dw, SLOT(show())); // Game->Debugger action
}

MainWindow::~MainWindow()
{
	delete _ui;
}

/**
 * Slot function called when File->Load is triggered
 * Loads a cartridge
 */
void MainWindow::_onMenuLoad()
{
	QString path = QFileDialog::getOpenFileName(this, "Open file", "/home");

	_gb->load(path.toStdString());
	_dw->updateUI();
}
