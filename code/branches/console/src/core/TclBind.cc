#include <iostream>
#include <string>

#include "ConsoleCommand.h"
#include "CommandExecutor.h"
#include "Debug.h"
#include "TclBind.h"

namespace orxonox
{
    ConsoleCommandShortcutExtern(tcl, AccessLevel::None);

    void Tcl_puts(Tcl::object const &args)
    {
        COUT(0) << args.get() << std::endl;
    }

    std::string Tcl_unknown(Tcl::object const &a)
    {
std::cout << "1\n";
std::cout << a.get() << std::endl;
        CommandEvaluation evaluation = CommandExecutor::evaluate(std::string(a.get()));
std::cout << "2\n";
        CommandExecutor::execute(evaluation);
std::cout << "3\n";
        if (evaluation.hasReturnvalue())
        {
std::cout << "4\n";
            return evaluation.getReturnvalue().toString();
        }
std::cout << "5\n";
        return "";
    }

    std::string tcl(const std::string& tclcode)
    {
        try
        {
            static Tcl::interpreter i;
            i.def("puts", Tcl_puts, Tcl::variadic());
            i.def("blub", Tcl_unknown, Tcl::variadic());
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
