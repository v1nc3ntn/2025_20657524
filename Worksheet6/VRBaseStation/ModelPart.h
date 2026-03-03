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

private:
    QList<ModelPart*> m_childItems;
    QList<QVariant> m_itemData;
    ModelPart* m_parentItem;
};

#endif // MODELPART_H