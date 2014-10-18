#include "calcdialog.h"
#include "ui_calcdialog.h"

#include "clientmodel.h"
#include "notificator.h"
#include "version.h"

#include "wallet.h"
#include "walletdb.h"
#include "main.h"
#include "init.h"

#include <QString>

calcDialog::calcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calcDialog)
{
    ui->setupUi(this);
	
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
}

calcDialog::~calcDialog()
{
    delete ui;
}

void calcDialog::setModel(ClientModel *model)
{

}


void calcDialog::pushButtonClicked()
{
	QString strUserSize = ui->blockSizeEdit->text();
	double dUserBlock = strUserSize.toDouble();
	double dMax = 1000;
	double dReward9 = qMin(dUserBlock * 7.5 / 365 * 9, dMax);
	double dReward12 = qMin(dUserBlock * 7.5 / 365 * 12, dMax);
	double dReward15 = qMin(dUserBlock * 7.5 / 365 * 15, dMax);
	double dReward20 = qMin(dUserBlock * 7.5 / 365 * 20, dMax);
	double dReward30 = qMin(dUserBlock * 7.5 / 365 * 30, dMax);
	
	
	ui->day9Result->setText(QString::number(dReward9));
	ui->day12Result->setText(QString::number(dReward12));
	ui->day15Result->setText(QString::number(dReward15));
	ui->day20Result->setText(QString::number(dReward20));
	ui->day30Result->setText(QString::number(dReward30));
}

void calcDialog::on_buttonBox_accepted()
{
	close();
}
