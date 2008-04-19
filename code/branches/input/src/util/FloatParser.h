//FloatParser.h

#ifndef _FloatParser_H__
#define _FloatParser_H__

#include "UtilPrereqs.h"

//Endzeichen für float expression: ')', '}', ']', ',', ';'
_UtilExport bool parse_float(char* const, char**, double*);
//Endzeichen angegeben
_UtilExport bool parse_float(char* const, char**, char, double*);
//Letzter Teil-float eines Vektors parsen (keine Vergleichs- und Logikoperationen)
_UtilExport bool parse_vector_float(char* const, char**, bool, double*);

#endif /* _FloatParser_H__ */