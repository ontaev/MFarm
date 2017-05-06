#include "calving.h"
#include "ui_calving.h"
#include "mainwindow.h"

#include <QtSql>
#include <QtGui>

Calving::Calving(QWidget *parent, int insem_id) :
    QDialog(parent),
    ui(new Ui::Calving)
{
    ui->setupUi(this);
    Calving::insem_id = insem_id;

    ui->AnimalList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->AnimalList->setEditTriggers(QAbstractItemView::NoEditTriggers);


    bullModel = new QSqlRelationalTableModel(this);
    bullModel->setTable("bulls");
    bullModel->select();

    cowModel = new QSqlRelationalTableModel(this);
    cowModel->setTable("cows");
    cowModel->select();


}

Calving::~Calving()
{
    delete ui;
}

void Calving::on_bullButton_toggled(bool checked)
{
    if(checked)
    {
        ui->AnimalList->setModel(bullModel);
        ui->AnimalList->hideColumn(0);
        ui->AnimalList->hideColumn(3);
        ui->AnimalList->hideColumn(4);
        ui->AnimalList->hideColumn(5);
        ui->AnimalList->hideColumn(6);
        ui->AnimalList->hideColumn(7);
        ui->AnimalList->hideColumn(8);
        ui->AnimalList->hideColumn(9);
        ui->AnimalList->hideColumn(10);
        ui->AnimalList->hideColumn(11);
        ui->AnimalList->hideColumn(12);

    }
}

void Calving::on_cowButton_toggled(bool checked)
{
    if(checked)
    {
        ui->AnimalList->setModel(cowModel);
        ui->AnimalList->hideColumn(0);
        ui->AnimalList->hideColumn(3);
        ui->AnimalList->hideColumn(4);
        ui->AnimalList->hideColumn(5);
        ui->AnimalList->hideColumn(6);
        ui->AnimalList->hideColumn(7);
        ui->AnimalList->hideColumn(8);
        ui->AnimalList->hideColumn(9);
        ui->AnimalList->hideColumn(10);
        ui->AnimalList->hideColumn(11);
        ui->AnimalList->hideColumn(12);
        ui->AnimalList->hideColumn(13);
        ui->AnimalList->hideColumn(14);
        ui->AnimalList->hideColumn(15);
        ui->AnimalList->hideColumn(16);
    }
}


void Calving::on_AnimalList_doubleClicked(const QModelIndex &index)
{
    if((ui->bullButton->isChecked() == false) and (ui->cowButton->isChecked() == false)){
        return;
    }

    int animal_id, animal_number;
    QString sex;
    if(ui->bullButton->isChecked())
    {
        animal_id = bullModel->data(bullModel->index(index.row(),0)).toInt();
        animal_number = bullModel->data(bullModel->index(index.row(),1)).toInt();
        sex = tr("Male");
    }
    if(ui->cowButton->isChecked())
    {
        animal_id = cowModel->data(cowModel->index(index.row(),0)).toInt();
        animal_number = cowModel->data(cowModel->index(index.row(),1)).toInt();
        sex = tr("Female");
    }

    QSqlQuery query;
    query.prepare("INSERT INTO calving (insem_id, sex, animal_id, animal_number) "
                  "VALUES (:insem_id, :sex, :animal_id, :animal_number)");
    query.bindValue(":insem_id", insem_id);
    query.bindValue(":sex", sex);
    query.bindValue(":animal_id", animal_id);
    query.bindValue(":animal_number", animal_number);

    if(!query.exec()){
        QMessageBox::warning(this, tr("Calving Table"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(query.lastError().databaseText()));
    }
    else{
        emit(calve_added());
        MainWindow::saveLog("Calving submitting status: OK");
        this->close();
    }



}
