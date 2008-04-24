#ifndef _TclBind_H__
#define _TclBind_H__

#include "cpptcl/CppTcl.h"

namespace orxonox
{
    Tcl::interpreter* createNewInterpreter();
    void Tcl_puts(Tcl::object const &c);
    std::string Tcl_unknown(Tcl::object const &c);
    std::string tcl(const std::string& tclcode);
}

#endif /* _TclBind_H__ */
