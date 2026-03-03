#ifndef MODELPARTLIST_H
#define MODELPARTLIST_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include "ModelPart.h"

class ModelPartList : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ModelPartList(const QString& data, QObject* parent = nullptr);
    ~ModelPartList();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
        const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    ModelPart* getRootItem();

private:
    ModelPart* rootItem;
};

#endif // MODELPARTLIST_H