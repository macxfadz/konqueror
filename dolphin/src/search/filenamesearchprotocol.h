/***************************************************************************
 *   Copyright (C) 2010 by Peter Penz <peter.penz19@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#ifndef FILENAMESEARCHPROTOCOL_H
#define FILENAMESEARCHPROTOCOL_H
 
#include <kio/slavebase.h>

class KUrl;
class QRegExp;
 
/**
 * @brief Lists files where the filename matches do a given query.
 *
 * The query is defined as part of the "search" query item of the URL.
 * Example: The URL filenamesearch:///home/peter?search=hello lists
 * recursively all files inside the directory home/peter, that contain
 * the "hello" as part of their filename.
 */
class FileNameSearchProtocol : public KIO::SlaveBase {
public:
    FileNameSearchProtocol(const QByteArray& pool, const QByteArray& app);
    virtual ~FileNameSearchProtocol();
    
    virtual void listDir(const KUrl& url);

private:
    void searchDirectory(const KUrl& directory);

    /**
     * @return True, if the pattern m_searchPattern is part of
     *         the file \a fileName.
     */
    bool containsPattern(const KUrl& fileName) const;

    bool m_checkContent;
    QRegExp* m_regExp;
};
 
#endif