#include "cowcard.h"
#include "ui_cowcard.h"
#include "bulls.h"
#include "cows.h"
#include "dbconnect.h"
#include "photodialog.h"
#include "mainwindow.h"
#include "bullcard.h"
#include "cowbonitation.h"
#include "measurement.h"
#include "insemination.h"
#include "calving.h"

#include <QtGui>
#include <QtSql>

cowCard::cowCard(QWidget *parent, int id) :
    QDialog(parent),
    ui(new Ui::cowCard)
{
    ui->setupUi(this);
    cowCard::id = id;
    cowCard::insem_id = 0;
    QString str;

    //Create model and set filter
    model = new QSqlRelationalTableModel(this);
    model->setTable("cows");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setFilter(QString("idCows=%1").arg(str.setNum(id)));
    model->select();//Create model and set filter

    //Add data mapper on cowcard form
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
    development->setFilter(QString("id_animal=%1 AND gender = 0").arg(str.setNum(id)));
    development->select();

    // Set the model and hide the ID column
    ui->development->setModel(development);
    ui->development->setColumnHidden(development->fieldIndex("idDevelopment"), true);

    // Create the data model
    measurement = new QSqlRelationalTableModel(this);
    measurement->setEditStrategy(QSqlTableModel::OnManualSubmit);
    measurement->setTable("measurement");
    measurement->setFilter(QString("id_animal=%1 AND gender = 0").arg(str.setNum(id)));
    measurement->select();

    // Set the model and hide the ID column
    ui->measurement->setModel(measurement);
    ui->measurement->setColumnHidden(measurement->fieldIndex("idMeasurement"), true);

    // Create the data model
    bonitation = new QSqlRelationalTableModel(this);
    bonitation->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bonitation->setTable("cowbonitation");
    bonitation->setFilter(QString("id_animal=%1").arg(str.setNum(id)));
    bonitation->select();

    // Set the model and hide the ID column
    ui->bonitation->setModel(bonitation);
    ui->bonitation->setColumnHidden(bonitation->fieldIndex("id"), true);
	
    // Create the data model
    productivity = new QSqlRelationalTableModel(this);
    productivity->setEditStrategy(QSqlTableModel::OnManualSubmit);
    productivity->setTable("insemination");
    productivity->setFilter(QString("cow_id=%1").arg(str.setNum(id)));
    productivity->select();

    // Set the model and hide the ID column
    ui->productivity->setModel(productivity);
    ui->productivity->setColumnHidden(productivity->fieldIndex("idinsemination"), true);

    // Create the data model
    calving = new QSqlRelationalTableModel(this);
    calving->setEditStrategy(QSqlTableModel::OnManualSubmit);
    calving->setTable("calving");
    calving->setFilter(QString("insem_id=0"));
    calving->select();

    // Set the model and hide the ID column
    ui->calving->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->calving->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->calving->setModel(calving);
    ui->calving->setColumnHidden(calving->fieldIndex("idcalving"), true);

	//Save old mother and father id's
    mother_old = mother_id(0, id);
    father_old = father_id(0, id);
    mother_new = 0;
    father_new = 0;

    //Set data in mother and father line's on cowcard form
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
    connect(ui->submitProdButton, SIGNAL(clicked()), this, SLOT(submit_prod()));
    connect(ui->revertProdButton, SIGNAL(clicked()), productivity, SLOT(revertAll()));

}

cowCard::~cowCard()
{
    delete ui;
    delete model;
    delete mapper;
}

void cowCard::submit()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting cow card. Animal id = %1").arg(id));
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

void cowCard::submit_lineage()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting lineage. Animal id = %1, mother id = %2, father id =%3").arg(id).arg(mother_new).arg(father_new));
    QSqlQuery query;
    if((mother_old == -2) or (father_old ==-2)){
        query.prepare("INSERT INTO relations (id_child, id_mother, id_father, is_bull)"
                      "VALUES (:id_child, :id_mother, :id_father, 0)");
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

void cowCard::revertAll(){
    m_line_update(mother_old);
    f_line_update(father_old);
}

void cowCard::on_MSelectButton_clicked()
{
    QWidget* cows = new Cows (this, true);
    cows->setWindowFlags(Qt::Dialog);
    cows->setWindowModality(Qt::ApplicationModal);
    QObject::connect (cows, SIGNAL(id_changed(int)),this, SLOT(m_line_update(int)));
    cows->show();
}

void cowCard::on_FSelectButton_clicked()
{
    QWidget* bulls = new Bulls (this, true);
    bulls->setWindowFlags(Qt::Dialog);
    bulls->setWindowModality(Qt::ApplicationModal);
    QObject::connect (bulls, SIGNAL(id_changed(int)),this, SLOT(f_line_update(int)));
    bulls->show();
}

void cowCard::m_line_update(int new_id){

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

void cowCard::f_line_update(int new_id){

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

void cowCard::on_photoButton_clicked()
{
    PhotoDialog photoDlg(this, this->id, 0);
    photoDlg.exec();
}

void cowCard::on_MCardButton_clicked()
{
    cowCard MCard(this, mother_current);
    MCard.exec();
}

void cowCard::on_FCardButton_clicked()
{
    bullCard FCard(this, father_current);
    FCard.exec();
}

void cowCard::on_MMCardButton_clicked()
{
    cowCard MMCard(this, mother_id(0, mother_current));
    MMCard.exec();
}

void cowCard::on_FMCardButton_clicked()
{
    bullCard FMCard(this, father_id(0, mother_current));
    FMCard.exec();
}

void cowCard::on_MFCardButton_clicked()
{
    cowCard MFCard(this, mother_id(1, father_current));
    MFCard.exec();
}

void cowCard::on_FFCardButton_clicked()
{
    bullCard FFCard(this, father_id(1, father_current));
    FFCard.exec();
}

void cowCard::on_newBonButton_clicked()
{
    cowBonitation newBonitation(this, id);
	QObject::connect(&newBonitation, SIGNAL(bon_added()), this, SLOT(bon_added()));
    newBonitation.exec();
}

void cowCard::on_newMeasButton_clicked()
{
    Measurement newMeasurement(this, id, 0);
	QObject::connect(&newMeasurement, SIGNAL(meas_added()), this, SLOT(meas_added()));
    newMeasurement.exec();
}

void cowCard::on_newProdButton_clicked()
{
    insemination newInsemination(this, id);
    QObject::connect(&newInsemination, SIGNAL(insem_added()), this, SLOT(prod_added()));
    newInsemination.exec();
}

void cowCard::submit_dev()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting changes to cow's development. Animal id = %1").arg(id));
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

void cowCard::submit_meas()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting changes to cow's measurement. Animal id = %1").arg(id));
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

void cowCard::submit_bon()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting changes to cow's bonitation. Animal id = %1").arg(id));
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

void cowCard::submit_prod()
{
    MainWindow::saveLog(QString::fromLocal8Bit("Submitting changes to cow's productivity. Animal id = %1").arg(id));
    productivity->database().transaction();
    if (productivity->submitAll()) {
        productivity->database().commit();
        MainWindow::saveLog("Submitting status: OK");
    } else {
        productivity->database().rollback();
        QMessageBox::warning(this, tr("Submit data"),
                             tr("The database reported an error: %1")
                             .arg(productivity->lastError().text()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(productivity->lastError().text()));
    }
}

void cowCard::bon_added()
{
    bonitation->select();
}

void cowCard::meas_added()
{
    measurement->select();
}

void cowCard::prod_added()
{
    productivity->select();
}

void cowCard::calve_added()
{
    calving->select();
}


void cowCard::on_productivity_clicked(const QModelIndex &index)
{
    QString insem_str;
    int count = ui->productivity->selectionModel()->selectedRows().count();
    //QModelIndexList indexes = ui->cowsTable->selectionModel()->selectedRows();
    if(count>1)
    {
        insem_id = 0;
        calving->setFilter(QString("insem_id = 0"));
        calving->select();
    }
    else
    {
        insem_id = productivity->data(productivity->index(index.row(),0)).toInt();
        qDebug() << insem_id << index << index.row();
        calving->setFilter(QString("insem_id = %1").arg(insem_str.setNum(insem_id)));
        calving->select();
    }
}

void cowCard::on_delBonButton_clicked()
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

void cowCard::on_delMeasButton_clicked()
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

void cowCard::on_delProdButton_clicked()
{
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;

    //Deleting from table
    int count = ui->productivity->selectionModel()->selectedRows().count();
    QModelIndexList indexes = ui->productivity->selectionModel()->selectedRows();

    //Row removing with index correction
    int removed = 0;
    int toDel = 1;
    int start = indexes[0].row();
    count =  indexes.count();

    for (int i = 0; i < count-1; i++){
        if(indexes.at(i).row() != (indexes.at(i+1).row()-1)){
            productivity->removeRows(start, toDel);
            removed += toDel;
            start = indexes.at(i+1).row();
            toDel = 1;
        }
        else{
            toDel++;
        }

    }
    productivity->removeRows(start, toDel);

    //Commit to database
    productivity->database().transaction();
    if (productivity->submitAll()){
        productivity->database().commit();
    }
    else{
        productivity->database().rollback();
        QMessageBox::warning(this, tr("Removing data"),
                             tr("The database reported an error: %1")
                             .arg(productivity->lastError().text()));
    }
}

void cowCard::on_delCalvingButton_clicked()
{
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;

    //Deleting from table
    int count = ui->calving->selectionModel()->selectedRows().count();
    QModelIndexList indexes = ui->calving->selectionModel()->selectedRows();

    //Row removing with index correction
    int removed = 0;
    int toDel = 1;
    int start = indexes[0].row();
    count =  indexes.count();

    for (int i = 0; i < count-1; i++){
        if(indexes.at(i).row() != (indexes.at(i+1).row()-1)){
            calving->removeRows(start, toDel);
            removed += toDel;
            start = indexes.at(i+1).row();
            toDel = 1;
        }
        else{
            toDel++;
        }

    }
    calving->removeRows(start, toDel);

    //Commit to database
    calving->database().transaction();
    if (calving->submitAll()){
        calving->database().commit();
    }
    else{
        calving->database().rollback();
        QMessageBox::warning(this, tr("Removing data"),
                             tr("The database reported an error: %1")
                             .arg(calving->lastError().text()));
    }
}

void cowCard::on_newCalvingButton_clicked()
{
    Calving nCalving (this, insem_id);
    QObject::connect(&nCalving, SIGNAL(calve_added()), this, SLOT(calve_added()));
    nCalving.exec();
}
