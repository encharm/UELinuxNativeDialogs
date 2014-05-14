#include <stdlib.h>
#include <gtk/gtk.h>
#include "UFileDialog.h"

struct UFileDialog
{
  GtkWidget* dialog;
  int gtk_response;
  UFileDialogResult result;
};


static void file_selected(GtkDialog *dialog, gint response_id, gpointer user_data)
{
  ((UFileDialog*)user_data)->gtk_response = response_id;
}

struct UFileDialog* UFileDialog_Create(struct UFileDialogHints *hints)
{
  if(!hints)
  {
    return NULL;
  }

  if(!gtk_init_check(0, NULL)) {
    return NULL;
  }

  UFileDialog* uedialog = calloc(1, sizeof(UFileDialog));

  uedialog->dialog = gtk_file_chooser_dialog_new ("Open File",
              NULL,
              GTK_FILE_CHOOSER_ACTION_OPEN,
              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
              GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
              NULL);
 
  g_signal_connect (G_OBJECT (uedialog->dialog), "response",
    G_CALLBACK (file_selected),
    uedialog);

  gtk_widget_show(uedialog->dialog);

  return uedialog;
}




bool UFileDialog_ProcessEvents(UFileDialog* handle)
{

  if(!GTK_IS_FILE_CHOOSER(handle->dialog))
  {
    // widget is destroyed
    return false;
  }

//  printf("resposne %i\n", gtk_dialog_get_response_for_widget(handle->dialog));

  if(handle->gtk_response) {
    printf("Got response: %i\n", handle->gtk_response);

    switch(handle->gtk_response) {
      case GTK_RESPONSE_ACCEPT:
      {
        printf("Widget selected\n");

        //bo gtk_file_chooser_get_filenames
        GSList *elem;
        GSList* list = gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER (handle->dialog));
        int i = 0;
        handle->result.selection = (const char**)malloc(sizeof(const char*) * g_slist_length(list));
        handle->result.count = g_slist_length(list);
        for(elem = list; elem; elem = elem->next) {
          handle->result.selection[i] = strdup(elem->data);
          i++;
        }
        g_slist_foreach(list, (GFunc)g_free, NULL);
        g_slist_free(list);
      }
      break;
      case GTK_RESPONSE_DELETE_EVENT:
      case GTK_RESPONSE_CANCEL:
      case GTK_RESPONSE_REJECT:
      {
        printf("Widget rejected\n");

      }
      break;
      default:
      ;
    }
  
    return false;
  }


  return gtk_main_iteration_do(false);
}

const UFileDialogResult* UFileDialog_Result(UFileDialog* handle)
{
  if(!handle)
    return NULL;
  return &(handle->result);
}

void UFileDialog_Destroy(UFileDialog* handle)
{
  int i;
  if(GTK_IS_FILE_CHOOSER(handle->dialog))
  {
    gtk_widget_destroy (handle->dialog);
  }

  if(handle->result.selection) {
    for(i = 0;i < handle->result.count;++i) {
      free((char*)handle->result.selection[i]);
    }
    free(handle->result.selection);
  }
  free(handle);
}
