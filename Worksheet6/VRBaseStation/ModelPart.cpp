#include "ModelPart.h"

/* Constructor - takes a list of data values and an optional parent pointer
   The data list contains things like name and visibility status
   The parent pointer links this item to its parent in the tree */
ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent)
{
}

/* Destructor - automatically deletes all child items when this item is deleted */
ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
}

/* Adds a child item to this item's list of children
   Also sets the child's parent pointer to this item */
void ModelPart::appendChild(ModelPart* item)
{
    item->m_parentItem = this;
    m_childItems.append(item);
}

/* Returns the child at the given row index */
ModelPart* ModelPart::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

/* Returns how many children this item has */
int ModelPart::childCount() const
{
    return m_childItems.count();
}

/* Returns how many columns of data this item has */
int ModelPart::columnCount() const
{
    return m_itemData.count();
}

/* Returns the data value at the given column index
   Column 0 = name, Column 1 = visible */
QVariant ModelPart::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

/* Returns what row number this item is within its parent's children list */
int ModelPart::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(
            const_cast<ModelPart*>(this));
    return 0;
}

/* Returns a pointer to this item's parent */
ModelPart* ModelPart::parentItem()
{
    return m_parentItem;
}

/* Sets the data value at the given column index */
void ModelPart::setData(int column, const QVariant& value)
{
    if (column < 0 || column >= m_itemData.size())
        return;
    m_itemData[column] = value;
}