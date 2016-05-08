/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_CORE_CONSOLE_HPP_INCLUDED
#define KIWI_CORE_CONSOLE_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <mutex>
#include <string>

namespace kiwi
{
    class Console
    {
    public:
        
        template<class Arg, class ...Args>
        static void post(Arg&& arg, Args&&... args)
        {
            output(std::cout, "> ", std::forward<Arg>(arg), std::forward<Args>(args)...);
        }
        
        template<class Arg, class ...Args>
        static void error(Arg&& arg, Args&&... args)
        {
            output(std::cerr, "- error: ", std::forward<Arg>(arg), std::forward<Args>(args)...);
        }
        
        template<class Arg, class ...Args>
        static void log(Arg&& arg, Args&&... args)
        {
            output(std::clog, "- log: ", std::forward<Arg>(arg), std::forward<Args>(args)...);
        }
        
    private:
        
        template<class ...Args>
        static void output(std::ostream& output_stream, Args&&... args)
        {
            std::ostringstream oss;
            
            // braced-init-list expansion trick
            using expander = int[];
            (void) expander{ (oss << std::forward<Args>(args), void(), 0)... };

            std::lock_guard<std::mutex> guard(m_mutex);
            output_stream << oss.str() << '\n';
        }
        
        Console() = delete;
        
        static std::mutex m_mutex;
    };
}

#endif // KIWI_CORE_CONSOLE_HPP_INCLUDED
