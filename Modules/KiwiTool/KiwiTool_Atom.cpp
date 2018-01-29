/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiTool/KiwiTool_Atom.h>

namespace kiwi { namespace tool {
    
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    Atom::Atom() noexcept
    : m_type(Type::Null)
    , m_value()
    {
        ;
    }
    
    Atom::Atom(const bool value) noexcept
    : m_type(Type::Int)
    , m_value(value ? int_t(1) : int_t(0))
    {
        ;
    }
    
    Atom::Atom(const int value) noexcept
    : m_type(Type::Int)
    , m_value(static_cast<int_t>(value))
    {
        ;
    }
    
    Atom::Atom(const long value) noexcept
    : m_type(Type::Int)
    , m_value(static_cast<int_t>(value))
    {
        ;
    }
    
    Atom::Atom(const long long value) noexcept
    : m_type(Type::Int)
    , m_value(static_cast<int_t>(value))
    {
        ;
    }
    
    Atom::Atom(const float value) noexcept
    : m_type(Type::Float)
    , m_value(static_cast<float_t>(value))
    {
        ;
    }
    
    Atom::Atom(const double value) noexcept
    : m_type(Type::Float)
    , m_value(static_cast<float_t>(value))
    {
        ;
    }
    
    Atom::Atom(string_t const& sym)
    : m_type(Type::String)
    , m_value(sym)
    {
        ;
    }
    
    Atom::Atom(string_t&& sym)
    : m_type(Type::String)
    , m_value(std::move(sym))
    {
        ;
    }
    
    Atom::Atom(char const* sym)
    : m_type(Type::String)
    , m_value(std::move(sym))
    {
        ;
    }
    
    Atom Atom::Comma()
    {
        Atom atom;
        atom.m_type = Type::Comma;
        return atom;
    }
    
    Atom Atom::Dollar(int_t index)
    {
        assert((index > 0 && index <= 9));
        
        Atom atom(index);
        atom.m_type = Type::Dollar;
        return (index > 0 && index <= 9) ? atom : Atom();
    }
    
    Atom::Atom(Atom const& other)
    : m_type(other.m_type)
    {
        switch(m_type)
        {
            case Type::Int:
            case Type::Dollar:
            {
                m_value = other.m_value.int_v;
                break;
            }
            case Type::Float:
            {
                m_value = other.m_value.float_v;
                break;
            }
            case Type::String:
            {
                assert(other.m_value.string_v != nullptr);
                m_value = *other.m_value.string_v;
                break;
            }
            case Type::Comma:
            {
                break;
            }
                
            default: break;
        }
    }
    
    Atom::Atom(Atom&& other)
    : m_type(std::move(other.m_type))
    , m_value(std::move(other.m_value))
    {
        // leave the other as a Null value Atom
        other.m_type = Type::Null;
        other.m_value = {};
    }
    
    Atom::~Atom()
    {
        if(isString())
        {
            std::allocator<string_t> alloc;
            alloc.destroy(m_value.string_v);
            alloc.deallocate(m_value.string_v, 1);
        }
    }
    
    Atom& Atom::operator=(Atom const& other)
    {
        if(!other.isString())
        {
            if(isString())
            {
                std::allocator<string_t> alloc;
                alloc.destroy(m_value.string_v);
                alloc.deallocate(m_value.string_v, 1);
            }
            
            m_value = other.m_value;
        }
        else
        {
            if(isString())
            {
                *m_value.string_v = *other.m_value.string_v;
            }
            else
            {
                m_value.string_v = create_string_pointer(*other.m_value.string_v);
            }
        }
        
        m_type = other.m_type;
        
        return *this;
    }
    
    Atom& Atom::operator=(Atom&& other) noexcept
    {
        std::swap(m_type, other.m_type);
        std::swap(m_value, other.m_value);
        return *this;
    }
    // ================================================================================ //
    //                                   Type Getters                                   //
    // ================================================================================ //
    
    Atom::Type Atom::getType() const noexcept
    {
        return m_type;
    }
    
    bool Atom::isNull() const noexcept
    {
        return (m_type == Type::Null);
    }
    
    bool Atom::isInt() const noexcept
    {
        return m_type == Type::Int;
    }
    
    bool Atom::isFloat() const noexcept
    {
        return m_type == Type::Float;
    }
    
    bool Atom::isNumber() const noexcept
    {
        return (isInt() || isFloat());
    }
    
    bool Atom::isString() const noexcept
    {
        return m_type == Type::String;
    }
    
    bool Atom::isBang() const
    {
        static const auto bang_str = "bang";
        return isString() && getString() == bang_str;
    }
    
    bool Atom::isComma() const noexcept
    {
        return m_type == Type::Comma;
    }
    
    bool Atom::isDollar() const noexcept
    {
        return m_type == Type::Dollar;
    }
    
    // ================================================================================ //
    //                                   Value Getters                                  //
    // ================================================================================ //
    
    Atom::int_t Atom::getInt() const noexcept
    {
        if(isInt())
        {
            return m_value.int_v;
        }
        else if(isFloat())
        {
            return static_cast<int_t>(m_value.float_v);
        }
        
        return int_t(0);
    }
    
    Atom::float_t Atom::getFloat() const noexcept
    {
        if(isFloat())
        {
            return m_value.float_v;
        }
        else if(isInt())
        {
            return static_cast<float_t>(m_value.int_v);
        }
        
        return float_t(0.0);
    }
    
    Atom::string_t const& Atom::getString() const
    {
        if(isString())
        {
            return *m_value.string_v;
        }
        
        static const std::string empty_string;
        return empty_string;
    }
    
    //! @brief Retrieves the Dollar index value if the Atom is a dollar type.
    //! @return The Dollar index if the Atom is a dollar, 0 otherwise.
    //! @see getType(), isDollar(), isDollarTyped()
    Atom::int_t Atom::getDollarIndex() const
    {
        return isDollar() ? m_value.int_v : 0;
    }
    
    // ================================================================================ //
    //                                      VALUE                                       //
    // ================================================================================ //
    
    //! @internal Exception-safe object creation helper
    Atom::string_t* Atom::create_string_pointer(string_t const& v)
    {
        std::allocator<string_t> alloc;
        auto deleter = [&alloc](string_t * object) { alloc.deallocate(object, 1); };
        std::unique_ptr<string_t, decltype(deleter)> object(alloc.allocate(1), deleter);
        alloc.construct(object.get(), v);
        return object.release();
    }
    
    Atom::string_t* Atom::create_string_pointer(string_t&& v)
    {
        std::allocator<string_t> alloc;
        auto deleter = [&alloc](string_t * object) { alloc.deallocate(object, 1); };
        std::unique_ptr<string_t, decltype(deleter)> object(alloc.allocate(1), deleter);
        alloc.construct(object.get(), std::move(v));
        return object.release();
    }
    
    // ================================================================================ //
    //                                    ATOM HELPER                                   //
    // ================================================================================ //

    std::vector<Atom> AtomHelper::parse(std::string const& text, int flags)
    {
        std::vector<Atom> atoms;
        const auto text_end = text.cend();
        auto text_it = text.begin();
        
        enum class NumberState
        {
            NaN = -1,
            MaybeNumber = 0,
            GotMinus,
            GotDigit,
            GotDotWithoutDigit,
            GotDotAfterDigit,
            GotDigitAfterDot,
            GotExpon,
            GotPlusMinusAfterExpon,
            GotDigitAfterExpon,
        };
        
        enum class DollarState
        {
            NotDollar = -1,
            MaybeDollar = 0,
            GotDollar,
            DollarValid, // $1 => $9
        };
        
        for(;;)
        {
            // skip witespaces
            while((text_it != text_end) && (*text_it == ' ' || (*text_it <= 13 && *text_it >= 9)))
                text_it++;
            
            if(text_it == text_end)
                break; // end of parsing
            
            std::ostringstream text_buffer;
            
            NumberState numstate = NumberState::MaybeNumber;
            DollarState dollar_state = ((flags & ParsingFlags::Dollar)
                                        ? DollarState::MaybeDollar
                                        : DollarState::NotDollar);
            
            bool is_quoted   = false;
            bool is_comma    = false;
            int dollar_index = 0;
            
            bool lastslash = false;
            bool slash = false;
            
            while(text_it != text_end)
            {
                bool buffer_is_empty = (((long)text_buffer.tellp()) == 0);
                char c = *text_it;
                lastslash = slash;
                slash = (c == '\\');
                
                // skip witespaces characters
                if(c == ' ' || (c <= 13 && c >= 9))
                {
                    // preserve whitespaces in quoted tags
                    if(!is_quoted)
                    {
                        break;
                    }
                }
                else if(c == '\"' && !lastslash)
                {
                    // closing quote
                    if(is_quoted)
                    {
                        text_it++;
                        break;
                    }
                    
                    // opening quote
                    if(buffer_is_empty)
                    {
                        text_it++;
                        is_quoted = true;
                        continue;
                    }
                }
                else if(is_comma
                        || ((flags & ParsingFlags::Comma) && (c == ',' && !is_quoted)))
                {
                    if(!buffer_is_empty)
                    {
                        break;
                    }
                    
                    is_comma = true;
                }
                else if(!is_quoted && numstate >= NumberState::MaybeNumber)
                {
                    const bool digit = (c >= '0' && c <= '9'),
                    dot = (c == '.'), minus = (c == '-'),
                    plusminus = (minus || (c == '+')),
                    expon = (c == 'e' || c == 'E');
                    
                    if (numstate == NumberState::MaybeNumber)
                    {
                        if (minus) numstate = NumberState::GotMinus;
                        else if (digit) numstate = NumberState::GotDigit;
                        else if (dot) numstate = NumberState::GotDotWithoutDigit;
                        else numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotMinus)
                    {
                        if (digit) numstate = NumberState::GotDigit;
                        else if (dot) numstate = NumberState::GotDotWithoutDigit;
                        else numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotDigit)
                    {
                        if (dot) numstate = NumberState::GotDotAfterDigit;
                        else if (expon) numstate = NumberState::GotDigit;
                        else if (!digit) numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotDotWithoutDigit)
                    {
                        if (digit) numstate = NumberState::GotDigitAfterDot;
                        else numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotDotAfterDigit)
                    {
                        if (digit) numstate = NumberState::GotDigitAfterDot;
                        else if (expon) numstate = NumberState::GotExpon;
                        else numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotDigitAfterDot)
                    {
                        if (expon) numstate = NumberState::GotExpon;
                        else if (!digit) numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotExpon)
                    {
                        if (plusminus) numstate = NumberState::GotPlusMinusAfterExpon;
                        else if (digit) numstate = NumberState::GotDigitAfterExpon;
                        else numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotPlusMinusAfterExpon)
                    {
                        if (digit) numstate = NumberState::GotDigitAfterExpon;
                        else numstate = NumberState::NaN;
                    }
                    else if (numstate == NumberState::GotDigitAfterExpon)
                    {
                        if (!digit) numstate = NumberState::NaN;
                    }
                }
                
                if(dollar_state >= DollarState::MaybeDollar
                   && numstate == NumberState::NaN && !is_quoted)
                {
                    if(dollar_state == DollarState::MaybeDollar)
                    {
                        dollar_state = ((buffer_is_empty && (c == '$') && !is_quoted && !lastslash)
                                        ? DollarState::GotDollar
                                        : DollarState::NotDollar);
                    }
                    else if (dollar_state == DollarState::GotDollar)
                    {
                        dollar_state = ((c > '0' && c <= '9')
                                        ? DollarState::DollarValid
                                        : DollarState::NotDollar);
                        
                        dollar_index = (dollar_state == DollarState::DollarValid ? (c - '0') : 0);
                    }
                    else if (dollar_state == DollarState::DollarValid)
                    {
                        if((c == ' ') || (flags & ParsingFlags::Comma && c == ','))
                        {
                            break;
                        }
                        else
                        {
                            dollar_state = DollarState::NotDollar;
                        }
                    }
                }
                
                // strip slashes
                if(slash)
                {
                    if(!lastslash)
                    {
                        text_it++;
                        continue;
                    }
                    else
                    {
                        slash = false;
                    }
                }
                
                text_buffer << c;
                text_it++;
            }
            
            const auto& word = text_buffer.str();
            
            if(!word.empty())
            {
                if(numstate == NumberState::GotDigit)
                {
                    atoms.emplace_back(std::stol(word));
                }
                else if(numstate == NumberState::GotDotAfterDigit
                        || numstate == NumberState::GotDigitAfterDot
                        || numstate == NumberState::GotDigitAfterExpon)
                {
                    atoms.emplace_back(std::stod(word));
                }
                else if(is_comma)
                {
                    atoms.emplace_back(Atom::Comma());
                }
                else if(dollar_state == DollarState::DollarValid)
                {
                    atoms.emplace_back(Atom::Dollar(dollar_index));
                }
                else
                {
                    atoms.emplace_back(word);
                }
            }
        }
        
        return atoms;
    }
    
    std::string AtomHelper::toString(Atom const& atom, const bool add_quotes)
    {
        switch(atom.getType())
        {
            case Atom::Type::Int:
            {
                return std::to_string(atom.getInt());
            }
            case Atom::Type::Float:
            {
                return trimDecimal(std::to_string(atom.getFloat()));
            }
            case Atom::Type::String:
            {
                const auto& str = atom.getString();
                if(!str.empty())
                {
                    const bool need_quote = add_quotes && str.find_first_of(' ', 0) != std::string::npos;
                    
                    if(need_quote)
                    {
                        std::ostringstream output;
                        output << "\"";
                        
                        // add slashes
                        for(auto c : str)
                        {
                            if(c == '\\' || c == '"')
                            {
                                output << '\\';
                            }
                            
                            output << c;
                        }
                        
                        output << "\"";
                        return output.str();
                    }
                    
                    return str;
                }
            }
            case Atom::Type::Comma:
            {
                return ",";
            }
            case Atom::Type::Dollar:
            {
                return "$" + std::to_string(atom.getDollarIndex());
            }
                
            default : break;
        }
        
        return {};
    }
    
    std::string AtomHelper::toString(std::vector<Atom> const& atoms, const bool add_quotes)
    {
        static const auto delimiter = ' ';
        std::ostringstream output;
        if(!atoms.empty())
        {
            for(std::vector<Atom>::size_type i = 0; i < atoms.size();)
            {
                output << toString(atoms[i], add_quotes);
                if(++i != atoms.size() && !atoms[i].isComma())
                {
                    output << delimiter;
                }
            }
        }
        
        return output.str();
    }
    
    std::string AtomHelper::trimDecimal(std::string const& text)
    {
        auto output = text;
        auto pos = text.find('.');
        
        if(pos != std::string::npos)
        {
            auto size = text.size();
            while(size > pos && text[size - 1] == '0')
            {
                output.pop_back();
                size--;
            }
        }
        
        return output;
    }
    
}}
