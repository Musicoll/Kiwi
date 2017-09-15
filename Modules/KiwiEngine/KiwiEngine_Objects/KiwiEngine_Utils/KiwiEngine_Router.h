/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <cstddef>
#include <set>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       ROUTER                                     //
        // ================================================================================ //
        
        class Router
        {
        public: // classes
            
            struct Cnx
            {
                Cnx(size_t input, size_t output);
                
                bool operator<(Cnx const& other) const;
                
                size_t m_input = 0;
                size_t m_output = 0;
            };
            
        public: // method
            
            Router() = default;
            
            void connect(size_t input_index, size_t output_index);
            
            void disconnect(size_t intput_index, size_t output_index);
            
            size_t getNumberOfConnections() const;
            
            std::set<Cnx> const& getConnections() const;
            
            ~Router() = default;
            
        private: // memebers
            
            std::set<Cnx> m_cnx;
        };
    }
}
