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

#ifndef __DEF_KIWI_MODELS_OBJECT__
#define __DEF_KIWI_MODELS_OBJECT__

#include "KiwiFlipAttr.hpp"

namespace kiwi
{
    class Patcher;
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    //! The object is a graphical object.
    /**
     The object is a graphical class that aims to be instantiated in a patcher.
     */
    class Object :  public flip::Object,
                    public Attribute::Manager
    {
    public:
        friend class Patcher;
        
    private:
        flip::String    m_name;
        flip::String    m_text;
        flip::Int       m_id;
        
    public:
        
        Object() = default;
                
        //! @brief Constructor.
        Object(std::string const& name, std::string const& text, const int64_t id);
        
        //! @brief Copy constructor.
        Object(const Object& rhs) noexcept;
        
        //! @brief Destructor.
        virtual ~Object() noexcept;
        
        //! @internal flip static declare method
        template<class TModel>
        static void declare()
        {
            if(TModel::template has<Object>()) return;
            
            TModel::template declare<Object>()
            .template name("cicm.kiwi.Object")
            .template member<flip::String, &Object::m_name>("name")
            .template member<flip::String, &Object::m_text>("text")
            .template member<flip::Int, &Object::m_id>("id");
        }
        
    public:
        
        //! Retrieve the patcher that manages the object.
        /** The function retrieves the patcher that manages the object.
         @return The patcher that manages the object.
         */
        inline Patcher* getPatcher()
        {
            Patcher* patcher = parent().ptr<Patcher>();
            return patcher;
        }
        
        //! Retrieve the name of the object.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        inline std::string getName() const noexcept { return m_name; }
        
        //! Retrieve the text of the object.
        /** The function retrieves the text of the object.
         @return The text of the object.
         */
        inline std::string getText() const noexcept { return m_text; }
        
        //! Retrieve the id of the object.
        /** The function retrieves the id of the object.
         @return The id of the object.
         */
        inline int64_t getId() const noexcept       { return m_id; }
                
        //! Retrieves if the box should be hidden when the patcher is locked.
        /** The function retrieves if the box should be hidden when the patcher is locked.
         @return True if the box should be hidden when the patcher is locked, false otherwise.
         */
        inline bool isHiddenOnLock() const noexcept
        {
            //return getAttributeValue("hidden").getInt();
            return true;
        }
        
        //! Retrieve if the box should be displayed in presentation.
        /** The function retrieves if the box should be displayed in presentation.
         @return True if the box should be displayed in presentation, otherwise false.
         */
        inline bool isIncludeInPresentation() const noexcept
        {
            //return getAttributeValue("presentation").getInt();
            return true;
        }
        
        //! Retrieve the "ignoreclick" attribute value of the box.
        /** The function retrieves the "ignoreclick" attribute value of the box.
         @return The "ignoreclick" attribute value of the box.
         */
        inline bool getIgnoreClick() const noexcept
        {
            //return getAttributeValue("ignoreclick").getInt();
            return true;
        }
    };
}


#endif
