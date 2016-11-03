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

#ifndef KIWI_PATCHER_VALIDATOR_HPP_INCLUDED
#define KIWI_PATCHER_VALIDATOR_HPP_INCLUDED

#include <flip/DocumentValidator.h>

#include <KiwiModel/KiwiModel_PatcherUser.hpp>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                 PATCHER VALIDATOR                                //
        // ================================================================================ //
        
        class PatcherValidator final: public flip::DocumentValidator<model::Patcher>
        {
        public:
            PatcherValidator() = default;
            
            //! @brief Called to validate a new transaction
            void validate(model::Patcher & root) final override;
            
            ~PatcherValidator() = default;
            
        private:
            PatcherValidator(PatcherValidator const& other) = delete;
            PatcherValidator(PatcherValidator && other) = delete;
            PatcherValidator &operator=(PatcherValidator const& other) = delete;
            PatcherValidator &operator=(PatcherValidator && other) = delete;
        };
    }
}

#endif //KIWI_PATCHER_VALIDATOR_HPP_INCLUDED
