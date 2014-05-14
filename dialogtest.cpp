#include "UFileDialog.h"
#include <cstdio>
#include <unistd.h>

int main(void)
{
  struct UFileDialogHints hints = DEFAULT_UFILEDIALOGHINTS;

  UFileDialog* dialog = UFileDialog_Create(&hints);

  while(UFileDialog_ProcessEvents(dialog)) {
    usleep(5000);
  }

  printf("Result: %s\n", UFileDialog_Result(dialog)->single());
     
  UFileDialog_Destroy(dialog);
  return 0;
}