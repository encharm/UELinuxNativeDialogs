// Author: Damian Kaczmarek <damian@codecharm.co.uk>

#ifndef UEFILEDIALOG_H
#define UEFILEDIALOG_H

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
	UFileDialogActionSave = 1
} UFileDialogActionType;

typedef enum
{
	// option sets:
	UFileDialogSingleSelection = 0,
	UFileDialogSingleDirectorySelection = 1,
	UFileDialogMultipleSelection = 2
} UFileDialogSelectionType;

typedef enum
{
	UFileDialogPositionAuto = 0,
	UFileDialogPositionSuggest = 1
} UFileDialogPositionType;

struct UFileDialogHints
{
	
	// things to consider: icon, window title (language?)
	UFileDialogActionType Action;

	UFileDialogSelectionType Selection;

	//! for example "All C++ files (*.cpp *.cc *.C *.cxx *.c++)""
	const char* filter;
	
	UFileDialogPositionType Position;
	int SuggestedWindowX;
	int SuggestedWindowY;
};

#define DEFAULT_UFILEDIALOGHINTS { \
	UFileDialogActionOpen, \
	UFileDialogSingleSelection, \
	"All files (*.*)", \
	UFileDialogPositionAuto, \
	0, \
	0  \
}


typedef struct UFileDialogResult
{
/* utility helpers for C++ implementors and usage */
#ifdef __cplusplus
private:
	// prevent copy constructor
	UFileDialogResult(const UFileDialogResult&) {};
public:
	UFileDialogResult() : count(0), selection(0) {}

	~UFileDialogResult()
	{
		for(int i = 0;i < count;++i)
		{
			if(selection[i])
				free((void*)selection[i]);
		}
		if(selection)
			free(selection);
	}

	bool isValid() const
	{
		return count;
	}

	const char* single() const
	{
		return isValid() ? selection[0] : "";
	}
#endif

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