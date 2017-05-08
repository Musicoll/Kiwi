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

#include <flip/DocumentValidator.h>

#include "KiwiModel_PatcherUser.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      PATCHERVALIDATOR                            //
        // ================================================================================ //
        
        class PatcherValidator : public flip::DocumentValidator<Patcher>
        {
        public: // methods
            
            PatcherValidator() = default;
            ~PatcherValidator() = default;
            
            // @brief Validate the model before a transaction can be executed.
            virtual void validate (Patcher& patcher) override;
            
        private: // methods
            
            //! @brief Carry out checks once a object is removed.
            void objectRemoved(Object const& object, Patcher const& patcher) const;
            
            //! @brief Carry out checks once a link is created.
            void linkAdded(Link const& link) const;
            
        private: // deleted methods
            PatcherValidator(PatcherValidator const& other) = delete;
            PatcherValidator(PatcherValidator && other) = delete;
            PatcherValidator& operator=(PatcherValidator const& other) = delete;
            PatcherValidator& operator=(PatcherValidator && other) = delete;
        };
    }
}
