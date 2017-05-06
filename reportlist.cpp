#include "reportlist.h"
#include "ui_reportlist.h"

//ncreport
#include "ncreport.h"
#include "ncreportoutput.h"
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewwindow.h"

reportList::reportList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportList)
{
    QStringList rList;
    rList
            << tr("Bonitation(Title)")
            << tr("Bonitation(Table 1-2)")
            << tr("Bonitation(Table 3)")
            << tr("Bonitation(Table 4)")
            << tr("Bonitation(Table 5)")
            << tr("Bonitation(Table 6)")
            << tr("Bonitation(Table 7)")
            << tr("Bonitation(Table 8)")
            << tr("Bonitation(Table 9)")
            << tr("Bonitation(Table 10)")
            << tr("Bonitation(Table 11)")
            << tr("Bonitation(Table 12)");
    ui->setupUi(this);
    ui->listWidget->addItems(rList);
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

reportList::~reportList()
{
    delete ui;
}

void reportList::on_listWidget_doubleClicked(const QModelIndex &index)
{
    switch(index.row()){
        case 0:
            report("D:/QtProjects/MFarm/title.xml");
            break;
        case 1:
            report("D:/QtProjects/MFarm/table_1_2.xml");
            break;
        case 2:
            report("D:/QtProjects/MFarm/table_3.xml");
            break;
        case 3:
            report("D:/QtProjects/MFarm/table_4.xml");
            break;
        case 4:
            report("D:/QtProjects/MFarm/table_5.xml");
            break;
        case 5:
            report("D:/QtProjects/MFarm/table_6.xml");
            break;
        case 6:
            report("D:/QtProjects/MFarm/table_7.xml");
            break;
        case 7:
            report("D:/QtProjects/MFarm/table_8.xml");
            break;
        case 8:
            report("D:/QtProjects/MFarm/table_9.xml");
            break;
        case 9:
            report("D:/QtProjects/MFarm/table_10.xml");
            break;
        case 10:
            report("D:/QtProjects/MFarm/table_11.xml");
            break;
        case 11:
            report("D:/QtProjects/MFarm/table_12.xml");
            break;

    }
}

void reportList::report(QString filename){

    NCReport *report = new NCReport();
    report->setReportFile(filename);
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
}
