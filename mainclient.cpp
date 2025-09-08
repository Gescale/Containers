#include "mainclient.h"
#include "containerwidget.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>
#include <QStyle>

MainClient::MainClient(QWidget *parent)
    : QMainWindow(parent)
{
    _containerWidget = new ContainerWidget(this);
    setCentralWidget(_containerWidget);

    setupActions();
    setupMenus();
    setupToolbar();

    connect(_actAbout, &QAction::triggered, this, &MainClient::showAbout);
    connect(_actHelp, &QAction::triggered, this, &MainClient::showHelp);
    connect(_actExit, &QAction::triggered, this, &QWidget::close);

    // Connecting actions to container slots
    connect(_actBackup, &QAction::triggered, _containerWidget, &ContainerWidget::backupState);
    connect(_actRestore, &QAction::triggered, _containerWidget, &ContainerWidget::restoreState);
    connect(_actPostXML, &QAction::triggered, _containerWidget, &ContainerWidget::postXml);

    setWindowTitle("Containers");
    setWindowIcon(QIcon(":/Resources/images/logo.png"));
    setMinimumSize(800, 600);
    statusBar()->showMessage("Ready");
}

MainClient::~MainClient() = default;

void MainClient::showAbout() {
    QMessageBox::about(this, "About Containers",
                       "Container Management Application\nVersion 1.0\n\n"
                       "Built with Qt6.");
}

void MainClient::showHelp() {
    QMessageBox::information(this, "Help",
                             "Use the 'Containers' tab to create and allocate containers to pallets.\n"
                             "Use the 'Post XML' tab to serialize the pallet data and send it to the server.");
}

void MainClient::setupActions() {
    _actBackup = new QAction(QIcon(":/resources/backup.png"), "&Backup Unallocated", this);
    _actBackup->setToolTip("Backup the list of unallocated containers");

    _actRestore = new QAction(QIcon(":/resources/restore.png"), "&Restore Unallocated", this);
    _actRestore->setToolTip("Restore the list of unallocated containers");

    _actPostXML = new QAction(QIcon(":/resources/save.png"), "&Post Pallets to Server", this);
    _actPostXML->setToolTip("Generate XML from pallets and send to server");

    auto questionIcon = style()->standardIcon(QStyle::SP_MessageBoxQuestion);
    auto infoIcon = style()->standardIcon(QStyle::SP_MessageBoxInformation);

    _actAbout = new QAction(infoIcon, "About", this);
    _actHelp = new QAction(questionIcon, "Help", this);
    _actExit = new QAction(QIcon(":/resources/exit.png"), "E&xit", this);
}

void MainClient::setupMenus() {
    _mnuFile = menuBar()->addMenu("&File");
    _mnuFile->addAction(_actExit);

    _mnuBackup = menuBar()->addMenu("&Backup/Restore");
    _mnuBackup->addAction(_actBackup);
    _mnuBackup->addAction(_actRestore);

    _mnuPost = menuBar()->addMenu("&Network");
    _mnuPost->addAction(_actPostXML);

    _mnuHelp = menuBar()->addMenu("&Help");
    _mnuHelp->addAction(_actAbout);
    _mnuHelp->addAction(_actHelp);
}

void MainClient::setupToolbar() {
    _toolbar = addToolBar("Main toolbar");
    _toolbar->addAction(_actBackup);
    _toolbar->addAction(_actRestore);
    _toolbar->addSeparator();
    _toolbar->addAction(_actPostXML);
    _toolbar->addSeparator();
    _toolbar->addAction(_actExit);
}

void MainClient::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton res = QMessageBox::question(this, "Confirm Exit",
                                                            "Are you sure you want to exit?",
                                                            QMessageBox::Cancel | QMessageBox::Yes);
    if (res != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}
