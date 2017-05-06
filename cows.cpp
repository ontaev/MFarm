#include "cows.h"
#include "ui_cows.h"

#include "cowcard.h"
#include "newcow.h"
#include "cowsfilter.h"

#include <QtSql>
#include <QMessageBox>
#include <QtGui>
#include <QtGlobal>

//ncreport
#include "ncreport.h"
#include "ncreportoutput.h"
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewwindow.h"

Cows::Cows(QWidget *parent, bool is_select) :
    QWidget(parent),
    ui(new Ui::Cows)
{
    Cows::is_select = is_select;
    filter = false;
    ui->setupUi(this);

    ui->cowsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->cowsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Create the data model
    model = new QSqlRelationalTableModel(ui->cowsTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("cows");
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Number"));
    model->setHeaderData(2, Qt::Horizontal, tr("Name"));
    model->setHeaderData(3, Qt::Horizontal, tr("Inventary\n number"));
    model->setHeaderData(4, Qt::Horizontal, tr("Mark"));
    model->setHeaderData(5, Qt::Horizontal, tr("Breed"));
    model->setHeaderData(6, Qt::Horizontal, tr("Line"));
    model->setHeaderData(7, Qt::Horizontal, tr("Validity"));
    model->setHeaderData(8, Qt::Horizontal, tr("Family"));
    model->setHeaderData(9, Qt::Horizontal, tr("Date of birth"));
    model->setHeaderData(10, Qt::Horizontal, tr("Purpose"));
    model->setHeaderData(11, Qt::Horizontal, tr("First calving"));
    model->setHeaderData(12, Qt::Horizontal, tr("Colour"));
    model->setHeaderData(13, Qt::Horizontal, tr("Place of birth"));
    model->setHeaderData(14, Qt::Horizontal, tr("Blood group"));
    model->setHeaderData(15, Qt::Horizontal, tr("Date of arrival"));
    model->setHeaderData(16, Qt::Horizontal, tr("Owner"));

    // Set the model and hide the ID column
    ui->cowsTable->setModel(model);
    ui->cowsTable->setColumnHidden(model->fieldIndex("idCows"), true);
}

Cows::~Cows()
{
    delete ui;
    delete model;
}

void Cows::on_cowAddButton_clicked()
{
    newCow nCow (this);
    QObject::connect(&nCow, SIGNAL(cow_added()), this, SLOT(cow_added()));
    nCow.exec();
}

void Cows::on_cowsTable_doubleClicked(const QModelIndex &index)
{
    int id = model->data(model->index(index.row(),0)).toInt();

    if (is_select){
        emit(id_changed(id));
    }
    else{
        cowCard cCard (this, id);
        cCard.exec();
    }
}

void Cows::on_cowDelButton_clicked()
{
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;

    //Deleting from "cows" table
    int count = ui->cowsTable->selectionModel()->selectedRows().count();
    QModelIndexList indexes = ui->cowsTable->selectionModel()->selectedRows();

	//Check relations table
    QString Numbers;
    int deleted = 0;
    for (int i = 0; i < count; i++){
        int cow_id = model->data(model->index(indexes.at(i-deleted).row(),0)).toInt();
        QString cow_number = model->data(model->index(indexes.at(i-deleted).row(),1)).toString();
        qDebug() << i << " " << cow_id << "  " << cow_number;
        QSqlQuery query;
        query.prepare("SELECT * FROM relations WHERE id_mother = :ID");
        query.bindValue(":ID", cow_id);
        if(query.exec()){
            if(query.size() > 0){
                Numbers.append(cow_number+",");
                indexes.removeAt(i-deleted);
                deleted = deleted + 1;
            }
        }
        else{
            QMessageBox::warning(this, tr("Check relations"), tr("The database reported an error: %1 Can not check relations!").arg(query.lastError().databaseText()));
            return;
        }
    }

    Numbers.chop(1);
    if (deleted) QMessageBox::warning(this, tr("Check relations"), tr("Can not delete numbers: ")+Numbers);

    //Row removing with index correction
    int removed = 0;
    int toDel = 1;
    int start = indexes[0].row();
	count =  indexes.count();

    for (int i = 0; i < count-1; i++){
        if(indexes.at(i).row() != indexes.at(i+1).row()-1){
            model->removeRows(start - removed, toDel);
            removed += toDel;
            start = indexes.at(i+1).row();
            toDel = 1;
        }
        else{
            toDel++;
        }

    }
    model->removeRows(start - removed, toDel);

    //Commit to database
    model->database().transaction();
    if (model->submitAll()){
        model->database().commit();
    }
	else{
        model->database().rollback();
        QMessageBox::warning(this, tr("Removing data"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
}

void Cows::on_filterButton_clicked()
{
    if(filter){
        model->setFilter("");
        ui->filterButton->setText(tr("Filter"));
    }
    else{
        cowsFilter cowFilter(this);
        QObject::connect (&cowFilter, SIGNAL(set_filter(QString)),this, SLOT(set_filter(QString)));
        cowFilter.exec();
    }
}

void Cows::set_filter(QString filter_str){
    filter = true;
    model->setFilter(filter_str);
    model->select();
    ui->filterButton->setText(tr("Reset filter"));
}

void Cows::cow_added(){
    qDebug() << "qqq";
    model->select();
}

void Cows::print(){



    NCReport *report = new NCReport();
    //report->clearBatch();
    report->setReportFile("D:/QtProjects/MFarm/report2.xml");
    report->addParameter("id", 5);

    report->runReportToPreview();


        if (report->hasError()) {

            qDebug() << "ERROR:" << report->lastErrorMsg();

        } else {

            NCReportPreviewWindow *pv = new NCReportPreviewWindow();
            pv->setOutput( (NCReportPreviewOutput*)report->output() );
            pv->setWindowModality( Qt::ApplicationModal );
            pv->setAttribute( Qt::WA_DeleteOnClose );
            pv->show();
        }

        delete report;

//////    qDebug() << report->reportSource();
//    NCReportPreviewOutput *output = 0;
//    qDebug() << "HUY";
//    output = new NCReportPreviewOutput();
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//    report->setOutput(output);
//    report->runReport();
//    bool error = report->hasError();
//    QString err = report->lastErrorMsg();

//    QApplication::restoreOverrideCursor();
//    qDebug() << "HUY";
//////    report->runReportToPrinter(1, true, 0);

//    NCReportPreviewWindow *pv = new NCReportPreviewWindow();
//    qDebug() << "HUY";
//    pv->setReport(report);
//    pv->setOutput((NCReportPreviewOutput*)report->output());
//    pv->setWindowModality(Qt::ApplicationModal);
//    pv->setAttribute(Qt::WA_DeleteOnClose);
//    pv->show();


//    qDebug() << "Printing Bulls!!!";
//    QPrinter printer(QPrinter::HighResolution);
//    QPrintPreviewDialog preview(&printer, this);

//    connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printDocument(QPrinter*)));
//    preview.exec();
}

void Cows::printDocument(QPrinter *printer)
{
//    QTextEdit doc;
//    QString text;
//    text = QString::fromLocal8Bit("<table width=\"100%\" border =3>"
//                  "<tr><td colspan=4>М</td><td colspan=4>О</td></tr>"
//                  "<tr><td colspan=4>%Mother</td><td colspan=4>%Father</td></tr>"
//                  "<tr><td colspan=2>ММ</td><td colspan=2>ОМ</td><td colspan=2>МО</td><td colspan=2>ОО</td></tr>"
//                  "<tr><td>МММ</td><td>ОММ</td><td>МОМ</td><td>ООМ</td><td>ММО</td><td>ОМО</td><td>МОО</td><td>ООО</td></tr>"
//                  "</table>");

//    doc.setText(text);
//    doc.print(printer);

//    QPainter painter(printer);
//    QRect rect = painter.viewport();
//    int side = qMin(rect.width(), rect.height());
//    painter.setViewport(0, 0, side, side);
//    painter.setWindow(-50, -50, 100, 100);
//    painter.drawRect(-50, -50, 100, 100);





}
