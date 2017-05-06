#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QtSql>
#include <QSqlDatabase>
#include <QString>
#include "mainwindow.h"



bool createConnection(QString host, QString database);

int father_id(int gender, int id_child);

int mother_id(int gender, int id_child);

QString father_name(int id);

QString mother_name(int id);

int father_number(int id);

int mother_number(int id);

void saveLog(const QString &datastr);

#endif // DBCONNECT_H
