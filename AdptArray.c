
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"

typedef struct AdptArray_{
	PElement *ArrElements;
	int size;
	DEL_FUNC delFunc;
	COPY_FUNC copyFunc;
	PRINT_FUNC printFunc;

}AdptArray;

// Create a new array of size New and copy the old array to the new array and release the memory of the old array.
PAdptArray copy_adpt_array(PAdptArray original,int sizeNew){
	PElement* org = (PElement)original->ArrElements;
	PElement* cp = (PElement)malloc(sizeof(PElement)*sizeNew);
	int i;	
	memset(cp,0,sizeof(PElement)*sizeNew);
	for (i = 0; i < GetAdptArraySize(original); i++)
		{
			if (org[i]){
				cp[i] = original->copyFunc(org[i]);
				original->delFunc(org[i]);
			}
		}
	free(original->ArrElements);
	original->ArrElements = cp;
	original->size = sizeNew;
	return original;
}

// Create a struct of type PAdptArray that initializes an empty array, array size, the 3 functions for deleting, copying and deleting
PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc,PRINT_FUNC printFunc){
	PAdptArray cp = (PAdptArray)malloc(sizeof(AdptArray));
	if(!cp){ return NULL;}
	cp->ArrElements = (PElement)malloc(sizeof(PElement)*1);
	cp->size = 1;
	memset(cp->ArrElements,0,sizeof(PElement)*1);
	cp->delFunc=delFunc;
	cp->copyFunc=copyFunc;
	cp->printFunc=printFunc;
	return cp;
}

// Delete the elements in the array and free the memory of the object
void DeleteAdptArray(PAdptArray cp){
	int i;
	if (NULL != cp){
		for (i = 0; i < GetAdptArraySize(cp); i++)
		{
			if (cp->ArrElements[i]){
				cp->delFunc(cp->ArrElements[i]);
			}	
		}
		free(cp->ArrElements);
		free(cp);
	}
}

// Receives an index and a member and saves a copy of the member in the requested location. Frees the old member if it exists.
Result SetAdptArrayAt(PAdptArray cp, int index, PElement element){
	if (NULL == cp || NULL == element || index < 0){
		return FAIL;
	}
	if (GetAdptArraySize(cp) <= index){
		copy_adpt_array(cp,index+1);
		cp->ArrElements[index] = cp->copyFunc(element);
		return SUCCESS;
	}else{
		if (cp->ArrElements[index]){
			cp->delFunc(cp->ArrElements[index]);
		}
		cp->ArrElements[index] = cp->copyFunc(element);
		return SUCCESS;
	}
}

// Gets an index and returns a copy of the member at this location
PElement GetAdptArrayAt(PAdptArray cp, int index){
	if (NULL == cp || index < 0 || GetAdptArraySize(cp) <= index){
		return NULL;
	}
	if(cp->ArrElements[index]){
		return cp->copyFunc(cp->ArrElements[index]);
	}else{
		return NULL;
	}
}

// Returns the size of the array
int GetAdptArraySize(PAdptArray cp){
	if (NULL == cp ){
		return 0;
	}
	return cp->size;
}

// Prints the elements in the array.
void PrintDB(PAdptArray cp){
	int i;
	if (NULL != cp ){
		for (i = 0; i < GetAdptArraySize(cp); i++)
		{
			if (cp->ArrElements[i]){
				cp->printFunc(cp->ArrElements[i]);
			}
		}
	}else{
		printf("error");
	}
}