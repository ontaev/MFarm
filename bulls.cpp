#include "bulls.h"
#include "ui_bulls.h"

#include "bullcard.h"
#include "newbull.h"
#include "bullsfilter.h"

#include <QtSql>
#include <QMessageBox>
#include <QtGui>
#include <QtGlobal>

//ncreport
#include "ncreport.h"
#include "ncreportoutput.h"
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewwindow.h"

Bulls::Bulls(QWidget *parent, bool is_select) :
    QWidget(parent),
    ui(new Ui::Bulls)
{
    Bulls::is_select = is_select;
    filter = false;
    ui->setupUi(this);

    ui->bullsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bullsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   
	// Create the data model
    model = new QSqlRelationalTableModel(ui->bullsTable);//4e za fignya?
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("bulls");
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Number"));
    model->setHeaderData(2, Qt::Horizontal, tr("Name"));
    model->setHeaderData(3, Qt::Horizontal, tr("Inventary\n number"));
    model->setHeaderData(4, Qt::Horizontal, tr("Mark"));
    model->setHeaderData(5, Qt::Horizontal, tr("Breed"));
    model->setHeaderData(6, Qt::Horizontal, tr("Line"));
    model->setHeaderData(7, Qt::Horizontal, tr("Date of birth"));
    model->setHeaderData(8, Qt::Horizontal, tr("Place of birth"));
    model->setHeaderData(9, Qt::Horizontal, tr("Colour"));
    model->setHeaderData(10, Qt::Horizontal, tr("Family"));
    model->setHeaderData(11, Qt::Horizontal, tr("Date of arrival"));
    model->setHeaderData(12, Qt::Horizontal, tr("Owner"));

    // Set the model and hide the ID column
    ui->bullsTable->setModel(model);
    ui->bullsTable->setColumnHidden(model->fieldIndex("idBulls"), true);
}

Bulls::~Bulls()
{
    delete ui;
    delete model;
}

void Bulls::on_bullAddButton_clicked()
{
    newBull nBull (this);
    QObject::connect(&nBull, SIGNAL(bull_added()), this, SLOT(bull_added()));
    nBull.exec();
}

void Bulls::on_bullsTable_doubleClicked(const QModelIndex &index)
{
    int id = model->data(model->index(index.row(),0)).toInt();

    if (is_select){
        emit(id_changed(id));
    }
    else{
        bullCard bCard (this, id);
        bCard.exec();
    }
}

void Bulls::on_bullDelButton_clicked()
{
    if (QMessageBox::question(this, tr("Removing data"), tr("Data will be removed permanently. Continue?"),
    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)    return;

    //Deleting from "bulls" table
    int count = ui->bullsTable->selectionModel()->selectedRows().count();
    QModelIndexList indexes = ui->bullsTable->selectionModel()->selectedRows();

    //Check relations table
    QString Numbers;
    int deleted = 0;
    for (int i = 0; i < count; i++){
        int bull_id = model->data(model->index(indexes.at(i-deleted).row(),0)).toInt();
        QString bull_number = model->data(model->index(indexes.at(i-deleted).row(),1)).toString();
        qDebug() << i << " " << bull_id << "  " << bull_number;
        QSqlQuery query;
        query.prepare("SELECT * FROM relations WHERE id_father = :ID");
        query.bindValue(":ID", bull_id);
        if(query.exec()){
            if(query.size() > 0){
                Numbers.append(bull_number+",");
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

    //Row removing with index correction !!!пюгнапюрэяъ я сдюкемхел рср йняъй Я ЛЮМСЮК ЯЮАЛХР!!!!
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

void Bulls::on_filterButton_clicked()
{
    if(filter){
        model->setFilter("");
        ui->filterButton->setText(tr("Filter"));
    }
    else{
        bullsFilter bullFilter(this);
        QObject::connect (&bullFilter, SIGNAL(set_filter(QString)),this, SLOT(set_filter(QString)));
        bullFilter.exec();
    }
}

void Bulls::set_filter(QString filter_str){
    filter = true;
    model->setFilter(filter_str);
    model->select();
    ui->filterButton->setText(tr("Reset filter"));
}

void Bulls::bull_added(){
    qDebug() << "qqq";
    model->select();
}

void Bulls::print(){



    NCReport *report = new NCReport();
    //report->clearBatch();
    report->setReportFile("D:/QtProjects/MFarm/report2.xml");
    //report->setReportFile("D:/QtProjects/MFarm/plan.xml");

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

void Bulls::printDocument(QPrinter *printer)
{
//    QTextEdit doc;
//    QString text;
//    text = QString::fromLocal8Bit("<table width=\"100%\" border =3>"
//                  "<tr><td colspan=4>л</td><td colspan=4>н</td></tr>"
//                  "<tr><td colspan=4>%Mother</td><td colspan=4>%Father</td></tr>"
//                  "<tr><td colspan=2>лл</td><td colspan=2>нл</td><td colspan=2>лн</td><td colspan=2>нн</td></tr>"
//                  "<tr><td>ллл</td><td>нлл</td><td>лнл</td><td>ннл</td><td>ллн</td><td>нлн</td><td>лнн</td><td>ннн</td></tr>"
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


