#include "containerwidget.h"
#include "containerfactory.h"
#include "memento.h"
#include "pallet.h"
#include "xmlworker.h"
#include <QMetaProperty>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QTabWidget>
#include <QMessageBox>
#include <QThread>

ContainerWidget::ContainerWidget(QWidget *parent) :
    QWidget(parent),
    _factory(new ContainerFactory()),
    _caretaker(new Caretaker())
{
    setupUi();
    connect(_buttonAddBox, &QPushButton::clicked, this, &ContainerWidget::addBox);
    connect(_buttonAddCylinder, &QPushButton::clicked, this, &ContainerWidget::addCylinder);
    connect(_buttonMoveToPallet, &QPushButton::clicked, this, &ContainerWidget::moveToPallet);
    connect(_displayUnallocated, &QListWidget::itemSelectionChanged, this, &ContainerWidget::updateButtons);
    connect(_buttonBackup, &QPushButton::clicked, this, &ContainerWidget::backupState);
    connect(_buttonRestore, &QPushButton::clicked, this, &ContainerWidget::restoreState);
    connect(_buttonPostXML, &QPushButton::clicked, this, &ContainerWidget::postXml);

    updateButtons();
}

ContainerWidget::~ContainerWidget() {
    qDeleteAll(_unallocatedContainers);
    qDeleteAll(_pallets);
    delete _factory;
    delete _caretaker;
}

void ContainerWidget::addBox() {
    Container *box = _factory->createContainer("box", _boxWeight->value(), _boxHeight->value(),
                                               _boxLength->value(), _boxBreadth->value());
    _unallocatedContainers.append(box);
    updateUnallocatedList();
    resetInputs();
}

void ContainerWidget::addCylinder() {
    Container *cyl = _factory->createContainer("cylinder", _cylinderWeight->value(), _cylinderHeight->value(),
                                               _cylinderDiameter->value());
    _unallocatedContainers.append(cyl);
    updateUnallocatedList();
    resetInputs();
}

void ContainerWidget::moveToPallet() {
    QListWidgetItem* selectedItem = _displayUnallocated->currentItem();
    if (!selectedItem) return;

    int selectedRow = _displayUnallocated->row(selectedItem);
    Container* containerToMove = _unallocatedContainers.takeAt(selectedRow);

    int palletNum = _palletNumber->value();
    if (!_pallets.contains(palletNum)) {
        _pallets[palletNum] = new Pallet(palletNum);
    }
    _pallets[palletNum]->addContainer(containerToMove);

    updateUnallocatedList();
    QMessageBox::information(this, "Success",
                             QString("Container %1 moved to Pallet %2.").arg(containerToMove->getCode()).arg(palletNum));
}

void ContainerWidget::backupState() {
    if (_unallocatedContainers.isEmpty()) {
        QMessageBox::warning(this, "Backup Failed", "No unallocated containers to back up.");
        return;
    }

    QList<QVariantMap> state;
    for (Container* container : _unallocatedContainers) {
        QVariantMap containerData;
        const QMetaObject* metaObject = container->metaObject();
        for (int i = 0; i < metaObject->propertyCount(); ++i) {
            QMetaProperty metaProperty = metaObject->property(i);
            if (strcmp(metaProperty.name(), "objectName") != 0) {
                containerData[metaProperty.name()] = metaProperty.read(container);
            }
        }
        containerData["_type"] = container->getType();
        state.append(containerData);
    }

    _caretaker->saveState(new Memento(state));
    qDeleteAll(_unallocatedContainers);
    _unallocatedContainers.clear();
    updateUnallocatedList();

    _buttonBackup->setEnabled(false);
    _buttonRestore->setEnabled(true);

    QMessageBox::information(this, "Backup Complete", "Unallocated containers have been backed up and cleared.");
}

void ContainerWidget::restoreState() {
    Memento* memento = _caretaker->restoreState();
    if (!memento) {
        QMessageBox::warning(this, "Restore Failed", "No backup state available to restore.");
        return;
    }

    qDeleteAll(_unallocatedContainers);
    _unallocatedContainers.clear();

    QList<QVariantMap> state = memento->getState();
    for (const QVariantMap& containerData : state) {
        Container* container = nullptr;
        if (containerData["_type"].toString() == "Box") {
            container = new Box(containerData["code"].toString(), containerData["weight"].toInt(),
                                containerData["height"].toInt(), containerData["length"].toInt(),
                                containerData["breadth"].toInt());
        } else if (containerData["_type"].toString() == "Cylinder") {
            container = new Cylinder(containerData["code"].toString(), containerData["weight"].toInt(),
                                     containerData["height"].toInt(), containerData["diameter"].toInt());
        }
        if (container) {
            _unallocatedContainers.append(container);
        }
    }
    updateUnallocatedList();
    _buttonBackup->setEnabled(true);
    _buttonRestore->setEnabled(false);
    QMessageBox::information(this, "Restore Complete", "Unallocated containers have been restored from backup.");
}

void ContainerWidget::postXml() {
    if (_pallets.isEmpty()) {
        QMessageBox::warning(this, "Post Error", "There are no pallets with containers to post.");
        return;
    }

    _buttonPostXML->setEnabled(false);
    _displayPalletXML->clear();
    _displayPalletXML->setText("Generating XML and sending to server...");

    QThread* thread = new QThread;
    XmlWorker* worker = new XmlWorker();
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, [this, worker](){ worker->process(_pallets); });
    connect(worker, &XmlWorker::xmlGenerated, this, [this](const QString& xml) {
        _displayPalletXML->setText(xml);
    });
    connect(worker, &XmlWorker::error, this, [this](const QString& err) {
        QMessageBox::critical(this, "Network Error", err);
        _displayPalletXML->setText(err);
    });
    connect(worker, &XmlWorker::finished, thread, &QThread::quit);
    connect(worker, &XmlWorker::finished, worker, &XmlWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, this, [this]() { _buttonPostXML->setEnabled(true); });

    thread->start();
}

void ContainerWidget::updateButtons() {
    bool hasSelection = !_displayUnallocated->selectedItems().isEmpty();
    _buttonMoveToPallet->setEnabled(hasSelection);
}

void ContainerWidget::updateUnallocatedList() {
    _displayUnallocated->clear();
    for (const auto& container : _unallocatedContainers) {
        _displayUnallocated->addItem(container->getCode());
    }
    updateButtons();
}

void ContainerWidget::resetInputs() {
    _boxBreadth->setValue(1);
    _boxLength->setValue(1);
    _boxHeight->setValue(1);
    _boxWeight->setValue(1);
    _cylinderDiameter->setValue(1);
    _cylinderHeight->setValue(1);
    _cylinderWeight->setValue(1);
    _boxBreadth->setFocus();
}

void ContainerWidget::setupUi() {
    _boxBreadth = new QSpinBox(); _boxLength = new QSpinBox(); _boxHeight = new QSpinBox(); _boxWeight = new QSpinBox();
    _cylinderDiameter = new QSpinBox(); _cylinderHeight = new QSpinBox(); _cylinderWeight = new QSpinBox();
    _buttonAddBox = new QPushButton("Add box");
    _buttonAddCylinder = new QPushButton("Add cylinder");
    _displayUnallocated = new QListWidget();
    _palletNumber = new QSpinBox();
    _buttonMoveToPallet = new QPushButton("Move to pallet");
    _buttonBackup = new QPushButton("Backup");
    _buttonRestore = new QPushButton("Restore");
    _buttonPostXML = new QPushButton("Post XML to network");
    _displayPalletXML = new QTextEdit();
    _displayPalletXML->setReadOnly(true);
    _displayPalletXML->setFontFamily("Courier");

    _boxBreadth->setRange(1, 100); _boxLength->setRange(1, 100); _boxHeight->setRange(1, 100); _boxWeight->setRange(1, 1000);
    _cylinderDiameter->setRange(1, 100); _cylinderHeight->setRange(1, 100); _cylinderWeight->setRange(1, 1000);
    _palletNumber->setMinimum(1);
    _buttonRestore->setEnabled(false);

    QGroupBox *boxGroup = new QGroupBox("Box");
    QGridLayout *boxlayout = new QGridLayout(boxGroup);
    boxlayout->addWidget(new QLabel("Breadth"), 0, 0); boxlayout->addWidget(_boxBreadth, 0, 1);
    boxlayout->addWidget(new QLabel("Length"), 1, 0); boxlayout->addWidget(_boxLength, 1, 1);
    boxlayout->addWidget(new QLabel("Height"), 2, 0); boxlayout->addWidget(_boxHeight, 2, 1);
    boxlayout->addWidget(new QLabel("Weight"), 3, 0); boxlayout->addWidget(_boxWeight, 3, 1);
    boxlayout->addWidget(_buttonAddBox, 4, 0, 1, 2);

    QGroupBox *cylinderGroup = new QGroupBox("Cylinder");
    QGridLayout *cylinderlayout = new QGridLayout(cylinderGroup);
    cylinderlayout->addWidget(new QLabel("Diameter"), 0, 0); cylinderlayout->addWidget(_cylinderDiameter, 0, 1);
    cylinderlayout->addWidget(new QLabel("Height"), 1, 0); cylinderlayout->addWidget(_cylinderHeight, 1, 1);
    cylinderlayout->addWidget(new QLabel("Weight"), 2, 0); cylinderlayout->addWidget(_cylinderWeight, 2, 1);
    cylinderlayout->addWidget(_buttonAddCylinder, 4, 0, 1, 2);

    QWidget *bottomWidget = new QWidget;
    QGridLayout *bottomLayout = new QGridLayout(bottomWidget);
    QLabel* listLabel = new QLabel("List of unallocated containers");
    bottomLayout->addWidget(listLabel, 0, 0);
    QLabel* clickLabel = new QLabel("Click on item in list, choose a pallet number, and move the container to that pallet.");
    clickLabel->setWordWrap(true);
    bottomLayout->addWidget(_displayUnallocated, 1, 0, 5, 1);
    bottomLayout->addWidget(clickLabel, 1, 1);
    bottomLayout->addWidget(_palletNumber, 2, 1);
    bottomLayout->addWidget(_buttonMoveToPallet, 3, 1);
    QHBoxLayout *backupRestoreLayout = new QHBoxLayout;
    backupRestoreLayout->addWidget(_buttonBackup);
    backupRestoreLayout->addWidget(_buttonRestore);
    bottomLayout->addLayout(backupRestoreLayout, 4, 1);
    bottomLayout->setColumnStretch(0, 1);
    bottomLayout->setColumnStretch(1, 1);

    QWidget *containerWidget = new QWidget;
    QGridLayout *tabLayoutContainer = new QGridLayout(containerWidget);
    tabLayoutContainer->addWidget(boxGroup, 0, 0);
    tabLayoutContainer->addWidget(cylinderGroup, 0, 1);
    tabLayoutContainer->addWidget(bottomWidget, 1, 0, 1, 2);

    QWidget *postWidget = new QWidget;
    QVBoxLayout *postLayout = new QVBoxLayout(postWidget);
    postLayout->addWidget(_buttonPostXML);
    postLayout->addWidget(_displayPalletXML);

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab(containerWidget, "Containers");
    tabWidget->addTab(postWidget, "Post XML");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
}
