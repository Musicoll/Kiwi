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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Utils/KiwiEngine_Router.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       ROUTER                                     //
        // ================================================================================ //
        
        Router::Cnx::Cnx(size_t input, size_t output):
        m_input(input),
        m_output(output)
        {
        }
        
        bool Router::Cnx::operator<(Cnx const& other) const
        {
            return m_input < other.m_input
            || (m_input == other.m_input && m_output < other.m_output);
        }
        
        void Router::connect(size_t input_index, size_t output_index)
        {
            m_cnx.insert(Cnx(input_index, output_index));
        }
        
        void Router::disconnect(size_t input_index, size_t ouptut_index)
        {
            m_cnx.erase(Cnx(input_index, ouptut_index));
        }
        
        size_t Router::getNumberOfConnections() const
        {
            return m_cnx.size();
        }
        
        std::set<Router::Cnx> const& Router::getConnections() const
        {
            return m_cnx;
        }
    }
}
