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

#include "KiwiFlipAttr.h"

namespace kiwi
{
    // ================================================================================ //
    //                                       INFO                                       //
    // ================================================================================ //
    
    struct Infos
    {
        const ulong             lid;
        const sTag              name;
        const string            text;
        const Dico				dico;
        const Vector			args;
        
        Infos() noexcept : lid(0), name(Tags::_empty), text(""), dico(), args({}) {}
        
        Infos(const ulong _id, sTag _name, const string _text, Dico const& _dico, Vector const& _args)
        : lid(_id), name(_name), text(_text), dico(_dico), args(_args) {}
    };
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    //! The object is a graphical object.
    /**
     The object is a graphical class that aims to be instantiated in a patcher.
     */
    class ObjectModel : public AttributeBase::Manager
    {
    public:
        friend class PatcherModel;
        
        struct Io
        {
            enum Type
            {
                Message = 0u,
                Signal  = 1u,
                Both    = 2u
            };
            
            enum Polarity
            {
                Cold   = false,
                Hot    = true
            };
        };
        
        class Arguments : public flip::Object
        {
            
        };
        
    private:
        FlipTag         m_name;
        Arguments       m_args;
        flip::String    m_text;
        flip::Int       m_id;
        
    public:
        
        ObjectModel() = default;
                
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        ObjectModel(Infos const& detail, const sTag name) noexcept;
        
        //! Copy constructor.
        ObjectModel(const ObjectModel& rhs) noexcept;
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        virtual ~ObjectModel() noexcept;
        
        //! flip declare method
        static void declare();
        
    public:
        
        //! Retrieve the patcher that manages the object.
        /** The function retrieves the patcher that manages the object.
         @return The patcher that manages the object.
         */
        inline PatcherModel* getPatcher()
        {
            PatcherModel* patcher = parent().ptr<PatcherModel>();
            return patcher;
        }
        
        //! Retrieve the name of the object.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        inline sTag getName() const noexcept
        {
            return m_name;
        }
        
        //! Retrieve the text of the object.
        /** The function retrieves the text of the object.
         @return The text of the object.
         */
        inline string getText() const noexcept
        {
            return m_text;
        }
        
        //! Retrieve the id of the object.
        /** The function retrieves the id of the object.
         @return The id of the object.
         */
        inline ulong getId() const noexcept
        {
            return m_id;
        }
        
    public:
        
        //! Retrieves if the box should be hidden when the patcher is locked.
        /** The function retrieves if the box should be hidden when the patcher is locked.
         @return True if the box should be hidden when the patcher is locked, false otherwise.
         */
        inline bool isHiddenOnLock() const noexcept
        {
            return getAttributeValue(Tags::hidden);
        }
        
        //! Retrieve if the box should be displayed in presentation.
        /** The function retrieves if the box should be displayed in presentation.
         @return True if the box should be displayed in presentation, otherwise false.
         */
        inline bool isIncludeInPresentation() const noexcept
        {
            return getAttributeValue(Tags::presentation);
        }
        
        //! Retrieve the "ignoreclick" attribute value of the box.
        /** The function retrieves the "ignoreclick" attribute value of the box.
         @return The "ignoreclick" attribute value of the box.
         */
        inline bool getIgnoreClick() const noexcept
        {
            return getAttributeValue(Tags::ignoreclick);
        }
    };
}


#endif


