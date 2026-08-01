#include <string.h>

#include "merc.h"
#include "pstring.h"
#include "db.h"

char *palloc_string(const char *str)
{
	char *tstr;
	int slen = strlen(str) + 1;
	if(str == nullptr)
	{
		RS.Logger.Warn("Bad code form that'll lead to free_pstr() crashes: attempting to palloc_string a nullptr.");
		return nullptr;
	}
	nAllocString++;
	tstr = new char[slen];
	if(!tstr)
	{
		RS.Logger.Warn("Unable to allocate pstring.");
		return nullptr;
	}
	strcpy(tstr,str);
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
