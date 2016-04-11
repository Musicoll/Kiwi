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

#include "KiwiId.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object is an abstract base class for kiwi objects.
        //! @details objects can be instantiated in a Patcher.
        class Object : public flip::Object
        {
        public:
            
            struct initInfos
            {
                const std::string         name;
                const std::string         text;
                const std::vector<Atom>   args;
                
                initInfos(std::string const& object_name, std::string const& object_text) :
                name(object_name),
                text(object_text),
                args(AtomHelper::parse(text)) {}
            };
            
            //! @internal flip Default constructor
            Object(flip::Default&) {}
            
            //! @brief Constructor.
            Object(initInfos const& infos);
            
            //! @brief Destructor.
            virtual ~Object();
            
            //! @internal flip static declare method
            template<class TModel> static void declare()
            {
                if(TModel::template has<model::Object>()) return;
                
                TModel::template declare<model::Object>()
                .name("cicm.kiwi.Object")
                .template member<flip::String, &Object::m_name>("name")
                .template member<flip::String, &Object::m_text>("text")
                .template member<flip::Int, &Object::m_inlets>("inlets")
                .template member<flip::Int, &Object::m_outlets>("outlets");
            }
            
            //! @brief Returns the Object Id
            inline ObjectId getId() const {return this;}
            
            //! @brief Returns the name of the Object.
            //! @return The name of the Object.
            inline std::string getName() const     { return m_name; }
            
            //! @brief Returns the text of the Object.
            //! @return The text of the Object.
            inline std::string getText() const     { return m_text; }
            
            //! @brief Returns the number of inlets.
            //! @return The number of inlets.
            inline uint32_t getNumberOfInlets() const noexcept
            {
                return static_cast<uint32_t>(m_inlets);
            }
            
            //! @brief Returns the number of outlets.
            //! @return The number of outlets.
            inline uint32_t getNumberOfOutlets() const noexcept
            {
                return static_cast<uint32_t>(m_outlets);
            }
            
            //! @brief Adds an inlet to the Object.
            //! @param type The type of input this inlet accepts.
            void addInlet() { m_inlets += 1; }
            
            //! @brief Removes the rightmost inlet of the Object.
            void removeInlet() { if(m_inlets > 0) m_inlets -= 1; }
            
            //! @brief Adds an outlet to the Object.
            //! @param type The type of output this outlet accepts.
            void addOutlet() { m_outlets += 1; }
            
            //! @brief Removes the rightmost outlet of the Object.
            void removeOutlet() { if(m_outlets > 0) m_outlets -= 1; }
            
        private:
            flip::String    m_name;
            flip::String    m_text;
            flip::Int       m_inlets;
            flip::Int       m_outlets;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
