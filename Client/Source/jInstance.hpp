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

#include <KiwiEngine/KiwiInstance.hpp>

#include "flip/Document.h"

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    //! @brief The main DocumentObserver.
    //! @details The jInstance dispatchs changes to other DocumentObserver objects
    class jInstance : public flip::DocumentObserver<model::Patcher>
    {
    public:
        jInstance();
        ~jInstance();
        
        void newPatcherDocument();
        
    private:
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void document_changed(model::Patcher& patcher) final;
        
        std::unique_ptr<engine::Instance>               m_instance;
        
        std::vector<std::unique_ptr<flip::Document>>    m_documents;
    };
}