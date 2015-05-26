#include "ltlDialog.h"
#include "ui_ltlDialog.h"

LTLDialog::LTLDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LTLDialog)
{
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);
	ui->setupUi(this);

	connect(ui->okButton, &QPushButton::pressed, this, &LTLDialog::onOkButtonPress);
	connect(ui->cancelButton, &QPushButton::pressed, this, &QDialog::hide);
}

LTLDialog::~LTLDialog()
{
	delete ui;
}

void LTLDialog::onOkButtonPress()
{
	emit okButtonPressed(ui->ltlTextEdit->toPlainText());
	this->hide();
}
