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

#ifndef KIWI_JINSTANCE_HPP_INCLUDED
#define KIWI_JINSTANCE_HPP_INCLUDED

#include <KiwiEngine/KiwiInstance.hpp>

#include "flip/Document.h"

#include "jWindow.hpp"
#include "jPatcher.hpp"

namespace kiwi
{
    //! @brief The main DocumentObserver.
    //! @details The jInstance dispatch changes to other DocumentObserver objects
    class jInstance : public flip::DocumentObserver<model::Patcher>
    {
    public:
        jInstance();
        ~jInstance();
        
        void newPatcher();
        
    private:
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void document_changed(model::Patcher& patcher) override final;
        
        std::unique_ptr<engine::Instance>   m_instance;
        std::unique_ptr<flip::Document>     m_document;
    };
}

#endif // KIWI_JINSTANCE_HPP_INCLUDED
