#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "debugwindow.h"

#include "../includes/Gb.class.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0, Gbmu::Gb *gb = 0);
	~MainWindow();

private slots:
	void _onMenuLoad();

private:
	Ui::MainWindow *_ui;
	DebugWindow *_dw;
	Gbmu::Gb *_gb;
};

#endif // MAINWINDOW_H
