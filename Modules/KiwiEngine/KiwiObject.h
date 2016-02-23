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

#ifndef __DEF_KIWI_OBJECT__
#define __DEF_KIWI_OBJECT__

#include "../KiwiModels/KiwiModels.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      DEFINES                                     //
    // ================================================================================ //
    
    class Object;
    typedef std::shared_ptr<Object>          sObject;
    typedef std::weak_ptr<Object>            wObject;
    typedef std::shared_ptr<const Object>    scObject;
    typedef std::weak_ptr<const Object>      wcObject;
    
    class Link;
    typedef std::shared_ptr<Link>            sLink;
    typedef std::shared_ptr<const Link>      scLink;
    typedef std::weak_ptr<Link>              wLink;
    typedef std::weak_ptr<const Link>        wcLink;
    
    class Patcher;
    typedef std::shared_ptr<Patcher>        sPatcher;
    typedef std::weak_ptr<Patcher>          wPatcher;
    typedef std::shared_ptr<const Patcher>  scPatcher;
    typedef std::weak_ptr<const Patcher>    wcPatcher;
    
    class Instance;
    typedef std::shared_ptr<Instance>        sInstance;
    typedef std::weak_ptr<Instance>          wInstance;
    typedef std::shared_ptr<const Instance>  scInstance;
    typedef std::weak_ptr<const Instance>    wcInstance;
    
    // ================================================================================ //
    //                                       INFO                                       //
    // ================================================================================ //
    
    /*
    struct Infos
    {
        const ulong             lid;
        const sTag              name;
        const std::string            text;
        const Dico				dico;
        const Vector			args;
        
        Infos() noexcept : lid(0), name(Tags::_empty), text(""), dico(), args({}) {}
        
        Infos(const ulong _id, sTag _name, const std::string _text, Dico const& _dico, Vector const& _args)
        : lid(_id), name(_name), text(_text), dico(_dico), args(_args) {}
    };
    */
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    //! The object is a graphical object.
    /**
     The object is a graphical class that aims to be instantiated in a patcher.
     */
    class Object
    {
    public:
        friend class Patcher;
        
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
        
    private:
        wPatcher        m_patcher;
        ObjectModel&    m_model;
        
    public:
        
        //Object() = default;
        
        //! Constructor.
        /** You should never call this method except if you really know what you're doing.
         */
        Object(ObjectModel& model) noexcept;
        
        //! Copy constructor.
        Object(const Object& rhs) noexcept;
        
        //! Destructor.
        /** You should never call this method except if you really know what you're doing.
         */
        virtual ~Object() noexcept;
        
    public:
        
        //! Retrieve the patcher that manages the object.
        /** The function retrieves the patcher that manages the object.
         @return The patcher that manages the object.
         */
        inline sPatcher getPatcher() const
        {
            return m_patcher.lock();
        }
        
        //! Retrieve the name of the object.
        /** The function retrieves the name of the object as a tag.
         @return The name of the object as a tag.
         */
        inline sTag getName() const noexcept
        {
            return Tag::create(m_model.getName());
        }
        
        //! Retrieve the text of the object.
        /** The function retrieves the text of the object.
         @return The text of the object.
         */
        inline std::string getText() const noexcept
        {
            return m_model.getText();
        }
        
        //! Retrieve the id of the object.
        /** The function retrieves the id of the object.
         @return The id of the object.
         */
        inline ulong getId() const noexcept
        {
            return m_model.getId();
        }
        
    public:
        
        //! Retrieves if the box should be hidden when the patcher is locked.
        /** The function retrieves if the box should be hidden when the patcher is locked.
         @return True if the box should be hidden when the patcher is locked, false otherwise.
         */
        inline bool isHiddenOnLock() const noexcept
        {
            return m_model.getAttributeValue(Tags::hidden);
        }
        
        //! Retrieve if the box should be displayed in presentation.
        /** The function retrieves if the box should be displayed in presentation.
         @return True if the box should be displayed in presentation, otherwise false.
         */
        inline bool isIncludeInPresentation() const noexcept
        {
            return m_model.getAttributeValue(Tags::presentation);
        }
        
        //! Retrieve the "ignoreclick" attribute value of the box.
        /** The function retrieves the "ignoreclick" attribute value of the box.
         @return The "ignoreclick" attribute value of the box.
         */
        inline bool getIgnoreClick() const noexcept
        {
            return m_model.getAttributeValue(Tags::ignoreclick);
        }
    };
}


#endif


