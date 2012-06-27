// -*-c++-*-


#include <qdialog.h>

class SlFileSelector;
class QVBoxLayout;
class QTERuputer;

class QTEFileDialog : public QDialog
{
  Q_OBJECT

  private:
#ifdef FILEDIALOG
	SlFileSelector *slFileSelector;
#endif
	QVBoxLayout *boxLayout;

	char const *selectFileName;

  public:
	QTEFileDialog(QTERuputer *parent);
	~QTEFileDialog(void);

	char const *getSelectFileName(void);

  protected slots:
	void accept();
};

