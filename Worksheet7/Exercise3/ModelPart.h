#ifndef MODELPART_H
#define MODELPART_H
#include <QList>
#include <QVariant>
#include <QString>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

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
    bool isVisible() const;
    void setVisible(bool visible);
    int getColourR() const;
    int getColourG() const;
    int getColourB() const;
    void setColour(int r, int g, int b);
    QString getName() const;
    void setName(const QString& name);
    void loadSTL(QString fileName);
    vtkSmartPointer<vtkActor> getActor();

private:
    QList<ModelPart*> m_childItems;
    QList<QVariant> m_itemData;
    ModelPart* m_parentItem;
    bool m_isVisible;
    int m_colourR;
    int m_colourG;
    int m_colourB;
    vtkSmartPointer<vtkSTLReader> stlReader;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};
#endif // MODELPART_H