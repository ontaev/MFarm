#include "bullcard.h"
#include "ui_bullcard.h"
#include "bulls.h"
#include "cows.h"
#include "dbconnect.h"
#include "photodialog.h"
#include "mainwindow.h"
#include "cowcard.h"
#include "bullbonitation.h"
#include "measurement.h"

#include <QtGui>
#include <QtSql>

bullCard::bullCard(QWidget *parent, int id) :
    QDialog(parent),
    ui(new Ui::bullCard)
{
    ui->setupUi(this);
    bullCard::id = id;
    QString str;

    //Create model and set filter
    model = new QSqlRelationalTableModel(this);
    model->setTable("bulls");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setFilter(QString("idBulls=%1").arg(str.setNum(id)));
    model->select();//Create model and set filter

    //Add data mapper on bullcard form
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(ui->NumberTxt, model->fieldIndex("Number"));
    mapper->addMapping(ui->NameTxt, model->fieldIndex("Name"));
    mapper->addMapping(ui->InvNumTxt, model->fieldIndex("InvNumber"));
    mapper->addMapping(ui->MarkTxt, model->fieldIndex("Mark"));
    mapper->addMapping(ui->BreedTxt, model->fieldIndex("Breed"));
    mapper->addMapping(ui->LineTxt, model->fieldIndex("Line"));
    mapper->addMapping(ui->BirthDateTxt, model->fieldIndex("BirthDate"));
    mapper->addMapping(ui->BirthPlaceTxt, model->fieldIndex("BirthPlace"));
    mapper->addMapping(ui->ColourTxt, model->fieldIndex("Colour"));
    mapper->addMapping(ui->FamilyTxt, model->fieldIndex("Family"));
    mapper->addMapping(ui->InDateTxt, model->fieldIndex("InDate"));
    mapper->addMapping(ui->OwnerTxt, model->fieldIndex("Owner"));
    mapper->setCurrentIndex(0);

    // Create the data model
    development = new QSqlRelationalTableModel(this);
    development->setEditStrategy(QSqlTableModel::OnManualSubmit);
    development->setTable("development");
    development->setFilter(QString("id_animal=%1 AND gender = 1").arg(str.setNum(id)));
    development->select();

    // Set the model and hide the ID column
    ui->development->setModel(development);
    ui->development->setColumnHidden(development->fieldIndex("idDevelopment"), true);

    // Create the data model
    measurement = new QSqlRelationalTableModel(this);
    measurement->setEditStrategy(QSqlTableModel::OnManualSubmit);
    measurement->setTable("measurement");
    measurement->setFilter(QString("id_animal=%1 AND gender = 1").arg(str.setNum(id)));
    measurement->select();

    // Set the model and hide the ID column
    ui->measurement->setModel(measurement);
    ui->measurement->setColumnHidden(measurement->fieldIndex("idMeasurement"), true);

    // Create the data model
    bonitation = new QSqlRelationalTableModel(this);
    bonitation->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bonitation->setTable("bullbonitation");
    bonitation->setFilter(QString("id_animal=%1").arg(str.setNum(id)));
    bonitation->select();

    // Set the model and hide the ID column
    ui->bonitation->setModel(bonitation);
    ui->bonitation->setColumnHidden(bonitation->fieldIndex("id"), true);

    //Save old mother and father id's
    mother_old = mother_id(1, id);
    father_old = father_id(1, id);
    mother_new = 0;
    father_new = 0;

    //Set data in mother and father line's
    m_line_update(mother_old);
    f_line_update(father_old);

    //Connect button's
    connect(ui->closeButton, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(ui->revertButton, SIGNAL(clicked()), model, SLOT(revertAll()));
    connect(ui->submitLineage, SIGNAL(clicked()), this, SLOT(submit_lineage()));
    connect(ui->revertLineage, SIGNAL(clicked()), this, SLOT(revertAll()));

    connect(ui->submitDevButton, SIGNAL(clicked()), this, SLOT(submit_dev()));
    connect(ui->revertDevButton, SIGNAL(clicked()), development, SLOT(revertAll()));
    connect(ui->submitMeasButton, SIGNAL(clicked()), this, SLOT(submit_meas()));
    connect(ui->revertMeasButton, SIGNAL(clicked()), measurement, SLOT(revertAll()));
    connect(ui->submitBonButton, SIGNAL(clicked()), this, SLOT(submit_bon()));
    connect(ui->revertBonButton, SIGNAL(clicked()), bonitation, SLOT(revertAll()));
}

bullCard::~bullCard()
{
    delete ui;
    delete model;
    delete mapper;
}

void bullCard::submit()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting bull card. Animal id = %1").arg(id));
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
        MainWindow::saveLog("Submitting status: OK");
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Submit data"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(model->lastError().text()));
    }
}

void bullCard::submit_lineage()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting lineage. Animal id = %1, mother id = %2, father id =%3").arg(id).arg(mother_new).arg(father_new));
    QSqlQuery query;
    if((mother_old == -2) or (father_old ==-2)){
        query.prepare("INSERT INTO relations (id_child, id_mother, id_father, is_bull)"
                      "VALUES (:id_child, :id_mother, :id_father, 1)");
        query.bindValue(":id_child", id);
        query.bindValue(":id_mother", mother_new);
        query.bindValue(":id_father", father_new);

    }
    else{
        query.prepare("UPDATE relations "
                      "SET id_mother = :id_mother, id_father = :id_father "
                      "WHERE id_child = :id_child");
        query.bindValue(":id_child", id);
        query.bindValue(":id_mother", mother_new);
        query.bindValue(":id_father", father_new);
    }
    if(!query.exec()){
        QMessageBox::warning(this, tr("Lineage Table"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(query.lastError().databaseText()));
    }
    else{
        mother_old = mother_new;
        father_old = father_new;
        MainWindow::saveLog("Submitting status: OK");
    }


}

void bullCard::revertAll(){
    m_line_update(mother_old);
    f_line_update(father_old);
}

void bullCard::on_MSelectButton_clicked()
{
    QWidget* cows = new Cows (this, true);
    cows->setWindowFlags(Qt::Dialog);
    cows->setWindowModality(Qt::ApplicationModal);
    QObject::connect (cows, SIGNAL(id_changed(int)),this, SLOT(m_line_update(int)));
    cows->show();
}

void bullCard::on_FSelectButton_clicked()
{
    QWidget* bulls = new Bulls (this, true);
    bulls->setWindowFlags(Qt::Dialog);
    bulls->setWindowModality(Qt::ApplicationModal);
    QObject::connect (bulls, SIGNAL(id_changed(int)),this, SLOT(f_line_update(int)));
    bulls->show();
}

void bullCard::m_line_update(int new_id){

    if(new_id < 0) return;
    mother_new = new_id;
    mother_current = mother_new;

    ui->MNumberTxt->setValue(mother_number(new_id));
    ui->MNameTxt->setText(mother_name(new_id));

    int MM_id = mother_id(0,new_id);
    ui->MMNumberTxt->setValue(mother_number(MM_id));
    ui->MMNameTxt->setText(mother_name(MM_id));

    int FM_id = father_id(0,new_id);
    ui->FMNumberTxt->setValue(father_number(FM_id));
    ui->FMNameTxt->setText(father_name(FM_id));

    int MMM_id = mother_id(0,MM_id);
    ui->MMMNumberTxt->setValue(mother_number(MMM_id));

    int FMM_id = father_id (0,MM_id);
    ui->FMMNumberTxt->setValue(father_number(FMM_id));

    int MFM_id = mother_id(1,FM_id);
    ui->MFMNumberTxt->setValue(mother_number(MFM_id));

    int FFM_id = father_id(1,FM_id);
    ui->FFMNumberTxt->setValue(father_number(FFM_id));

}

void bullCard::f_line_update(int new_id){

    if(new_id < 0) return;
    father_new = new_id;
    father_current = father_new;

    ui->FNumberTxt->setValue(father_number(new_id));
    ui->FNameTxt->setText(father_name(new_id));

    int MF_id = mother_id(1,new_id);
    ui->MFNumberTxt->setValue(mother_number(MF_id));
    ui->MFNameTxt->setText(mother_name(MF_id));

    int FF_id = father_id(1,new_id);
    ui->FFNumberTxt->setValue(father_number(FF_id));
    ui->FFNameTxt->setText(father_name(FF_id));

    int MMF_id = mother_id(0,MF_id);
    ui->MMFNumberTxt->setValue(mother_number(MMF_id));

    int FMF_id = father_id (0,MF_id);
    ui->FMFNumberTxt->setValue(father_number(FMF_id));

    int MFF_id = mother_id(1,FF_id);
    ui->MFFNumberTxt->setValue(mother_number(MFF_id));

    int FFF_id = father_id(1,FF_id);
    ui->FFFNumberTxt->setValue(father_number(FFF_id));
}

void bullCard::on_photoButton_clicked()
{
    PhotoDialog photoDlg(this, this->id, 1);
    photoDlg.exec();
}

void bullCard::on_MCardButton_clicked()
{
    cowCard MCard(this, mother_current);
    MCard.exec();
}

void bullCard::on_FCardButton_clicked()
{
    bullCard FCard(this, father_current);
    FCard.exec();
}

void bullCard::on_MMCardButton_clicked()
{
    cowCard MMCard(this, mother_id(0, mother_current));
    MMCard.exec();
}

void bullCard::on_FMCardButton_clicked()
{
    bullCard FMCard(this, father_id(0, mother_current));
    FMCard.exec();
}

void bullCard::on_MFCardButton_clicked()
{
    cowCard MFCard(this, mother_id(1, father_current));
    MFCard.exec();
}

void bullCard::on_FFCardButton_clicked()
{
    bullCard FFCard(this, father_id(1, father_current));
    FFCard.exec();
}

void bullCard::on_newBonButton_clicked()
{
    bullBonitation newBonitation(this, id);
    QObject::connect(&newBonitation, SIGNAL(bon_added()), this, SLOT(bon_added()));
    newBonitation.exec();
}

void bullCard::on_newMeasButton_clicked()
{
    Measurement newMeasurement(this, id, 1);
    QObject::connect(&newMeasurement, SIGNAL(meas_added()), this, SLOT(meas_added()));
    newMeasurement.exec();
}

void bullCard::submit_dev()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting changes to bull's development. Animal id = %1").arg(id));
    development->database().transaction();
    if (development->submitAll()) {
        development->database().commit();
        MainWindow::saveLog("Submitting status: OK");
    } else {
        development->database().rollback();
        QMessageBox::warning(this, tr("Submit data"),
                             tr("The database reported an error: %1")
                             .arg(development->lastError().text()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(development->lastError().text()));
    }
}

void bullCard::submit_meas()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting changes to bull's measurement. Animal id = %1").arg(id));
    measurement->database().transaction();
    if (measurement->submitAll()) {
        measurement->database().commit();
        MainWindow::saveLog("Submitting status: OK");
    } else {
        measurement->database().rollback();
        QMessageBox::warning(this, tr("Submit data"),
                             tr("The database reported an error: %1")
                             .arg(measurement->lastError().text()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(measurement->lastError().text()));
    }
}

void bullCard::submit_bon()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting changes to bull's bonitation. Animal id = %1").arg(id));
    bonitation->database().transaction();
    if (bonitation->submitAll()) {
        bonitation->database().commit();
        MainWindow::saveLog("Submitting status: OK");
    } else {
        bonitation->database().rollback();
        QMessageBox::warning(this, tr("Submit data"),
                             tr("The database reported an error: %1")
                             .arg(bonitation->lastError().text()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(bonitation->lastError().text()));
    }
}

void bullCard::bon_added()
{
    bonitation->select();
}

void bullCard::meas_added()
{
    measurement->select();
}

void bullCard::on_delBonButton_clicked()
{
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;

    //Deleting from table
    int count = ui->bonitation->selectionModel()->selectedRows().count();
    QModelIndexList indexes = ui->bonitation->selectionModel()->selectedRows();

    //Row removing with index correction
    int removed = 0;
    int toDel = 1;
    int start = indexes[0].row();
    count =  indexes.count();

    for (int i = 0; i < count-1; i++){
        if(indexes.at(i).row() != (indexes.at(i+1).row()-1)){
            bonitation->removeRows(start, toDel);
            removed += toDel;
            start = indexes.at(i+1).row();
            toDel = 1;
        }
        else{
            toDel++;
        }

    }
    bonitation->removeRows(start, toDel);

    //Commit to database
    bonitation->database().transaction();
    if (bonitation->submitAll()){
        bonitation->database().commit();
    }
    else{
        bonitation->database().rollback();
        QMessageBox::warning(this, tr("Removing data"),
                             tr("The database reported an error: %1")
                             .arg(bonitation->lastError().text()));
    }
}

void bullCard::on_delMeasButton_clicked()
{
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;

    //Deleting from table
    int count = ui->measurement->selectionModel()->selectedRows().count();
    QModelIndexList indexes = ui->measurement->selectionModel()->selectedRows();

    //Row removing with index correction
    int removed = 0;
    int toDel = 1;
    int start = indexes[0].row();
    count =  indexes.count();

    for (int i = 0; i < count-1; i++){
        if(indexes.at(i).row() != (indexes.at(i+1).row()-1)){
            measurement->removeRows(start, toDel);
            removed += toDel;
            start = indexes.at(i+1).row();
            toDel = 1;
        }
        else{
            toDel++;
        }

    }
    measurement->removeRows(start, toDel);

    //Commit to database
    measurement->database().transaction();
    if (measurement->submitAll()){
        measurement->database().commit();
    }
    else{
        measurement->database().rollback();
        QMessageBox::warning(this, tr("Removing data"),
                             tr("The database reported an error: %1")
                             .arg(measurement->lastError().text()));
    }
}
