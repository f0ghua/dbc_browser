#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "dbwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setStatusBarText(const QString&);

private slots:
    void on_m_actOpen_triggered();

private:
    Ui::MainWindow *ui;

    DbWindow *createDbWindow();
    QMdiSubWindow *findDbWindow(const QString &fileName);
    void openDbFile();

};

#endif // MAINWINDOW_H
