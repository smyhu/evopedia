/*
 * evopedia: An offline Wikipedia reader.
 *
 * Copyright (C) 2010-2011 evopedia developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QVector>
#include <QHash>
#include "titlelistmodel.h"


TitleListModel::TitleListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int,QByteArray> roles;
    roles[NameRole]="name";
    roles[LengthRole]="length";
    setRoleNames(roles);
}

TitleListModel::~TitleListModel()
{
//    qDeleteAll(titles.begin(),titles.end());
//    titles.clear();
}

int TitleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return titles.size();
}

bool TitleListModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return titleIter.hasNext();
}

void TitleListModel::setTitleIterator(TitleIterator iter)
{
    titleIter = iter;
    titles = QList<QSharedPointer<Title> >();

    reset();
}

void TitleListModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);
    QList<QSharedPointer<Title> > newTitles;
    while (titleIter.hasNext() && newTitles.size() < 50) {
        newTitles += titleIter.next();
    }
    beginInsertRows(QModelIndex(), titles.size(), titles.size() + newTitles.size() - 1);
    titles += newTitles;
    endInsertRows();

    emit numberPopulated(newTitles.size());
}

QVariant TitleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= titles.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
        return titles[index.row()]->getReadableName();

    if (role == NameRole)
        return titles[index.row()]->getReadableName();

    if (role == LengthRole)
        return titles[index.row()]->getArticleLength();

    return QVariant();
}

QSharedPointer<Title> TitleListModel::getTitleAt(const QModelIndex &index) const
//QSharedPointer<Title> TitleListModel::getTitleAt(int row) const
{
    if (!index.isValid())
        return QSharedPointer<Title>(new Title());

    if (index.row() >= titles.size() || index.row() < 0)
//    if (row >= titles.size() || row < 0)
        return QSharedPointer<Title>(new Title());

    return titles[index.row()];
//    return titles[row];
}

QSharedPointer<Title> TitleListModel::getTitleFrom(const QString &title) const
{
    foreach( QSharedPointer<Title> t, titles)
    {
        if(t->getReadableName()==title)
            return t;
    }

    return QSharedPointer<Title>(new Title());
}


