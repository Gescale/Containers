#ifndef CONTAINERWIDGET_H
#define CONTAINERWIDGET_H

#include <QWidget>
#include <QList>

class QSpinBox;
class QPushButton;
class QListWidget;
class QTextEdit;
class Container;
class Pallet;
class ContainerFactory;
class Caretaker;

class ContainerWidget : public QWidget {
    Q_OBJECT

public:
    explicit ContainerWidget(QWidget *parent = nullptr);
    ~ContainerWidget();

public slots:
    void backupState();
    void restoreState();
    void postXml();

private slots:
    void addBox();
    void addCylinder();
    void moveToPallet();
    void updateButtons();

private:
    void setupUi();
    void resetInputs();
    void updateUnallocatedList();

    // UI Elements
    QSpinBox *_boxBreadth, *_boxLength, *_boxHeight, *_boxWeight;
    QSpinBox *_cylinderDiameter, *_cylinderHeight, *_cylinderWeight;
    QPushButton *_buttonAddBox, *_buttonAddCylinder;
    QListWidget *_displayUnallocated;
    QSpinBox *_palletNumber;
    QPushButton *_buttonMoveToPallet, *_buttonBackup, *_buttonRestore;
    QPushButton *_buttonPostXML;
    QTextEdit *_displayPalletXML;

    // Data
    QList<Container*> _unallocatedContainers;
    QMap<int, Pallet*> _pallets;
    ContainerFactory* _factory;
    Caretaker* _caretaker;
};

#endif // CONTAINERWIDGET_H
