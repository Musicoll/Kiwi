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


namespace kiwi
{
    // ================================================================================ //
    //                                      SYMBOL                                      //
    // ================================================================================ //
    
    //! @brief The Symbol holds a reference to a "unique" std::string in the global application scope.
    //! @details Comparing two Symbol objects is an O(1) operation, so it's much faster
    //! than comparing two std::string together. However Symbol creation is much slower than
    //! std::string creation as it needs to insert and lookup each created std::string in a set.
    //! So the best strategy to use the Symbol class optimally is to keep some static Symbol
    //! objects for the names you will often use or compare.
    //! @see Symbols
    class Symbol
    {
    private:
        using stringRef = std::reference_wrapper<const std::string>;
        
    public:
        
        //! @brief Constructs a Symbol that references an empty string.
        explicit Symbol() : m_name(getManager().getEmptyRef()) {};
        
        //! @brief Constructs a Symbol with an std::string.
        explicit Symbol(std::string const& name) : m_name(getStringRef(name)) {std::cout << "constructor string\n";};
        
        //! @brief Copy constructor
        Symbol(Symbol const& symbol) : m_name(symbol.m_name) {std::cout << "constructor copy\n";};
        
        //! @brief Copy assignment operator
        inline Symbol& operator=(Symbol const& rhs) noexcept
        {
            std::cout << "operator\n";
            m_name = rhs.m_name;
            return *this;
        }
        
        //! @brief Destructor.
        ~Symbol() = default;
        
        //! @brief Get the symbol as an std::string.
        inline std::string toString() const { return m_name.get(); }
        
        //! @brief Returns true if the two symbols are equals
        inline bool operator==(Symbol const& rhs) const noexcept
        {
            return (&m_name.get() == &rhs.m_name.get());
        }
        
        //! @brief Returns true if the two symbols are NOT equals
        inline bool operator!=(Symbol const& rhs) const noexcept
        {
            return ! (*this == rhs);
        }
        
    private:
        
        class Manager
        {
        public:
            Manager() : m_empty_ref(getStringRef("")) {}
            
            stringRef getStringRef(const std::string& name)
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                return *m_symbols.emplace(name).first;
            }
            
            stringRef getEmptyRef() const noexcept
            {
                return m_empty_ref;
            }
        private:
            std::set<std::string>  m_symbols;
            std::mutex             m_mutex;
            stringRef              m_empty_ref;
        };
        
        static Manager& getManager()
        {
            static Manager mng;
            return mng;
        }

        static stringRef getStringRef(const std::string& name)
        {
            return getManager().getStringRef(name);
        }

        //! @internal constant string reference.
        stringRef m_name;
    };
}

#endif // KIWI_CORE_SYMBOL_HPP_INCLUDED
