#ifndef MODELPART_H
#define MODELPART_H

#include <QList>
#include <QVariant>
#include <QString>

class ModelPart {
public:
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* item);
    ModelPart* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    ModelPart* parentItem();
    void setData(int column, const QVariant& value);

    /* These are the additional properties required by the worksheet */
    bool isVisible() const;
    void setVisible(bool visible);

    int getColourR() const;
    int getColourG() const;
    int getColourB() const;
    void setColour(int r, int g, int b);

    QString getName() const;
    void setName(const QString& name);

private:
    QList<ModelPart*> m_childItems;
    QList<QVariant> m_itemData;
    ModelPart* m_parentItem;

    /* Additional properties */
    bool m_isVisible;
    int m_colourR;
    int m_colourG;
    int m_colourB;
};

#endif // MODELPART_H