#include "proposalsdialog.h"
#include "ui_proposalsdialog.h"

#include "db.h"
#include "main.h"
#include "walletmodel.h"
#include <QStandardItemModel>


ProposalsDialog::ProposalsDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ProposalsDialog)
{
    ui->setupUi(this);
}

ProposalsDialog::~ProposalsDialog()
{
    delete ui;
}

void ProposalsDialog::SetWalletModel(WalletModel *model)
{
    this->walletModel = model;
    UpdateTable();
}

void ProposalsDialog::UpdateTable()
{
    int columns = 8;
    int rows = mapProposals.size();
    QStandardItemModel* model = new QStandardItemModel(rows, columns, this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("TxID")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("ID")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Abstract")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Location")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString("Start Block")));
    model->setHorizontalHeaderItem(6, new QStandardItem(QString("End Block")));
    model->setHorizontalHeaderItem(7, new QStandardItem(QString("Bits")));

    CVoteDB voteDB("r");
    int i = 0;
    for (auto it : mapProposals) {
        CVoteProposal proposal;
        if (voteDB.ReadProposal(it.first, proposal)) {
            QList<QStandardItem*> listItems;
            listItems.push_back(new QStandardItem(QString::fromStdString(it.first.GetHex())));
            listItems.push_back(new QStandardItem(QString::fromStdString(proposal.GetHash().GetHex())));
            listItems.push_back(new QStandardItem(QString::fromStdString(proposal.GetName())));
            listItems.push_back(new QStandardItem(QString::fromStdString(proposal.GetDescription())));
            listItems.push_back(new QStandardItem(QString::number(proposal.GetShift())));
            listItems.push_back(new QStandardItem(QString::number(proposal.GetStartHeight())));
            listItems.push_back(new QStandardItem(QString::number(proposal.GetStartHeight() - proposal.GetCheckSpan())));
            listItems.push_back(new QStandardItem(QString::number(proposal.GetBitCount())));
            model->insertRow(i, listItems);
            i++;
        }
    }
    ui->tableView->setModel(model);
}

void ProposalsDialog::on_checkBox_Active_stateChanged(int arg1)
{
    UpdateTable();
}

void ProposalsDialog::on_checkBox_Upcoming_stateChanged(int arg1)
{
    UpdateTable();
}

void ProposalsDialog::on_checkBox_Finished_stateChanged(int arg1)
{
    UpdateTable();
}
