/* This file is part of the KDE project
   Copyright (C) 1999 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __konqdirlister_h__
#define __konqdirlister_h__

#include <kdirlister.h>
#include <dcopobject.h>

/**
 * A custom directory lister (KDirLister) to create KonqFileItems
 * instead of KFileItems.
 */
class KonqDirLister : public KDirLister, public DCOPObject
{
  Q_OBJECT
  K_DCOP

public:
  /**
   * Create a directory lister
   */
  KonqDirLister( bool _delayedMimeTypes = false );
  /**
   * Destroy the directory lister
   */
  ~KonqDirLister();

  /**
   * @return true if koffice documents were listed since the last clear()
   */
  //bool kofficeDocsFound() { return m_bKofficeDocs; }

k_dcop:
  /**
   * Notify that files have been added in @p directory
   * The receiver will list that directory again to find
   * the new items (since it needs more than just the names anyway).
   *
   * Note: this is ASYNC since it is supposed to be used with a broadcast
   */
  ASYNC FilesAdded( const KURL & directory );

  /**
   * Notify that files have been deleted.
   * This call passes the exact urls of the deleted files
   * so that any view showing them can simply remove them
   * or be closed (if its current dir was deleted)
   *
   * Note: this is ASYNC since it is supposed to be used with a broadcast
   */
  ASYNC FilesRemoved( const KURL::List & fileList );

signals:
  /**
   * Instruct the view to close itself, since the dir was just deleted
   * The directory is passed along for views that show multiple dirs (tree views)
   */
  void closeView( const KURL & directory );

protected:
  /**
   * Overrides the default KFileItem creation to create a KonqFileItem
   */
  virtual KFileItem * createFileItem( const KIO::UDSEntry&, const KURL&,
				      bool determineMimeTypeOnDemand );
};

#endif
