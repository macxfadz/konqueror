/* This file is part of the KDE project
   Copyright (C) 2000 Simon Hausmann <hausmann@kde.org>
   Copyright (C) 2006, 2008 David Faure <faure@kde.org>

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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KONQ_FILEUNDOMANAGER_P_H
#define KONQ_FILEUNDOMANAGER_P_H

#include "konq_fileundomanager.h"
#include <QtCore/QStack>
#include <QUndoCommand>
#include <kurl.h>

struct KonqBasicOperation
{
    typedef QStack<KonqBasicOperation> Stack;

    KonqBasicOperation()
    { m_valid = false; }

    bool m_valid;
    bool m_renamed;

    enum Type { File, Link, Directory };
    Type m_type:2;

    KUrl m_src;
    KUrl m_dst;
    QString m_target;
    time_t m_mtime;
};

// ### I considered inheriting this from QUndoCommand.
// ### but since it is being copied by value in the code, we can't use that.
// Alternatively the data here should be contained into the QUndoCommand-subclass.
// This way we could copy the data in the manager code.
//
// ### also it would need to implement undo() itself (well, it can call the undomanager for it)
class KonqCommand
{
public:
    typedef QStack<KonqCommand> Stack;

    KonqCommand()
    {
      m_valid = false;
    }

    // TODO: is ::TRASH missing?
    bool isMoveCommand() const { return m_type == KonqFileUndoManager::MOVE || m_type == KonqFileUndoManager::RENAME; }

    bool m_valid;

    KonqFileUndoManager::CommandType m_type;
    KonqBasicOperation::Stack m_opStack;
    KUrl::List m_src;
    KUrl m_dst;
    quint64 m_serialNumber;
};

class KJob;

// This class listens to a job, collects info while it's running (for copyjobs)
// and when the job terminates, on success, it calls addCommand in the undomanager.
class KonqCommandRecorder : public QObject
{
  Q_OBJECT
public:
  KonqCommandRecorder( KonqFileUndoManager::CommandType op, const KUrl::List &src, const KUrl &dst, KIO::Job *job );
  virtual ~KonqCommandRecorder();

private Q_SLOTS:
  void slotResult( KJob *job );

  void slotCopyingDone( KIO::Job *, const KUrl &from, const KUrl &to, time_t, bool directory, bool renamed );
  void slotCopyingLinkDone( KIO::Job *, const KUrl &from, const QString &target, const KUrl &to );

private:
  KonqCommand m_cmd;
};

enum UndoState { MAKINGDIRS = 0, MOVINGFILES, STATINGFILE, REMOVINGDIRS, REMOVINGLINKS };

// The private class is, exceptionally, a real QObject
// so that it can be the class with the DBUS adaptor forwarding its signals.
class KonqFileUndoManagerPrivate : public QObject
{
    Q_OBJECT
public:
    KonqFileUndoManagerPrivate(KonqFileUndoManager* qq);

    ~KonqFileUndoManagerPrivate()
    {
        delete m_uiInterface;
    }

    void pushCommand( const KonqCommand& cmd );

    void broadcastPush( const KonqCommand &cmd );
    void broadcastPop();
    void broadcastLock();
    void broadcastUnlock();

    void addDirToUpdate( const KUrl& url );
    bool initializeFromKDesky();

    void undoStep();

    void stepMakingDirectories();
    void stepMovingFiles();
    void stepRemovingLinks();
    void stepRemovingDirectories();

    /// called by KonqFileUndoManagerAdaptor
    QByteArray get() const;

    friend class KonqUndoJob;
    /// called by KonqUndoJob
    void stopUndo( bool step );

    friend class KonqCommandRecorder;
    /// called by KonqCommandRecorder
    void addCommand( const KonqCommand &cmd );

    bool m_syncronized;
    bool m_lock;

    KonqCommand::Stack m_commands;

    KonqCommand m_current;
    KIO::Job *m_currentJob;
    UndoState m_undoState;
    QStack<KUrl> m_dirStack;
    QStack<KUrl> m_dirCleanupStack;
    QStack<KUrl> m_linkCleanupStack;
    QList<KUrl> m_dirsToUpdate;
    KonqFileUndoManager::UiInterface* m_uiInterface;

    KonqUndoJob *m_undoJob;
    quint64 m_nextCommandIndex;

    KonqFileUndoManager* q;

    // DBUS interface
Q_SIGNALS:
    /// DBUS signal
    void push(const QByteArray &command);
    /// DBUS signal
    void pop();
    /// DBUS signal
    void lock();
    /// DBUS signal
    void unlock();

public Q_SLOTS:
    // Those four slots are connected to DBUS signals
    void slotPush(QByteArray);
    void slotPop();
    void slotLock();
    void slotUnlock();

    void slotResult(KJob*);
};

#endif /* KONQ_FILEUNDOMANAGER_P_H */
