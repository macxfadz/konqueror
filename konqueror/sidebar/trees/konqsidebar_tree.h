#ifndef _konq_sidebar_test_h_
#define _konq_sidebar_test_h_
#include <konqsidebarplugin.h>
#include <qlabel.h>
#include <qlayout.h>
#include <kparts/part.h>
#include <kparts/factory.h>
#include <kparts/browserextension.h>
#include <kdialogbase.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <klocale.h>

class KonqSidebarTree;

class KonqSidebar_Tree: public KonqSidebarPlugin
        {
                Q_OBJECT
                public:
                KonqSidebar_Tree(QObject *parent,QWidget *widgetParent, QString &desktopName_, const char* name=0);
                ~KonqSidebar_Tree();
                virtual void *provides(const QString &);
		void emitStatusBarText (const QString &);
                virtual QWidget *getWidget();
	        void enableActions( bool copy, bool cut, bool paste,
                        bool trash, bool del, bool shred,
                        bool rename = false );
                protected:
                        class KonqSidebarTree *tree;
                        virtual void handleURL(const KURL &url);
		protected slots:
			void copy();
			void cut();
			void paste();
			void trash();
			void del();
			void shred();
			void rename();
#undef signals
#define signals public
signals:
#undef signals
#define signals protected
			void openURLRequest( const KURL &url, const KParts::URLArgs &args = KParts::URLArgs() );
  			void createNewWindow( const KURL &url, const KParts::URLArgs &args = KParts::URLArgs() );
			void popupMenu( const QPoint &global, const KURL &url,
					const QString &mimeType, mode_t mode = (mode_t)-1 );
			void popupMenu( const QPoint &global, const KFileItemList &items );
			void enableAction( const char * name, bool enabled );
        };


class KonqSidebarTreeSelectionDialog : public KDialogBase
{
	Q_OBJECT
	public:
	KonqSidebarTreeSelectionDialog(QWidget *parent,const QStringList &list):
    		KDialogBase( parent, "konqsidebartreeselectiondialog", true,i18n("Select Type"), Ok|Cancel, 
			Ok, true ),list_(list)
  		{
    			QWidget *page = new QWidget( this );
 			setMainWidget(page);
 			QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() );
			QLabel *label = new QLabel( i18n("Select type"), page, "caption" );
			topLayout->addWidget( label );
			cb=new QComboBox(page);
			cb->setMinimumWidth(fontMetrics().maxWidth()*20);
			cb->insertStringList(list);
			cb->setEditable(true);
			topLayout->addWidget( cb );
			topLayout->addStretch(10);
		}
		int  getValue()
		{
			return list_.findIndex(cb->currentText());
		}		
		~KonqSidebarTreeSelectionDialog(){;}
	private:
		QComboBox *cb;
		QStringList list_;
};

#endif
