/* This file is part of the KDE project
   Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
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

// Default values for konqueror/kdesktop settings
// This file is used by konqueror and kdesktop, and will be used by kcmkonq,
// to share the same defaults

#if 0 // OLD KFM STUFF - we might want to do this also in konq ?
#define ID_STRING_OPEN 1 /* Open */
#define ID_STRING_CD   2 /* Cd */
#define ID_STRING_NEW_VIEW 3 /* New View */
#define ID_STRING_COPY  4 /* Copy */
#define ID_STRING_DELETE 5 /* Delete */
#define ID_STRING_MOVE_TO_TRASH 6 /* Move to Trash */
#define ID_STRING_PASTE 7 /* Paste */
#define ID_STRING_OPEN_WITH 8 /* Open With */
#define ID_STRING_CUT 9 /* Cut */
#define ID_STRING_MOVE 10 /* Move */
#define ID_STRING_PROP 11 /* Properties */
#define ID_STRING_LINK 12 /* Link */
#define ID_STRING_TRASH 13 /* Empty Trash Bin*/
#define ID_STRING_ADD_TO_BOOMARKS 14
#define ID_STRING_SAVE_URL_PROPS 15 /* sven */
#define ID_STRING_SHOW_MENUBAR 16 /* sven */
#define ID_STRING_UP 17 /* sven */
#define ID_STRING_BACK 18 /* sven */
#define ID_STRING_FORWARD 19 /* sven */
#endif

// appearance tab
#define DEFAULT_UNDERLINELINKS true
#define DEFAULT_WORDWRAPTEXT true // kfm-like, sorry Reggie :-)

// browser window color defaults -- Bernd
#define HTML_DEFAULT_BG_COLOR Qt::lightGray
#define HTML_DEFAULT_LNK_COLOR Qt::blue
#define HTML_DEFAULT_TXT_COLOR Qt::black
#define HTML_DEFAULT_VLNK_COLOR Qt::magenta

// FM colors
#define FM_DEFAULT_BG_COLOR Qt::white
#define FM_DEFAULT_TXT_COLOR Qt::black
#define FM_DEFAULT_HIGHLIGHTED_TXT_COLOR Qt::blue

// icon spacing defaults (is it used ?)
#define DEFAULT_ICON_SPACING 5

// root window icon text transparency default -- stefan@space.twc.de
//#define DEFAULT_TRANSPARENT_ICON_TEXT true

// show hidden files on desktop default
#define DEFAULT_SHOW_HIDDEN_ROOT_ICONS false
#define DEFAULT_VERT_ALIGN true

// the default size of the main window (currently unused)
#define MAINWINDOW_HEIGHT 360
#define MAINWINDOW_WIDTH 540

// Default terminal for Open Terminal and for 'run in terminal'
#define DEFAULT_TERMINAL "konsole"

// Default UserAgent string (e.g. Konqueror/2.0)
#define DEFAULT_USERAGENT_STRING QString("Konqueror/")+KDE_VERSION_STRING

// Default action on delete :
// 1 = move to trash, 2 = simple delete, 3 = shred
#define DEFAULT_DELETEACTION 1
