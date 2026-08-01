#include <string.h>

#include "merc.h"
#include "pstring.h"
#include "db.h"

char *palloc_string(const char *str)
{
	if (str == nullptr)
	{
		RS.Logger.Warn("Bad code form that'll lead to free_pstr() crashes: attempting to palloc_string a nullptr.");
		return nullptr;
	}

	int slen = strlen(str) + 1;

	nAllocString++;

	char *tstr = new char[slen];

	strcpy(tstr, str);

	return tstr;
}

void free_pstring(char *pstr)
{
	if (pstr == nullptr
         ||   pstr == &str_empty[0])
                return;
	nAllocString--;
	delete[] pstr;
}
