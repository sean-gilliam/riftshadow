#ifndef PSTRING_H
#define PSTRING_H

//
// LOCAL FUNCTIONS
//

// Permanent string allocation: a thin new char[] / delete[] pair serving the
// char * fields on the entity structs. free_pstring tolerates nullptr and the
// shared str_empty sentinel, both of which entities are initialised to.

char *palloc_string (const char *str);
void free_pstring (char *pstr);

#endif /* PSTRING_H */
