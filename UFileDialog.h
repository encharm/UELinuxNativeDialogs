// Author: Damian Kaczmarek <damian@codecharm.co.uk>

#ifndef UEFILEDIALOG_H
#define UEFILEDIALOG_H

/*
	Usage:

	struct UFileDialogHints hints = DEFAULT_UFILEDIALOGHINTS;
	<setup your hints>
	dialog = UFileDialog_Create(hints);

	and in the event loop call:

	bool status = UFileDialog_ProcessEvents(dialog);
	if(!status) {
		const UFileDialogResult* result =  UFileDialog_Result(dialog);
		<do something with the result>
		UFileDialog_Destroy(dialog);
	}
*/


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct UFileDialog;
typedef struct UFileDialog UFileDialog;

typedef enum
{
	UFileDialogActionOpen = 0,
	UFileDialogActionOpenMultiple = 1,
	UFileDialogActionOpenDirectory = 2,
	UFileDialogActionSave = 3
} UFileDialogActionType;


struct UFileDialogHints
{
	UFileDialogActionType Action;

	//! (optional) for example "All C++ files (*.cpp *.cc *.C *.cxx *.c++)""
	const char* NameFilter;
	//! (optional) absolute directory from which the dialog whould open
	const char* InitialDirectory;
	//! (optional) title to put on open file dialog
	const char* WindowTitle;
};

#define DEFAULT_UFILEDIALOGHINTS { \
	UFileDialogActionOpen, \
	"All files (*.*)", \
	NULL, \
	NULL \
}


typedef struct UFileDialogResult
{
	int count;
	const char** selection;
} UFileDialogResult;

UFileDialog* UFileDialog_Create(struct UFileDialogHints* hints);
bool UFileDialog_ProcessEvents(UFileDialog* handle);
// valid after Process returning false
const UFileDialogResult* UFileDialog_Result(UFileDialog* handle);
void UFileDialog_Destroy(UFileDialog* handle);

#ifdef __cplusplus
}
#endif

#endif // UEFILEDIALOG_H