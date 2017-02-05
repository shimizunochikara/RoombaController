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

    //1セルあたりの距離(mm)
    static const double LengthPerCell = 300;
    static const double PI = 3.1415;

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
        forward  = 0,
        right    = 1,
        behind   = 2,
        left     = 3
    };

    static unsigned char extractDirection(unsigned char object)
    {
        return (object >> directionShift) & directionMask;
    }

    static unsigned char isRoombaObject(unsigned char object)
    {
        return ((object >> objectTypeShift) &  objectTypeMask) == roombaStartPoint;
    }

    static void convertToPositionFromMapCell(int mapRow, int mapCol, double &x, double &y)
    {
        x = (mapCol + 0.5)* LengthPerCell;
        y = (mapRow + 0.5)* LengthPerCell;
        return;
    }

    static void convertToMapCellFromPosition( double x, double y, int &mapRow, int &mapCol)
    {
        mapRow = y / LengthPerCell - 0.5;
        mapCol = x / LengthPerCell - 0.5;
        if(mapRow < 0)
        {
            mapRow = 0;
        }
        if(mapCol < 0)
        {
            mapCol = 0;
        }
        return;
    }

    static void convertToDirectionFromMapDirection(unsigned char mapDir, double &Dir)
    {
        Dir = (mapDir & directionMask) * PI / 2;
        switch((int)mapDir)
        {
        case forward:
            Dir = 0;
            break;
        case left:
            Dir = PI / 2;
            break;
        case behind:
            Dir = PI;
            break;
        case right:
            Dir = PI * 3 / 2;
            break;
        }
        return;
    }

    static void convertToMapDirectionFromDirection(double &Dir, unsigned char &mapDir)
    {
        if(0 <= Dir && Dir < PI / 2)
        {
            mapDir = forward;
        }
        else if(PI / 2 <= Dir && Dir < PI)
        {
            mapDir = left;
        }
        else if(PI <= Dir && Dir < PI * 3 / 2)
        {
            mapDir = behind;
        }
        else if(PI * 3 / 2 <= Dir && Dir < PI * 2)
        {
            mapDir = right;
        }
        else
        {
            mapDir = forward;
        }
        return;
    }
}

class MapEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapEditDialog(QWidget *parent = 0);
    ~MapEditDialog();
    void convertToPositionFromMapCell(int mapRow, int mapCol, double &x, double &y);
    void convertToMapCellFromPosition( double x, double y, int &mapRow, int &mapCol);
    void convertToDirectionFromMapDirection(unsigned char mapDir, double &Dir);
    void convertToMapDirectionFromDirection(double &Dir, unsigned char &mapDir);

private:
    Ui::MapEditDialog *ui;
    QString xmlFileName;

    int roombaPointRow;
    int roombaPointCol;
    unsigned char roombaDirection;
    unsigned char selectedDirection;

    // XMX Tag name
    const QString dataVersionTagName        = "dataVersion";
    const QString rowCountTagName           = "rowCount";
    const QString colCountTagName           = "colCount";
    const QString objectArrayTagName        = "objectArrayData";
    const QString roombaInfoTagName         = "roombaInfo";
    const QString roombaXPositionTagName    = "roombaXPosition";
    const QString roombaYPositionTagName    = "roombaYPosition";
    const QString roombaDerectionTagName    = "roombaDerection";

    // roomba logo
    const QString roombaFowardLogo = "^";
    const QString roombaLeftLogo   = "<";
    const QString roombaRightLogo  = ">";
    const QString roombaBehindLogo = "|";

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
    void clearAllObjects();
    void slotSelectedObjectChanged(const QString &);
    void pressedUpButton();
    void pressedDownButton();
    void pressedLeftButton();
    void pressedRightButton();
    void pressedDeleteButton();
};

#endif // MAPEDITDIALOG_H
