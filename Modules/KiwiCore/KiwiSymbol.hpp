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

#ifndef KIWI_CORE_SYMBOL_HPP_INCLUDED
#define KIWI_CORE_SYMBOL_HPP_INCLUDED

#include "KiwiTools.hpp"

namespace kiwi
{
    class Symbol;
    
    class Symbols
    {
    public:
        static const Symbol empty;
    };
    
    // ================================================================================ //
    //                                      SYMBOL                                      //
    // ================================================================================ //
    
    //! @brief The Symbol holds a reference to a "unique" std::string in the global application scope.
    //! @details Comparing two Symbol objects is an O(1) operation, so it's much faster
    //! than comparing two std::string together. However Symbol creation is much slower than
    //! std::string creation as it need to insert and lookup each created std::string in a set.
    //! So the best strategy to use the Symbol class optimally is to keep some static Symbol
    //! objects for the names you will often use or compare.
    //! @see Symbols
    class Symbol
    {
    private:
        using stringRef = std::reference_wrapper<const std::string>;
        
    public:
        
        //! @brief Constructs a Symbol that references an empty string.
        Symbol() : m_name(Symbols::empty) {};
        
        //! @brief Constructs a Symbol with an std::string.
        Symbol(std::string const& name) : m_name(get(name)) {};
        
        //! @brief Constructs a Symbol with an std::string with move semantics.
        Symbol(std::string&& name) : m_name(get(std::forward<std::string>(name))) {};
        
        //! @brief Copy constructor
        Symbol(Symbol const& symbol) : m_name(symbol.m_name) {};
        
        //! @brief Copy assignment operator
        inline Symbol& operator = (Symbol rhs) noexcept
        {
            m_name = rhs.m_name;
            return *this;
        }
        
        //! @brief Destructor.
        ~Symbol() = default;
        
        //! @brief exchanges the internal std::string reference of two Symbol objects
        void swap(Symbol& other)
        noexcept (std::is_nothrow_move_constructible<stringRef>::value
                  && std::is_nothrow_move_assignable<stringRef>::value)
        {
            std::swap(m_name, other.m_name);
        }
        
        //! @brief Returns the std::string const reference that contains the Symbol.
        inline operator std::string const&() const noexcept { return m_name.get(); }
        
        //! @brief Get the symbol as an std::string.
        inline std::string toString() const                 { return m_name.get(); }
        
        //! @brief Returns true if the two symbols are equals
        inline bool operator == (Symbol const& rhs) const noexcept
        {
            return (&m_name.get() == &rhs.m_name.get());
        }
        
        //! @brief Returns true if the two symbols are NOT equals
        inline bool operator != (Symbol const& rhs) const noexcept
        {
            return ! (*this == rhs);
        }
        
    private:
        
        static stringRef get(const std::string& name)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return *m_symbols.emplace(name).first;
        }
        
        static stringRef get(std::string&& name)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return *m_symbols.emplace(std::move(name)).first;
        }
        
        //! @internal constant string reference.
        stringRef m_name;
        
        //! @internal
        static std::set<std::string>  m_symbols;
        static std::mutex             m_mutex;
    };
}

#endif // KIWI_CORE_SYMBOL_HPP_INCLUDED
