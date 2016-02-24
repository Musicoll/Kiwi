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
}
#endif // KIWI_CORE_SYMBOL_HPP_INCLUDED
