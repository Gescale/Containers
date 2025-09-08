#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QMainWindow>
class QAction;
class QMenu;
class QToolBar;
class ContainerWidget;

class MainClient : public QMainWindow {
    Q_OBJECT

public:
    explicit MainClient(QWidget *parent = nullptr);
    ~MainClient();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showAbout();
    void showHelp();

private:
    void setupActions();
    void setupMenus();
    void setupToolbar();

    ContainerWidget* _containerWidget;

    // Actions
    QAction *_actBackup;
    QAction *_actRestore;
    QAction *_actPostXML;
    QAction *_actAbout;
    QAction *_actHelp;
    QAction *_actExit;

    // Menus
    QMenu *_mnuFile;
    QMenu *_mnuBackup;
    QMenu *_mnuPost;
    QMenu *_mnuHelp;

    // Toolbar
    QToolBar *_toolbar;
};

#endif // MAINCLIENT_H
