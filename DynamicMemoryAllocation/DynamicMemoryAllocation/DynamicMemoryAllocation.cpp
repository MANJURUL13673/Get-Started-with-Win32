#include <Windows.h>
#include <iostream>
#include <cstdlib>

typedef int ElementType;

typedef struct _MyObject
{
	// the count of elements
	int count;

	// this elements array should be the last field
	// of this struct
	ElementType elements[1];
} MyObject, *PMYObject;

void TestDynamicAllocation()
{
	int count = 5;
	size_t byte_count = sizeof(MyObject) + // +1 element
		(count - 1) * sizeof(ElementType); // count - 1 elements
	// 1 element from MyObject
	// and (count-1) elements
	// which makes total count elements and one MyObject

	// allocate one MyObject with count (5) elemets
	PMYObject pMyObject = (PMYObject)malloc(byte_count);
	pMyObject->count = count;

	// user pMyObject
	for (int i = 0; i < pMyObject->count; ++i)
	{
		pMyObject->elements[i] = (i + 1);
		std::cout << i << " : " << pMyObject->elements[i] << std::endl;
	}

	//free pMyObject after use
	free(pMyObject);
}

// if successful, returns non-zero value (of TRUE)
// if fails, returns value zero (or FALSE)
BOOL WinAPIPattern(PMYObject* ppMyObject, int count, int* size)
{
	if (count == 0 && size != NULL)
	{
		// the element count is determined dynamically
		// by windows API function
		*size = 5;

		return TRUE;
	}
	else if (ppMyObject != NULL && count > 0)
	{
		size_t byte_count = sizeof(MyObject) + // +1 element
			(count - 1) * sizeof(ElementType); // count - 1 elements

		MyObject* ptr = (MyObject*)malloc(byte_count);
		if (ptr != NULL)
		{
			// allocation successful
			ptr->count = count;

			// fill the elements
			for (int i = 0; i < ptr->count; ++i)
			{
				ptr->elements[i] = (i + 1);
			}

			*ppMyObject = ptr;

			if (size != NULL)
			{
				*size = count;
			}

			return TRUE;
		}
		else
		{
			*ppMyObject = NULL;
			if (size != NULL)
			{
				*size = 0;
			}
			return FALSE;
		}
	}
	else
	{
		if (ppMyObject != NULL)
		{
			*ppMyObject = NULL;
		}
		if (size != NULL)
		{
			*size = 0;
		}

		return FALSE;
	}
}

void TestWinAPIPattern()
{
	int count, size;

	//STEP 1: retrieve element count
	if (!WinAPIPattern(NULL, 0, &count))
	{
		std::cout << "Failed to get element count" << std::endl;
		return;
	}

	PMYObject pMyObject;
	//STEP 2: allocate count elements
	if (!WinAPIPattern(&pMyObject, count, &size))
	{
		std::cout << "Failed to allocate " << count << " elements of MyObject" << std::endl;
		return;
	}

	//user pMyObject
	for (int i = 0; i < pMyObject->count; ++i)
	{
		std::cout << i << " : " << pMyObject->elements[i] << std::endl;
	}
	
	//Don't forget to free the memory
	free(pMyObject);
}

int main()
{
	//TestDynamicAllocation();
	TestWinAPIPattern();
}