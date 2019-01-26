#pragma once

#include <QAbstractItemModel>
#include <QVariant>

class MyTreeModel : public QAbstractItemModel {
    Q_OBJECT
    struct InternalTreeItem;
public:
    explicit MyTreeModel(QObject *parent = nullptr);
    ~MyTreeModel()override;

    InternalTreeItem* indexToItem(const QModelIndex& index)const;
    void dump();
    void dump(InternalTreeItem*);
    bool appendRow(const QModelIndex &index);
    bool insertRow(const QModelIndex &index);
    bool removeRow(const QModelIndex &index);
    bool appendChild(const QModelIndex &index);
protected:
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
private:
    InternalTreeItem *rootItem=nullptr;
};
