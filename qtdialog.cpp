#include "UFileDialog.h"

#include <QApplication>
#include <QFileDialog>


// KDE integration fails after QApplication is destroyed
// leave it as singleton and lazy initialize
//static QApplication* qApp;

struct UFileDialog
{
public:
  QFileDialog dialog;
  UFileDialog()
  {
    result.count = 0;
    result.selection = NULL;
  }
  virtual ~UFileDialog() {

    for(int i = 0;i < result.count;++i)
    {
      if(result.selection[i])
        free((void*)result.selection[i]);
    }
    if(result.selection)
      free(result.selection);
  }
  UFileDialogResult result;
};

UFileDialog* UFileDialog_Create(UFileDialogHints *hints)
{
  /*
    FAT NOTE. This plugin shouldn't be unloaded as things
    could break. Doing multiple QApplication initializations
    and destructions as long as there is only one at a time
    alive is perfectly valid for Qt but unfortunately KDE
    integration is not as forthcoming and a single instance
    of QApplication needs to be left alive
    */
  if(!qApp)
  { //lazy initialize QApplication
    static int argc;
    new QApplication(argc, NULL);
  }

  if(hints == NULL)
  {
    return NULL;
  }

  UFileDialog* dialog = new UFileDialog();
  
  if(hints->InitialDirectory)
  {
    dialog->dialog.setDirectory(hints->InitialDirectory);
  }

  if(hints->WindowTitle)
  {
    dialog->dialog.setWindowTitle(hints->WindowTitle);
  }

  if(hints->NameFilter)
  {
    dialog->dialog.setNameFilter(hints->NameFilter);
  }

  switch(hints->Action)
  {
    case UFileDialogActionOpen:
      printf("Set action open\n");
      dialog->dialog.setAcceptMode(QFileDialog::AcceptOpen);
      dialog->dialog.setFileMode(QFileDialog::ExistingFile);
      break;
    case UFileDialogActionOpenMultiple:
      printf("Set action open multiple\n");
      dialog->dialog.setAcceptMode(QFileDialog::AcceptOpen);
      dialog->dialog.setFileMode(QFileDialog::ExistingFiles);
      break;
    case UFileDialogActionOpenDirectory:
      printf("Set action open dir\n");
      dialog->dialog.setAcceptMode(QFileDialog::AcceptOpen);
      dialog->dialog.setFileMode(QFileDialog::Directory);
      break;
    case UFileDialogActionSave:
      printf("Set action save dir\n");
      dialog->dialog.setAcceptMode(QFileDialog::AcceptSave);
      dialog->dialog.setFileMode(QFileDialog::AnyFile);
      break;
  }
  dialog->dialog.show();
  return dialog;
}

bool UFileDialog_ProcessEvents(UFileDialog* handle)
{
  qApp->processEvents();

  int result = handle->dialog.result();
  if(result != 0)
  {
    QStringList selectedFiles = handle->dialog.selectedFiles();

    handle->result.count = selectedFiles.size();
    handle->result.selection = (const char**)malloc(sizeof(const char*) * selectedFiles.size());
    QStringList::const_iterator constIterator;
    int i = 0;
    for(constIterator = selectedFiles.begin(); constIterator != selectedFiles.end();++constIterator)
    {
      handle->result.selection[i] = strdup((*constIterator).toStdString().c_str());
      i++;
    }
    return false;
  }
  if(handle->dialog.isHidden()) // someone closed it
  {
    return false;
  }
  return true;
}

void UFileDialog_Destroy(UFileDialog* handle)
{
  if(handle)
    delete handle;
}

const UFileDialogResult* UFileDialog_Result(UFileDialog* handle)
{

  return &(handle->result);
}
