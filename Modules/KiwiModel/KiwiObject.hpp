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

#ifndef KIWI_MODEL_OBJECT_HPP_INCLUDED
#define KIWI_MODEL_OBJECT_HPP_INCLUDED

#include "KiwiAttribute.hpp"

namespace kiwi
{
    namespace model
    {
        class Patcher;
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object is an abstract base class for kiwi objects.
        //! @details objects can be instantiated in a Patcher.
        class Object : public Attribute::Manager
        {
        public:
            
            using Ref = flip::ObjectRef<model::Object>;
            
            //! @brief Default constructor.
            Object()
            {
                std::cout << "Object Default ctor called\n";
            }
            
            //! @brief Constructor.
            Object(std::string const& name, std::string const& text);
            
            //! @brief Destructor.
            virtual ~Object() noexcept {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<model::Object>()) return;
                
                TModel::template declare<model::Object>()
                .name("cicm.kiwi.Object")
                .template inherit<Attribute::Manager>()
                .template member<flip::String, &Object::m_name>("name")
                .template member<flip::String, &Object::m_text>("text");
            }
            
            //! @brief Returns the patcher that manages the object.
            //! @return The Patcher's pointer.
            inline Patcher* getParentPatcher() { return parent().ptr<Patcher>(); }
            
            //! @brief Returns the name of the Object.
            //! @return The name of the Object.
            inline std::string getName() const noexcept { return m_name; }
            
            //! @brief Returns the text of the Object.
            //! @return The text of the Object.
            inline std::string getText() const noexcept { return m_text; }
            
        private:
            flip::String    m_name;
            flip::String    m_text;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
