#include "UFileDialog.h"

#include <QApplication>
#include <QFileDialog>


struct UFileDialog : public QApplication
{
private:
  static int argc;
public:
  QFileDialog dialog;
  UFileDialog() : QApplication(argc, NULL)
  {
    dialog.show();
  }
  UFileDialogResult result;
};
int UFileDialog::argc;

UFileDialog* UFileDialog_Create(UFileDialogHints *hints)
{
  if(hints == NULL)
  {
    return NULL;
  }

  int argc = 0;
  UFileDialog* dialog = new UFileDialog();
  
  return dialog;
}

bool UFileDialog_ProcessEvents(UFileDialog* handle)
{
  handle->processEvents();
  int result = handle->dialog.result();
  if(result != 0) {

    QStringList selectedFiles = handle->dialog.selectedFiles();

    handle->result.count = selectedFiles.size();
    handle->result.selection = (const char**)malloc(sizeof(const char*) * selectedFiles.size());
    QStringList::const_iterator constIterator;
    int i = 0;
    for(constIterator = selectedFiles.begin(); constIterator != selectedFiles.end();++constIterator)
    {
      handle->result.selection[i] = strdup((*constIterator).toStdString().c_str());
    }

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
