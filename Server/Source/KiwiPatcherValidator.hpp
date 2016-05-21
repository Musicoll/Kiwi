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

#ifndef KIWI_PATCHER_VALIDATOR_HPP_INCLUDED
#define KIWI_PATCHER_VALIDATOR_HPP_INCLUDED

#include <flip/DocumentValidator.h>

#include <KiwiModel/KiwiPatcher.hpp>

namespace kiwi
{
    namespace server
    {
        class PatcherValidator final: public flip::DocumentValidator<model::Patcher>
        {
        public:
            PatcherValidator() = default;
            
            void validate (model::Patcher & root) final override;
            
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
