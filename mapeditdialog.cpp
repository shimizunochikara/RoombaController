#include "mapeditdialog.h"
#include "ui_mapeditdialog.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QtCore>
#include <QDomNodeList>
#include <QTimer>
#include <QColor>

MapEditDialog::MapEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapEditDialog)
{
    roombaPointRow = 0;
    roombaPointCol = 0;
    roombaDirection = 0;

    ui->setupUi(this);
    // connect apply map size button
    connect(ui->pushButton_applySize, SIGNAL(pressed()), this, SLOT(applyMapSize()));
    //connect save button
    connect(ui->pushButton_mapSave, SIGNAL(pressed()), this, SLOT(saveXml()));
    //connect load button
    connect(ui->pushButton_mapLoad, SIGNAL(pressed()), this, SLOT(loadXmx()));
    //connect addObjectButton
    connect(ui->pushButton_addItem, SIGNAL(pressed()), this, SLOT(addObject()));
    //connect all clear button
    connect(ui->pushButton_deleteAllItem, SIGNAL(pressed()), this, SLOT(clearAllObjects()));
    // connect clear button
    connect(ui->pushButton_deleteItem, SIGNAL(pressed()), this, SLOT(pressedDeleteButton()));

    //connect cursor button
    connect(ui->pushButton_cursor_forward, SIGNAL(pressed()), this, SLOT(pressedUpButton()));
    connect(ui->pushButton_cursor_behind, SIGNAL(pressed()), this, SLOT(pressedDownButton()));
    connect(ui->pushButton_left, SIGNAL(pressed()), this, SLOT(pressedLeftButton()));
    connect(ui->pushButton_right, SIGNAL(pressed()), this, SLOT(pressedRightButton()));

    connect(ui->tableWidget_EditMap, SIGNAL(itemSelectionChanged()), this, SLOT(selectedCellChanged()));

    connect(ui->comboBox_SelectItem, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotSelectedObjectChanged(const QString &)));
    // initialize map data
    for(int i = 0; i < maxRowCount; i++)
    {
        for(int j = 0; j < maxColCount; j++)
        {
            mapData[i][j] = 0x00;
        }
    }

    //initialize table widget item
    for(int i = 0; i < maxRowCount; i++)
    {
        for(int j = 0; j < maxColCount; j++)
        {
            tableItem[i][j] = new QTableWidgetItem();
            ui->tableWidget_EditMap->setItem(i,j, tableItem[i][j]);
        }
    }
    xmlFileName = "mapdata.xml";
    ui->label_FileName->setText("FileName: " + xmlFileName);
    loadXmx();
}

MapEditDialog::~MapEditDialog()
{
    disconnect(ui->pushButton_applySize, SIGNAL(pressed()), this, SLOT(applyMapSize()));
    disconnect(ui->tableWidget_EditMap, SIGNAL(itemSelectionChanged()), this, SLOT(selectedCellChanged()));
    delete ui;
}

void MapEditDialog::applyMapSize()
{
    //ui->tableWidget_EditMap->clearContents();
    rowCount = ui->spinBox_Horizontal->value();
    colCount = ui->spinBox_Vertical->value();
    ui->tableWidget_EditMap->setRowCount(rowCount);
    ui->tableWidget_EditMap->setColumnCount(colCount);
    updateMap();
}

void MapEditDialog::selectedCellChanged()
{
    updateObjectComboBox();
}

void MapEditDialog::saveXml()
{
    QDomDocument doc;
    QDomElement newroot = doc.createElement("MapData");
    doc.appendChild(newroot);

    //create new element
    QDomElement versionElement = doc.createElement(dataVersionTagName);
    QDomElement rowCountElement = doc.createElement(rowCountTagName);
    QDomElement colCountElement = doc.createElement(colCountTagName);

    // Roomba Info object
    QDomElement roombaInfoElement = doc.createElement(roombaInfoTagName);
    QDomElement roombaXPositionElement = doc.createElement(roombaXPositionTagName);
    QDomElement roombaYPositionElement = doc.createElement(roombaYPositionTagName);
    QDomElement roombaDerectionElement = doc.createElement(roombaDerectionTagName);
    //assemble node
    roombaInfoElement.appendChild(roombaXPositionElement);
    roombaInfoElement.appendChild(roombaYPositionElement);
    roombaInfoElement.appendChild(roombaDerectionElement);

    // Map Array object
    QDomElement objectArrayElement = doc.createElement(objectArrayTagName);

    // create text
    QDomText domTextVersion = doc.createTextNode("1.0");
    QDomText domTextRowCount = doc.createTextNode(QString::number(rowCount));
    QDomText domTextColCount = doc.createTextNode(QString::number(colCount));

    //prepare roomba Position parameters
    double tmpRoombaX;
    double tmpRoombaY;
    double tmpRoombaDir;
    MapObject::convertToPositionFromMapCell(roombaPointRow, roombaPointCol, tmpRoombaX, tmpRoombaY);
    MapObject::convertToDirectionFromMapDirection(roombaDirection, tmpRoombaDir);

    QDomText domTextroombaXPosition = doc.createTextNode(QString::number(tmpRoombaX));
    QDomText domTexroombaYPosition = doc.createTextNode(QString::number(tmpRoombaY));
    QDomText domTextroombaDerection = doc.createTextNode(QString::number(tmpRoombaDir));

    QString strArrayData = "";
    for(int i = 0; i < rowCount; i++)
    {
        for(int j = 0; j < colCount; j++)
        {
            strArrayData += mapData[i][j];
        }
    }
    QDomText domTextArrayData = doc.createTextNode(strArrayData);

    //set text to dom object
    versionElement.appendChild(domTextVersion);
    rowCountElement.appendChild(domTextRowCount);
    colCountElement.appendChild(domTextColCount);
    roombaXPositionElement.appendChild(domTextroombaXPosition);
    roombaYPositionElement.appendChild(domTexroombaYPosition);
    roombaDerectionElement.appendChild(domTextroombaDerection);
    objectArrayElement.appendChild(domTextArrayData);

    //set object to root
    newroot.appendChild(versionElement);
    newroot.appendChild(rowCountElement);
    newroot.appendChild(colCountElement);
    newroot.appendChild(roombaInfoElement);
    newroot.appendChild(objectArrayElement);

    // save to file
    QFile file(xmlFileName);//保存するファイルを設定
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug("File cannot open!!");
        return;
    }
    QTextStream out(&file);
    doc.save(out,4);// 4 is indent count maybe…
    file.close();
}

void MapEditDialog::loadXmx()
{
    QDomDocument doc;
    QFile file(xmlFileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QString xmlFileVersion = doc.elementsByTagName(dataVersionTagName).at(0).toElement().text();
    QString rowCountStr = doc.elementsByTagName(rowCountTagName).at(0).toElement().text();
    QString colCountStr = doc.elementsByTagName(colCountTagName).at(0).toElement().text();
    QString objectArrayAtr = doc.elementsByTagName(objectArrayTagName).at(0).toElement().text();
    //get Roomba Position
    QString roombaXPosStr = doc.elementsByTagName(roombaXPositionTagName).at(0).toElement().text();
    QString roombaYPosStr = doc.elementsByTagName(roombaYPositionTagName).at(0).toElement().text();
    QString roombaDirStr = doc.elementsByTagName(roombaDerectionTagName).at(0).toElement().text();

    double tmpRoombaX = roombaXPosStr.toDouble();
    double tmpRoombaY = roombaYPosStr.toDouble();
    double tmpRoombaDir = roombaDirStr.toDouble();
    rowCount = rowCountStr.toInt();
    colCount = colCountStr.toInt();
    MapObject::convertToMapCellFromPosition(tmpRoombaX, tmpRoombaY ,roombaPointRow, roombaPointCol);
    MapObject::convertToMapDirectionFromDirection(tmpRoombaDir, roombaDirection);

    ui->spinBox_Vertical->setValue(rowCount);
    ui->spinBox_Horizontal->setValue(colCount);

    ui->tableWidget_EditMap->setRowCount(rowCount);
    ui->tableWidget_EditMap->setColumnCount(colCount);

    for(int i = 0; i < rowCount; i++)
    {
        for(int j = 0; j < colCount; j++)
        {
            const QChar ch = objectArrayAtr.at(i * colCount + j);
            mapData[i][j] = ch.toLatin1();
        }
    }

    updateMap();
}

void MapEditDialog::updateMap()
{
    //process roomba object
    QString roombaMark = "R";
    if(roombaDirection == MapObject::forward)
    {
        roombaMark = roombaFowardLogo;
    }
    else if(roombaDirection == MapObject::left)
    {
        roombaMark = roombaLeftLogo;
    }
    else if(roombaDirection == MapObject::right)
    {
        roombaMark = roombaRightLogo;
    }
    else if(roombaDirection == MapObject::behind)
    {
        roombaMark = roombaBehindLogo;
    }

    // process all cell
    for(int i = 0; i < rowCount; i++)
    {
        for(int j = 0; j < colCount; j++)
        {
            bool isRoomba = (i == roombaPointRow) && (j == roombaPointCol);
            bool isObstacle = 0x01 & (mapData[i][j] & MapObject::TranversabilityMask) >> MapObject::TranversabilityShift;
            if(isObstacle)
            {
                tableItem[i][j]->setBackgroundColor(Qt::blue);
                tableItem[i][j]->setText("");
            }
            else if(isRoomba)
            {
                tableItem[i][j]->setBackgroundColor(Qt::yellow);
                tableItem[i][j]->setText(roombaMark);
            }
            else
            {
                tableItem[i][j]->setBackgroundColor(Qt::white);
                tableItem[i][j]->setText("");
            }
            ui->tableWidget_EditMap->setItem(i,j, tableItem[i][j]->clone());
        }
    }
}

void MapEditDialog::updateObjectComboBox()
{
    //QList<QTableWidgetItem *> selectedItemList = ui->tableWidget_EditMap->selectedItems();
    QList<QTableWidgetSelectionRange> range = ui->tableWidget_EditMap->selectedRanges();
    if(range.count() == 0)
    {
        return;
    }
    int tmpRowCount = range.at(0).columnCount() * range.at(0).rowCount();

    if(tmpRowCount == 0)
    {
        ui->comboBox_SelectItem->clear();
    }
    else if(tmpRowCount == 1)
    {
        ui->comboBox_SelectItem->clear();
        ui->comboBox_SelectItem->addItem(roombaLabelText);
        ui->comboBox_SelectItem->addItem(obstaclerLabelText);
        ui->comboBox_SelectItem->addItem(vacantLabelText);
    }
    else if(tmpRowCount > 1)
    {
        ui->comboBox_SelectItem->clear();
        ui->comboBox_SelectItem->addItem(obstaclerLabelText);
        ui->comboBox_SelectItem->addItem(vacantLabelText);
    }
    updateButton();
}

void MapEditDialog::slotSelectedObjectChanged(const QString &)
{
    updateButton();
}

void MapEditDialog::updateButton()
{
    bool isCursorButtonEnable = false;
    if(ui->comboBox_SelectItem->currentText() == roombaLabelText)
    {
        isCursorButtonEnable = true;
    }
    ui->pushButton_cursor_forward->setEnabled(isCursorButtonEnable);
    ui->pushButton_cursor_behind->setEnabled(isCursorButtonEnable);
    ui->pushButton_left->setEnabled(isCursorButtonEnable);
    ui->pushButton_right->setEnabled(isCursorButtonEnable);
    ui->pushButton_addItem->setEnabled(!isCursorButtonEnable);

    if(isCursorButtonEnable)
    {
        ui->label_cursor_notation->setText("add object to touch above cursor button");
    }
    else
    {
       ui->label_cursor_notation->setText("");
    }

}

void MapEditDialog::addObject()
{
    QList<QTableWidgetSelectionRange> rangeList = ui->tableWidget_EditMap->selectedRanges();
    QTableWidgetSelectionRange rangeElem = rangeList.at(0);
    int tmpRowCount = rangeElem.columnCount() * rangeElem.rowCount();

    if(ui->comboBox_SelectItem->currentText() == roombaLabelText)
    {
        roombaDirection = selectedDirection;
        roombaPointRow = rangeElem.topRow();
        roombaPointCol = rangeElem.leftColumn();
    }
    else if(ui->comboBox_SelectItem->currentText() == obstaclerLabelText)
    {
        for(int i = rangeElem.topRow(); i <= rangeElem.bottomRow(); i++)
        {
            for(int j = rangeElem.leftColumn(); j <= rangeElem.rightColumn(); j++)
            {
                mapData[i][j] |= 0x01 << MapObject::TranversabilityShift;
            }
        }
    }
    else if(ui->comboBox_SelectItem->currentText() == vacantLabelText)
    {
        for(int i = rangeElem.topRow(); i <= rangeElem.bottomRow(); i++)
        {
            for(int j = rangeElem.leftColumn(); j <= rangeElem.rightColumn(); j++)
            {
                mapData[i][j] = 0x00;
            }
        }
    }
    updateMap();
}

void MapEditDialog::clearAllObjects()
{
    //initialize table widget item
    for(int i = 0; i < maxRowCount; i++)
    {
        for(int j = 0; j < maxColCount; j++)
        {
            mapData[i][j] = 0;

        }
    }
    ui->tableWidget_EditMap->clearContents();
}

void MapEditDialog::pressedUpButton()
{
    selectedDirection = MapObject::forward;
    addObject();
}

void MapEditDialog::pressedDownButton()
{
    selectedDirection = MapObject::behind;
    addObject();
}

void MapEditDialog::pressedLeftButton()
{
    selectedDirection = MapObject::left;
    addObject();
}

void MapEditDialog::pressedRightButton()
{
    selectedDirection = MapObject::right;
    addObject();
}

void MapEditDialog::pressedDeleteButton()
{
    QList<QTableWidgetSelectionRange> rangeList = ui->tableWidget_EditMap->selectedRanges();
    QTableWidgetSelectionRange rangeElem = rangeList.at(0);
    for(int i = rangeElem.topRow(); i <= rangeElem.bottomRow(); i++)
    {
        for(int j = rangeElem.leftColumn(); j <= rangeElem.rightColumn(); j++)
        {
            mapData[i][j] = 0x00;
        }
    }
    updateMap();
}
