/*  This file is part of the KDE project
    Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __konqdrag_h__
#define __konqdrag_h__

#include <qdragobject.h>
#include <qrect.h>
#include <qstring.h>
#include <qiconview.h>

#include <kurl.h>

/*
class KonqDragItem : public QIconDragItem
{
public:
    KonqDragItem() : QIconDragItem() {}
    ~KonqDragItem() {}

    QString url;
};
*/

/*****************************************************************************
 *
 * Class KonqDrag
 *
 *****************************************************************************/

class KonqDrag : public QIconDrag
{
    Q_OBJECT

public:
    //typedef QValueList<KonqDragItem> KonqList;

    KonqDrag( QWidget * dragSource, const char* name = 0 );
    virtual ~KonqDrag() {}

    const char* format( int i ) const;
    QByteArray encodedData( const char* mime ) const;

    void append( const QIconDragItem &item, const QRect &pr,
                             const QRect &tr, const QString &url );

    static bool canDecode( const QMimeSource* e );

    //static bool decode( const QMimeSource *e, QValueList<KonqDragItem> &lst );

    static bool decode( const QMimeSource *e, QStringList &uris );

protected:
    //KonqList icons;
    QStringList urls;
};

#endif
