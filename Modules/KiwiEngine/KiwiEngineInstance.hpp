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

#ifndef KIWI_ENGINE_INSTANCE_HPP_INCLUDED
#define KIWI_ENGINE_INSTANCE_HPP_INCLUDED

#include "flip/Document.h"

#include "KiwiEnginePatcher.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        //! @brief The Instance adds the engine objects to the engine::Factory
        //! @details The patcher document model must be initialised before creating the Instance
        class Instance
        {
        public: // methods
            
            //! @brief Constructs an Instance and adds the engine objects to the engine::Factory.
            Instance();
            
            //! @brief Destructor.
            ~Instance();
            
        private: // methods
            
            //! @internal Adds the engine objects to the engine::Factory
            void addObjectsToFactory();
        };
    }
}


#endif // KIWI_ENGINE_INSTANCE_HPP_INCLUDED
