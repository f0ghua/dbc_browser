#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

DbWindow *MainWindow::createDbWindow()
{
    DbWindow *child = new DbWindow;
    QMdiSubWindow *subWindow = ui->m_maMain->addSubWindow(child);
    subWindow->setWindowState(Qt::WindowMaximized);

    return child;
}

QMdiSubWindow *MainWindow::findDbWindow(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, ui->m_maMain->subWindowList()) {
        DbWindow *child = qobject_cast<DbWindow *>(window->widget());
        if (child->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void MainWindow::openDbFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open"), QString(), tr("DBC Database (*.dbc);;All Files (*.*)"));

    if (fileName.isEmpty())
        return;

    QMdiSubWindow *existing = findDbWindow(fileName);
    if (existing)
    {
        ui->m_maMain->setActiveSubWindow(existing);
        return;
    }

    DbWindow *child = createDbWindow();
    connect(child, &DbWindow::statusEvent, this, &setStatusBarText);

    if (child->loadFile(fileName)) {
        statusBar()->showMessage(tr("File Loaded"), 2000);
        child->show();
    } else {
        child->close();
    }

}

void MainWindow::on_m_actOpen_triggered()
{
    openDbFile();
}

void MainWindow::setStatusBarText(const QString& msg)
{
    statusBar()->showMessage(msg);
}