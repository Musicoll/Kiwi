/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiModel_PatcherValidator.h"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   PATCHER VALIDATOR                              //
        // ================================================================================ //
        
        void PatcherValidator::validate(Patcher & patcher)
        {
            if (patcher.changed())
            {
                if (patcher.objectsChanged())
                {
                    for(Object const& object : patcher.getObjects())
                    {
                        if(object.removed())
                        {
                            objectRemoved(object, patcher);
                        }
                    }
                }
                
                if (patcher.linksChanged())
                {
                    for(Link const& link : patcher.getLinks())
                    {
                        if(link.added())
                        {
                            linkAdded(link);
                        }
                    }
                }
            }
        }
        
        void PatcherValidator::objectRemoved(Object const& object, Patcher const& patcher) const
        {
            for(Link const& link : patcher.getLinks())
            {
                if((link.getSenderObject().ref() == object.ref() || link.getReceiverObject().ref() == object.ref())
                   && !link.removed())
                {
                    flip_VALIDATION_FAILED ("Removing object without removing its links");
                }
            }
        }
        
        void PatcherValidator::linkAdded(Link const& link) const
        {
            if(!link.isSenderValid() || !link.isReceiverValid())
            {
                flip_VALIDATION_FAILED ("Creating link to non existing object");
            }
            
            PinType outlet_type = link.getSenderObject().getOutlet(link.getSenderIndex()).getType();
            
            if (!link.getReceiverObject().getInlet(link.getReceiverIndex()).hasType(outlet_type))
            {
                flip_VALIDATION_FAILED("Link creation type mismatch between outlet and inlet");
            }
        }
    }
}
