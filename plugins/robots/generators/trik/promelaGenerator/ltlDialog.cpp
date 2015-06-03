#include "ltlDialog.h"
#include "ui_ltlDialog.h"

LTLDialog::LTLDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LTLDialog)
{
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);
	ui->setupUi(this);

	connect(ui->runVerifierButton, &QPushButton::pressed, this, &LTLDialog::onRunVerifierButtonPress);
	connect(ui->cancelButton, &QPushButton::pressed, this, &QDialog::hide);
}

LTLDialog::~LTLDialog()
{
	delete ui;
}

void LTLDialog::onRunVerifierButtonPress()
{
	this->hide();
	emit runVerifierButtonPressed(ui->ltlTextEdit->toPlainText());
}
