//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//


#ifndef _NORMEANTISISMICHE_H_
#define _NORMEANTISISMICHE_H_

#include "normeAntisismiche-io.h"

/* a type renaming */
typedef int Boolean;
class Entry;
typedef Entry *Symbol;

Boolean copy_Boolean(Boolean);
void assert_Boolean(Boolean);
void dump_Boolean(ostream &,int,Boolean);

Symbol copy_Symbol(Symbol);
void assert_Symbol(Symbol);
void dump_Symbol(ostream &,int,Symbol);

#endif
