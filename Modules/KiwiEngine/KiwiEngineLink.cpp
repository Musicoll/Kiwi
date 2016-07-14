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

#include "KiwiEngineLink.hpp"
#include "KiwiEngineObject.hpp"

#include <KiwiModel/KiwiModelLink.hpp>
#include <KiwiModel/KiwiModelObject.hpp>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        Link::Link(model::Link const& model) :
        m_model(model)
        {
            ;
        }
        
        Object& Link::getSenderObject() const
        {
            return *m_model.getSenderObject().entity().use<std::shared_ptr<engine::Object>>().get();
        }
 
        Object& Link::getReceiverObject() const
        {
            return *m_model.getReceiverObject().entity().use<std::shared_ptr<engine::Object>>().get();
        }
        
        size_t Link::getSenderIndex() const
        {
            return m_model.getSenderIndex();
        }
        
        size_t Link::getReceiverIndex() const
        {
            return m_model.getReceiverIndex();
        }
    }
}
