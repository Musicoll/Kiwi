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

#include <functional>
#include <memory>
#include <map>

#include <KiwiModel/KiwiModel_Object.h>

#include <KiwiModel/KiwiModel_Factory.h>

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
        
        using ctor_fn_t = std::function<std::unique_ptr<ObjectView>(model::Object & model)>;
        
    public: // methods

        //! @brief The construction methods.
        //! @details Returns an object corresponding to a certain object's model.
        static std::unique_ptr<ObjectView> createObjectView(model::Object & object_model);
        
        //! @brief Adds a object to the factory.
        template<class TObject>
        static void add(std::string const& name, ctor_fn_t create_method)
        {
            static_assert(std::is_base_of<ObjectView, TObject>::value,
                          "object's view doesn't inherit from class ObjectView");
            
            static_assert(!std::is_abstract<TObject>::value,
                          "The object's view must not be abstract.");
            
            assert(model::Factory::has(name) && "Adding an engine object that has no corresponding model");
            assert(m_creators.count(name) == 0 && "The object already exists");
            
            m_creators[name] = create_method;
        };
        
        static std::map<std::string, ctor_fn_t> m_creators;

    private: // deleted methods
        
        Factory() = delete;
        ~Factory() = delete;
        Factory(Factory const& other) = delete;
        Factory(Factory && other) = delete;
        Factory& operator=(Factory const& other) = delete;
        Factory& operator=(Factory && other) = delete;
    };
}
