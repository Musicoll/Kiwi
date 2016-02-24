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

#ifndef KIWI_TAG_H_INCLUDED
#define KIWI_TAG_H_INCLUDED

#include "KiwiTools.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      SYMBOL                                      //
    // ================================================================================ //
    
    //! @brief The Symbol is an unique object that matchs to a "unique" std::string in the scope of all the Kiwi application.
    //! @details Comparing two Symbol objects is very fast (O(1))
    //! The symbols are uniques and match to a string. If you construct a symbol with a std::string that already matchs to a symbol, the creation function will return this symbol, otherwise it will create a new one.
    class Symbol
    {
    public:
        
        //! @brief Constructs a Symbol with an std::string.
        Symbol(std::string const& name) : m_name(get(name)) {};
        
        //! @brief Constructs a Symbol with an std::string with move semantics.
        Symbol(std::string&& name) : m_name(get(std::forward<std::string>(name))) {};
        
        //! @brief Copy constructor
        Symbol(Symbol const& symbol) : m_name(symbol.m_name) {};
        
        //! @brief Destructor.
        ~Symbol() = default;
        
        //! @brief Get the total number of symbols already cached
        //! @todo remove this function (?)
        static size_t size() noexcept                               { return m_symbols.size(); }
        
        //! @brief Get the symbol as an std::string.
        inline std::string toString() const noexcept                { return m_name; }
        
        //! @brief Returns true if the too symbols are equals
        inline bool operator == (Symbol const& rhs) const noexcept  { return (&m_name == &rhs.m_name); }
        
        //! @brief Returns true if the too symbols are NOT equals
        inline bool operator != (Symbol const& rhs) const noexcept  { return ! (*this == rhs); }
        
    private:
        
        static const std::string& get(const std::string& name) noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            const auto sym = m_symbols.emplace(name).first;
            return *sym;
        }
        
        static const std::string& get(std::string&& name) noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            const auto sym = m_symbols.emplace(std::move(name)).first;
            return *sym;
        }
        
        const std::string& m_name;
        
        //! @internal
        static std::set<std::string>  m_symbols;
        static std::mutex             m_mutex;
    };
            
    // ================================================================================ //
    //                                    SYMBOLS                                       //
    // ================================================================================ //
    
    struct Symbols
    {
        static const Symbol _empty;
        static const Symbol arguments;
        static const Symbol Arial;
    };
    
    // ================================================================================ //
    //                                      TAG                                         //
    // ================================================================================ //
    
    //! The tag is an unique object that matchs to a "unique" std::string in the scope of all the kiwi applications.
    /**
     The tag are uniques and matchs to a string. If you create a tag with a std::string that already matchs to a tag, the creation function will return this tag, otherwise it will create a new tag.
     @see TagFactory
     */
    class Tag
    {
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        inline Tag(std::string const& name) : m_name(name) {}

        //! The constructor.
        /** You should never use this method except if you really know what you do.
         */
        inline Tag(std::string&& name) noexcept : m_name(name) {}
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        inline ~Tag() noexcept {}
        
        //! Retrieve the std::string of the tag.
        /** The function retrieves the unique std::string of the tag.
         @return The std::string of the tag.
         */
        inline std::string getName() const noexcept { return m_name; }
        
        //! Tag creator.
        /** This function checks if a tag with this name has already been created and returns it,
         otherwise it creates a new tag with this name.
         @param  name   The name of the tag to retrieve.
         @return        The tag that match with the name.
         */
        static inline sTag create(std::string const& name)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            auto it = m_tags.find(name);
            if(it != m_tags.end())
            {
                return it->second;
            }
            else
            {
                auto tag = std::make_shared<Tag>(name);
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
        static inline sTag create(std::string&& name) noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            auto it = m_tags.find(name);
            if(it != m_tags.end())
            {
                return it->second;
            }
            else
            {
                auto tag = std::make_shared<Tag>(name);
                m_tags[name] = tag;
                return tag;
            }
        }
        
    private:
        const std::string                   m_name;
        static std::map<std::string, sTag>  m_tags;
        static std::mutex                   m_mutex;
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


#endif // KIWI_TAG_H_INCLUDED


