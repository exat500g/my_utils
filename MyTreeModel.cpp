#include "treeitem.h"
#include "MyTreeModel.h"
#include <qdebug.h>
#include <QStringList>

struct MyTreeModel::InternalTreeItem{
    InternalTreeItem(){
        id=++idCounter;
    }
    ~InternalTreeItem(){
        qDeleteAll(children);
    }
    void appendChild(InternalTreeItem* item){
        item->parent=this;
        children.append(item);
    }
    void insertChild(int row,InternalTreeItem* item){
        item->parent=this;
        children.insert(row,item);
    }
    QList<InternalTreeItem*> children;
    InternalTreeItem* parent=nullptr;
    int id;
    static int idCounter;
};
int MyTreeModel::InternalTreeItem::idCounter = 0;

MyTreeModel::MyTreeModel(QObject *parent)
    : QAbstractItemModel(parent) {
    rootItem = new InternalTreeItem();
    auto item1 = new InternalTreeItem();
    auto item2 = new InternalTreeItem();
    auto item3 = new InternalTreeItem();
    rootItem->appendChild(item1);
    rootItem->appendChild(item2);
    item1->appendChild(item3);
}

MyTreeModel::~MyTreeModel() {
    delete rootItem;
}

MyTreeModel::InternalTreeItem *MyTreeModel::indexToItem(const QModelIndex &index) const{
    if(!index.isValid()){
        return rootItem;
    }
    InternalTreeItem* parent=static_cast<InternalTreeItem*>(index.internalPointer());
    if(index.row() >= parent->children.size()){
        return nullptr;
    }
    return parent->children[index.row()];
}

void MyTreeModel::dump(){
    dump(rootItem);
}
void MyTreeModel::dump(InternalTreeItem* item){
    qDebug()<<item->children;
    for(auto& i : item->children){
        dump(i);
    }
}

//append row next to index
bool MyTreeModel::appendRow(const QModelIndex &index){
    InternalTreeItem *item = indexToItem(index);
    if(!item || item==rootItem)return false;
    int row=index.row()+1;
    beginInsertRows(index.parent(),row,row);
    item->parent->insertChild(row,new InternalTreeItem);
    endInsertRows();
    return true;
}

bool MyTreeModel::insertRow(const QModelIndex &index){
    InternalTreeItem *item = indexToItem(index);
    if(!item || item==rootItem)return false;
    int row=index.row();
    beginInsertRows(index.parent(),row,row);
    item->parent->insertChild(row,new InternalTreeItem);
    endInsertRows();
    return true;
}

bool MyTreeModel::removeRow(const QModelIndex &index){
    InternalTreeItem *item = indexToItem(index);
    if(!item || item==rootItem)return false;
    beginRemoveRows(index.parent(),index.row(),index.row());
    item->parent->children.removeAt(index.row());
    endRemoveRows();
    return true;
}

bool MyTreeModel::appendChild(const QModelIndex &index){
    InternalTreeItem *item = indexToItem(index);
    if(!item || item==rootItem)return false;
    int row=item->children.size();
    beginInsertRows(index,row,row);
    item->insertChild(row,new InternalTreeItem);
    endInsertRows();
    return true;
}

int MyTreeModel::columnCount(const QModelIndex &) const {
    return 2;
}

QVariant MyTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    InternalTreeItem *item = indexToItem(index);

    if(index.column()==0){
        return QString("id%1").arg(item->id);
    }
    return QString("row%1col%2").arg(index.row()).arg(index.column());
}

Qt::ItemFlags MyTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QModelIndex MyTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    InternalTreeItem *parentItem = indexToItem(parent);

    if(!parentItem || row >= parentItem->children.size()){
        return QModelIndex();
    }
    return createIndex(row, column, parentItem);
}

QModelIndex MyTreeModel::parent(const QModelIndex &index) const {
    InternalTreeItem* parent=static_cast<InternalTreeItem*>(index.internalPointer());
    if(!parent || parent==rootItem)return QModelIndex();
    InternalTreeItem *parentParent = parent->parent;
    if (!parentParent)
        parentParent = rootItem;

    return createIndex(parentParent->children.indexOf(parent), 0, parentParent);
}

int MyTreeModel::rowCount(const QModelIndex &index) const {
    InternalTreeItem* item=indexToItem(index);
    //qDebug()<<__FUNCTION__<<index<<"item="<<item;
    if(!item)return 0;
    if (index.column() > 0)
        return 0;

    return item->children.size();
}
