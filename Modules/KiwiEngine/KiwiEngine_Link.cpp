/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiEngine_Link.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        Link::Link(Object & receiver, size_t index) :
        m_receiver(receiver),
        m_index(index)
        {
            ;
        }
        
        bool Link::operator<(Link const& other) const
        {
            return (&m_receiver < &other.m_receiver
                    || (&m_receiver == &other.m_receiver && m_index < other.m_index));
        }
 
        Object & Link::getReceiver() const
        {
            return m_receiver;
        }
        
        size_t Link::getReceiverIndex() const
        {
            return m_index;
        }
    }
}
