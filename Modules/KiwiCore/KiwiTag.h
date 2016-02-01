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

#ifndef __DEF_KIWI_TAG__
#define __DEF_KIWI_TAG__

#include "KiwiTools.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      TAG                                         //
    // ================================================================================ //
    
    //! The tag is an unique object that matchs to a "unique" string in the scope of all the kiwi applications.
    /**
     The tag are uniques and matchs to a string. If you create a tag with a string that already matchs to a tag, the creation function will return this tag, otherwise it will create a new tag.
     @see TagFactory
     */
    class Tag
    {
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        inline Tag(string const& name) : m_name(name) {}
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        inline Tag(string&& name) noexcept : m_name(name) {}
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        inline ~Tag() noexcept {}
        
        //! Retrieve the string of the tag.
        /** The function retrieves the unique string of the tag.
         @return The string of the tag.
         */
        inline string getName() const noexcept { return m_name; }
        
        //! Tag creator.
        /** This function checks if a tag with this name has already been created and returns it,
         otherwise it creates a new tag with this name.
         @param  name   The name of the tag to retrieve.
         @return        The tag that match with the name.
         */
        static inline sTag create(string const& name)
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = m_tags.find(name);
            if(it != m_tags.end())
            {
                return it->second;
            }
            else
            {
                auto tag = make_shared<Tag>(name);
                m_tags[name] = tag;
                return tag;
            }
        }
        
        //! Tag creator.
        /** This function checks if a tag with this name has already been created and returns it,
         otherwise it creates a new tag with this name.
         @param  name   The name of the tag to retrieve.
         @return        The tag that match with the name.
         */
        static inline sTag create(string&& name) noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            auto it = m_tags.find(name);
            if(it != m_tags.end())
            {
                return it->second;
            }
            else
            {
                auto tag = make_shared<Tag>(name);
                m_tags[name] = tag;
                return tag;
            }
        }
        
    private:
        const string                m_name;
        static map<string, sTag>    m_tags;
        static mutex                m_mutex;
    };
    
    class Tags
    {
    public:
        static const sTag _empty;
        static const sTag arguments;
        static const sTag Arial;
        
        static const sTag bang;
        static const sTag bdcolor;
        static const sTag bgcolor;
        static const sTag bold;
        static const sTag bold_italic;
        
        static const sTag center;
        static const sTag color;
        static const sTag Color;
        static const sTag command;
        static const sTag circlecolor;
        
        static const sTag dsp;
        
        static const sTag focus;
        static const sTag font;
        static const sTag Font;
        static const sTag Font_Face;
        static const sTag Font_Justification;
        static const sTag Font_Name;
        static const sTag Font_Size;
        static const sTag fontface;
        static const sTag fontjustification;
        static const sTag fontname;
        static const sTag fontsize;
        static const sTag from;
        
        static const sTag gridsize;
        
        static const sTag hidden;
        
        static const sTag id;
        static const sTag ignoreclick;
        static const sTag italic;
        
        static const sTag ledcolor;
        static const sTag left;
        static const sTag link;
        static const sTag links;
        static const sTag locked_bgcolor;
        
        static const sTag Menelo;
        static const sTag mescolor;
        static const sTag Message_Color;
        
        static const sTag name;
        static const sTag newlink;
        static const sTag newobject;
        static const sTag ninlets;
        static const sTag normal;
        static const sTag noutlets;
        
        static const sTag object;
        static const sTag objects;
        
        static const sTag patcher;
        static const sTag position;
        static const sTag presentation;
        static const sTag presentation_position;
        static const sTag presentation_size;
        
        static const sTag removelink;
        static const sTag removeobject;
        static const sTag right;
        
        static const sTag set;
        static const sTag sigcolor;
        static const sTag Signal_Color;
        static const sTag size;
        
        static const sTag text;
        static const sTag textcolor;
        static const sTag to;
        
        static const sTag unlocked_bgcolor;
        
    };
};


#endif


