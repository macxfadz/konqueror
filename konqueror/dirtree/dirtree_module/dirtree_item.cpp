/* This file is part of the KDE project
   Copyright (C) 2000 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "konq_treepart.h"
#include "dirtree_item.h"
#include "dirtree_module.h"
#include <konq_operations.h>
#include <konq_fileitem.h>
#include <ksimpleconfig.h>
#include <kdebug.h>
#include <kglobalsettings.h>
#include <kuserprofile.h>
#include <qapplication.h>
#include <qclipboard.h>

#define MYMODULE static_cast<KonqDirTreeModule*>(module())

KonqDirTreeItem::KonqDirTreeItem( KonqTree *parent, KonqTreeItem *parentItem, KonqTreeTopLevelItem *topLevelItem, KonqFileItem *fileItem )
    : KonqTreeItem( parent, parentItem, topLevelItem ), m_fileItem( fileItem )
{
    if ( m_topLevelItem )
        MYMODULE->addSubDir( this, m_fileItem->url() );
}

KonqDirTreeItem::KonqDirTreeItem( KonqTree *parent, KonqTreeTopLevelItem *topLevelItem, KonqFileItem *fileItem )
    : KonqTreeItem( parent, topLevelItem ), m_fileItem( fileItem )
{
    if ( m_topLevelItem )
        MYMODULE->addSubDir( this, m_fileItem->url() );
}

KonqDirTreeItem::~KonqDirTreeItem()
{
    if ( m_topLevelItem )
        MYMODULE->removeSubDir( this, m_fileItem->url() );
}

void KonqDirTreeItem::setOpen( bool open )
{
    if ( open & !childCount() && m_bListable )
        MYMODULE->openSubFolder( this );

    KonqTreeItem::setOpen( open );
}

void KonqDirTreeItem::paintCell( QPainter *_painter, const QColorGroup & _cg, int _column, int _width, int _alignment )
{
    if (m_fileItem->isLink())
    {
        QFont f( _painter->font() );
        f.setItalic( TRUE );
        _painter->setFont( f );
    }
    QListViewItem::paintCell( _painter, _cg, _column, _width, _alignment );
}

KURL KonqDirTreeItem::externalURL() const
{
    return m_fileItem->url();
}

QString KonqDirTreeItem::externalMimeType() const
{
    return m_fileItem->mimetype();
}

bool KonqDirTreeItem::acceptsDrops( const QStrList & formats )
{
    if ( formats.contains("text/uri-list") )
        return m_fileItem->acceptsDrops();
    return false;
}

void KonqDirTreeItem::drop( QDropEvent * ev )
{
    KonqOperations::doDrop( m_fileItem, externalURL(), ev, tree() );
}

void KonqDirTreeItem::itemSelected()
{
    bool cutcopy, del;
    bool bInTrash = false;

    if ( m_fileItem->url().directory(false) == KGlobalSettings::trashPath() )
        bInTrash = true;

    cutcopy = del = true;

    KParts::BrowserExtension * ext = tree()->part()->extension();
    emit ext->enableAction( "copy", cutcopy );
    emit ext->enableAction( "cut", cutcopy );
    emit ext->enableAction( "trash", del && !bInTrash );
    emit ext->enableAction( "del", del );
    emit ext->enableAction( "shred", del );

    QMimeSource *data = QApplication::clipboard()->data();
    bool paste = ( data->encodedData( data->format() ).size() != 0 );

    emit ext->enableAction( "paste", paste );
}

void KonqDirTreeItem::middleButtonPressed()
{
    // Duplicated from KonqDirPart :(
    // Optimisation to avoid KRun to call kfmclient that then tells us
    // to open a window :-)
    KService::Ptr offer = KServiceTypeProfile::preferredService(m_fileItem->mimetype(), true);
    if (offer) kdDebug() << "KonqDirPart::mmbClicked: got service " << offer->desktopEntryName() << endl;
    if ( offer && offer->desktopEntryName() == "kfmclient" )
    {
        KParts::URLArgs args;
        args.serviceType = m_fileItem->mimetype();
        emit tree()->part()->extension()->createNewWindow( m_fileItem->url(), args );
    }
    else
        m_fileItem->run();
}

void KonqDirTreeItem::rightButtonPressed()
{
    KFileItemList lstItems;
    lstItems.append( m_fileItem );
    emit tree()->part()->extension()->popupMenu( QCursor::pos(), lstItems );
}
