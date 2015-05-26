#pragma once

#include <QtWidgets/QDialog>

namespace Ui {
class LTLDialog;
}

class LTLDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LTLDialog(QWidget *parent = 0);
	~LTLDialog();

signals:
	void okButtonPressed(QString const &formula);

private:
	void onOkButtonPress();

	Ui::LTLDialog *ui;
};
