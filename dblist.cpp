#include "dblist.h"
#include "adddb.h"
#include "ui_dblist.h"
#include "dbconnect.h"

dbList::dbList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbList)
{
    ui->setupUi(this);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //ui->tableWidget->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    dbList::updateList();

}

dbList::~dbList()
{
    delete ui;
}

void dbList::on_tableWidget_doubleClicked(const QModelIndex &index)
{

    QString database = ui->tableWidget->item(index.row(),0)->data(Qt::DisplayRole).toString();
    QString host = ui->tableWidget->item(index.row(),1)->data(Qt::DisplayRole).toString();
    QString imagePath = ui->tableWidget->item(index.row(),2)->data(Qt::DisplayRole).toString();
    if(createConnection(host, database)){
        qApp->setProperty("ImagePath", imagePath);
        //qDebug() << qApp->property("ImagePath");
    }

}

void dbList::on_addButton_clicked()
{
    addDb addDbDlg(this);
    addDbDlg.exec();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    updateList();
}

void dbList::on_deleteButton_clicked()
{
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;

    int count = ui->tableWidget->selectionModel()->selectedRows().count();
    QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedIndexes();

    QSettings settings("Bdm Soft", "MFarm");
    settings.beginGroup("Databases");

    for (int i = 0; i < count; i++){
        QString database = ui->tableWidget->item(indexes.at(i).row(),0)->data(Qt::DisplayRole).toString();
        settings.remove(database);
    }
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    updateList();
}

void dbList::updateList(){

    QSettings settings("Bdm Soft", "MFarm");
    settings.beginGroup("Databases");

        QStringList dbList = settings.childGroups();

        for(int i=0;i<dbList.count();i++){
            settings.beginGroup(dbList.at(i));
                ui->tableWidget->insertRow(i);
                QTableWidgetItem* nameItem = new QTableWidgetItem();
                QTableWidgetItem* serverItem = new QTableWidgetItem();
                QTableWidgetItem* imageItem = new QTableWidgetItem();
                nameItem->setText(dbList.at(i));
                serverItem->setText(settings.value("Server").toString());
                imageItem->setText(settings.value("Image folder").toString());
                ui->tableWidget->setItem(i,0, nameItem);
                ui->tableWidget->setItem(i,1, serverItem);
                ui->tableWidget->setItem(i,2, imageItem);
                ui->tableWidget->resizeColumnsToContents();
            settings.endGroup();
        }

    settings.endGroup();

}
