#ifndef MAPEDITDIALOG_H
#define MAPEDITDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class MapEditDialog;
}

namespace MapObject{
    static unsigned char TranversabilityMask = 0x80;
    static unsigned char objectTypeMask      = 0x7C;
    static unsigned char directionMask       = 0x03;

    static int TranversabilityShift = 7;
    static int objectTypeShift      = 2;
    static int directionShift       = 0;

    enum Traversablity // 2 pattern
    {
        vacant = 0, //通行可能
        Obstacle    //障害物あり
    };
    enum objectType // 32 pattern
    {
        nothing = 0,            // なし(物のない床上)
        roombaStartPoint,       // ルンバの初期位置
        roombaDoc,              // ルンバ用ドック
        stableObstacle          // 移動しない障害物(家具など)
    };

    enum direction // 4 pattern
    {
        forward = 0,
        left    = 1,
        right   = 2,
        behind  = 3
    };
}

class MapEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapEditDialog(QWidget *parent = 0);
    ~MapEditDialog();

private:
    Ui::MapEditDialog *ui;
    QString xmlFileName;

    // XMX Tag name
    const QString dataVersionTagName = "dataVersion";
    const QString rowCountTagName    = "rowCount";
    const QString colCountTagName    = "colCount";
    const QString objectArrayTagName = "objectArrayData";

    // roomba logo
    const QString roombaFowardLogo = "f";
    const QString roombaLeftLogo   = "l";
    const QString roombaRightLogo  = "r";
    const QString roombaBehindLogo = "b";

    // comboBox Label
    const QString roombaLabelText    = "Roomba";
    const QString obstaclerLabelText = "Obstacles";
    const QString vacantLabelText    = "vacant";

    // Maximum map size
    static const int maxRowCount = 1000;
    static const int maxColCount = 1000;
    QString xmlFileVersion;
    int rowCount;
    int colCount;
    unsigned char mapData[1000][1000];
    QTableWidgetItem* tableItem[1000][1000];

public slots:
    void applyMapSize();
    void selectedCellChanged();
    void saveXml();
    void loadXmx();
    void updateMap();
    void updateObjectComboBox();
    void updateButton();
    void addObject();
};

#endif // MAPEDITDIALOG_H
