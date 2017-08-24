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

#include <functional>
#include <memory>
#include <map>

#include <KiwiModel/KiwiModel_Object.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

#pragma once

namespace kiwi
{
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //

    class Factory
    {
    public: // definitions
        
        using factory_func = std::function<std::unique_ptr<ObjectView>(model::Object & model)>;
        
    public: // methods

        //! @brief The construction methods.
        //! @details Returns an object corresponding to a certain object's model.
        static std::unique_ptr<ObjectView> createObjectView(model::Object & object_model);
        
        //! @brief Initializes the list of creators adding a function for each type of objects.
        static void initialise();
        
    private: // members
        
        static std::map<std::string, factory_func> m_creator_map;

    private: // deleted methods
        
        Factory() = delete;
        ~Factory() = delete;
        Factory(Factory const& other) = delete;
        Factory(Factory && other) = delete;
        Factory& operator=(Factory const& other) = delete;
        Factory& operator=(Factory && other) = delete;
    };
}
