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
    ui->setupUi(this);
    // connect apply map size button
    connect(ui->pushButton_applySize, SIGNAL(pressed()), this, SLOT(applyMapSize()));
    //connect save button
    connect(ui->pushButton_mapSave, SIGNAL(pressed()), this, SLOT(saveXml()));
    //connect load button
    connect(ui->pushButton_mapLoad, SIGNAL(pressed()), this, SLOT(loadXmx()));
    //connect addObjectButton
    connect(ui->pushButton_addItem, SIGNAL(pressed()), this, SLOT(addObject()));

    connect(ui->tableWidget_EditMap, SIGNAL(itemSelectionChanged()), this, SLOT(selectedCellChanged()));

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
        }
    }
    xmlFileName = "mapdata.xml";
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
    qDebug("saveXML");
    QDomDocument doc;
    QDomElement newroot = doc.createElement("MapData");
    doc.appendChild(newroot);

    //create new element
    QDomElement versionElement = doc.createElement(dataVersionTagName);
    QDomElement rowCountElement = doc.createElement(rowCountTagName);
    QDomElement colCountElement = doc.createElement(colCountTagName);
    QDomElement objectArrayElement = doc.createElement(objectArrayTagName);

    // create text
    QDomText domTextVersion = doc.createTextNode("1.0");
    QDomText domTextRowCount = doc.createTextNode(QString::number(rowCount));
    QDomText domTextColCount = doc.createTextNode(QString::number(colCount));
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
    objectArrayElement.appendChild(domTextArrayData);

    //set object to root
    newroot.appendChild(versionElement);
    newroot.appendChild(rowCountElement);
    newroot.appendChild(colCountElement);
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

    rowCount = rowCountStr.toInt();
    colCount = colCountStr.toInt();

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
    for(int i = 0; i < rowCount; i++)
    {
        for(int j = 0; j < colCount; j++)
        {
            bool isObstacle = 0x01 & (mapData[i][j] & MapObject::TranversabilityMask) >> MapObject::TranversabilityShift;
            if(isObstacle)
            {
                //tableItem[i][j]->setBackgroundColor(Qt::blue);
            }
            else
            {
                tableItem[i][j]->setBackgroundColor(Qt::white);
            }
            ui->tableWidget_EditMap->setItem(i,j, tableItem[i][j]->clone());
        }
    }
}

void MapEditDialog::updateObjectComboBox()
{
    //QList<QTableWidgetItem *> selectedItemList = ui->tableWidget_EditMap->selectedItems();
    QList<QTableWidgetSelectionRange> range = ui->tableWidget_EditMap->selectedRanges();
    int tmpRowCount = range.at(0).columnCount() * range.at(0).rowCount();

    qDebug() << tmpRowCount;
    if(tmpRowCount == 0)
    {
        qDebug("updateObjectComboBox(): 0");
        ui->comboBox_SelectItem->clear();
    }\
    else if(tmpRowCount == 1)
    {
        qDebug("updateObjectComboBox(): 1");
        ui->comboBox_SelectItem->clear();
        ui->comboBox_SelectItem->addItem(roombaLabelText);
        ui->comboBox_SelectItem->addItem(obstaclerLabelText);
        ui->comboBox_SelectItem->addItem(vacantLabelText);
    }
    else if(tmpRowCount > 1)
    {
        qDebug("updateObjectComboBox(): 2");
        ui->comboBox_SelectItem->clear();
        ui->comboBox_SelectItem->addItem(obstaclerLabelText);
        ui->comboBox_SelectItem->addItem(vacantLabelText);
    }
}

void MapEditDialog::updateButton()
{

}

void MapEditDialog::addObject()
{
    QList<QTableWidgetSelectionRange> rangeList = ui->tableWidget_EditMap->selectedRanges();
    QTableWidgetSelectionRange rangeElem = rangeList.at(0);
    int tmpRowCount = rangeElem.columnCount() * rangeElem.rowCount();
    qDebug() << ui->comboBox_SelectItem->currentText();

    if(ui->comboBox_SelectItem->currentText() == roombaLabelText)
    {
        /* feature implement.
        if(tmpRowCount != 1)
        {
            return;
        }
        else
        {

        }
        */
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
