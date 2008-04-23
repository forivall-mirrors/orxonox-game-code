#include <iostream>
#include <string>

#include "ConsoleCommand.h"
#include "Debug.h"
#include "cpptcl/CppTcl.h"
#include "TclBind.h"

namespace orxonox
{
    ConsoleCommandShortcutExtern(test, AccessLevel::None);
    ConsoleCommandShortcutExtern(tcl, AccessLevel::None);

    void hello()
    {
        std::cout << "Hello C++/Tcl!" << std::endl;
    }

    void test()
    {
        Tcl::interpreter i;
        i.def("hello", hello);

        std::string script = "for {set i 0} {$i != 4} {incr i} { hello }";

        i.eval(script);
    }

    std::string tcl(const std::string& tclcode)
    {
        try
        {
            static Tcl::interpreter i;
            std::string output = i.eval(tclcode);
            COUT(0) << "tcl> " << output << std::endl;
            return output;
        }
        catch (Tcl::tcl_error const &e)
        {
            COUT(1) << "Error: Tcl: " << e.what() << std::endl;
        }
        catch (std::exception const &e)
        {
            COUT(1) << "Error while executing tcl: " << e.what() << std::endl;
        }

        return "";
    }
}
