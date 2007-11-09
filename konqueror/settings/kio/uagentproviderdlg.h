/**
 * Copyright (c) 2001 Dawit Alemayehu <adawit@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef USERAGENTCONFIGDLG_H
#define USERAGENTCONFIGDLG_H

#include <QtGui/QDialog>
#include "ui_uagentproviderdlg.h"

class UserAgentInfo;

class UserAgentConfigDlg : public QDialog
{
  Q_OBJECT

public:
  explicit UserAgentConfigDlg( const QString& caption, UserAgentInfo* info,
                          QWidget *parent = 0, Qt::WindowFlags f = 0 );
  ~UserAgentConfigDlg();

  void setSiteName( const QString& );
  void setIdentity( const QString& );

  QString siteName();
  QString identity();
  QString alias();

protected Q_SLOTS:
  void on_siteLineEdit_textChanged( const QString& );
  void on_aliasComboBox_activated( const QString& );

protected:
  void init();

private:
  UserAgentInfo* m_userAgentInfo;
  Ui::UserAgentConfigDlg ui;
};

#endif // UAGENTPROVIDERDLG_H
