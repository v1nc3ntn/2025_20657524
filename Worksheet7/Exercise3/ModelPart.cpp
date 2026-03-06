#include "ModelPart.h"

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent),
    m_isVisible(true), m_colourR(255), m_colourG(255), m_colourB(255)
{
}

ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item)
{
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const
{
    return m_childItems.count();
}

int ModelPart::columnCount() const
{
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

int ModelPart::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(
            const_cast<ModelPart*>(this));
    return 0;
}

ModelPart* ModelPart::parentItem()
{
    return m_parentItem;
}

void ModelPart::setData(int column, const QVariant& value)
{
    if (column < 0 || column >= m_itemData.size())
        return;
    m_itemData[column] = value;
}

/* Visibility */
bool ModelPart::isVisible() const
{
    return m_isVisible;
}

void ModelPart::setVisible(bool visible)
{
    m_isVisible = visible;
    /* Also update the QVariant data array so treeview reflects change */
    setData(1, visible ? QString("true") : QString("false"));
}

/* Colour */
int ModelPart::getColourR() const { return m_colourR; }
int ModelPart::getColourG() const { return m_colourG; }
int ModelPart::getColourB() const { return m_colourB; }

void ModelPart::setColour(int r, int g, int b)
{
    m_colourR = r;
    m_colourG = g;
    m_colourB = b;
}

/* Name */
QString ModelPart::getName() const
{
    return data(0).toString();
}

void ModelPart::setName(const QString& name)
{
    setData(0, name);
}