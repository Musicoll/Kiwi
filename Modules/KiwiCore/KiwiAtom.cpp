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

#include "KiwiAtom.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    Atom::Atom(Atom const& other) noexcept
    {
        const auto quark = other.m_quark;
        
        if(other.isBool())
        {
            m_quark = new QuarkBool(quark->getBool());
        }
        else if(other.isInt())
        {
            m_quark = new QuarkInt(quark->getInt());
        }
        else if(other.isFloat())
        {
            m_quark = new QuarkFloat(quark->getFloat());
        }
        else if(other.isTag())
        {
            m_quark = new QuarkTag(quark->getTag());
        }
        else if(other.isVector())
        {
            m_quark = new QuarkVector(quark->getVector());
        }
        else if(other.isDico())
        {
            m_quark = new QuarkDico(quark->getDico());
        }
        else
        {
            m_quark = new Quark();
        }
    }
    
    
    Atom& Atom::operator=(Atom const& other) noexcept
    {
        delete m_quark;
        
        const auto quark = other.m_quark;
        
        if(other.isBool())
        {
            m_quark = new QuarkBool(quark->getBool());
        }
        else if(other.isInt())
        {
            m_quark = new QuarkInt(quark->getInt());
        }
        else if(other.isFloat())
        {
            m_quark = new QuarkFloat(quark->getFloat());
        }
        else if(other.isTag())
        {
            m_quark = new QuarkTag(quark->getTag());
        }
        else if(other.isVector())
        {
            m_quark = new QuarkVector(quark->getVector());
        }
        else if(other.isDico())
        {
            m_quark = new QuarkDico(quark->getDico());
        }
        else
        {
            m_quark = new Quark();
        }
        
        return *this;
    }
    
    std::ostream& Atom::toJson(std::ostream &output, const Atom &atom, ulong& indent)
    {
        if(atom.isBool())
        {
            output << atom.operator bool();
        }
        else if(atom.isInt())
        {
            output << atom.operator integer_t();
        }
        else if(atom.isFloat())
        {
            output << atom.operator float_t();
        }
        else if(atom.isTag())
        {
            output << jsonEscape((atom.operator tag_t())->getName());
        }
        else if(atom.isVector())
        {
            Vector const& vec = atom;
            output << '[';
            for(Vector::size_type i = 0; i < vec.size();)
            {
                toJson(output, vec[i], indent);
                if(++i != vec.size())
                {
                    output << ", ";
                }
            }
            output << ']';
        }
        else if(atom.isDico())
        {
            Dico const& dico = atom;
            output << "{\n";
            ++indent;
            for(auto it = dico.begin(); it != dico.end();)
            {
                for(ulong i = 0; i < indent; i++)
                {
                    output << '\t';
                }
                output << jsonEscape(it->first->getName()) << " : ";
                toJson(output, it->second, indent);
                if(++it != dico.end())
                {
                    output << ',' << '\n';
                }
                else
                {
                    output << '\n';
                }
            }
            --indent;
            for(ulong i = 0; i < indent; i++)
            {
                output << '\t';
            }
            output << '}';
        }
        return output;
    }
    
    std::ostream& operator<<(std::ostream &output, const Atom &atom)
    {
        const bool boolalpha = output.flags() & std::ios::boolalpha;
        if(!boolalpha)
        {
            output << std::boolalpha;
        }
        ulong indent = 0;
        Atom::toJson(output, atom, indent);
        if(!boolalpha)
        {
            output << std::noboolalpha;
        }
        return output;
    }
    
    Vector Atom::parse(std::string const& text)
    {
        Vector atoms;
        const auto textlen = text.length();
        auto pos = text.find_first_not_of(' ', 0);
        
        while(pos < textlen)
        {
            std::string word;
            word.reserve(20); // is it more efficient ?
            bool is_tag      = false;
            bool is_number   = false;
            bool is_float    = false;
            bool is_negative = false;
            bool is_quoted   = false;
            
            while(pos < textlen)
            {
                const char c = text[pos];
                
                if(c == ' ')
                {
                    if(!is_quoted)
                    {
                        if(word.empty()) // delete useless white spaces
                        {
                            pos++;
                            continue;
                        }
                        else // preserve white space in quoted tags, otherwise break word
                        {
                            break;
                        }
                    }
                }
                else if(c == '\"')
                {
                    if(is_quoted) // closing quote
                    {
                        pos++;
                        break;
                    }
                    
                    if(word.empty()) // begin quote
                    {
                        pos++;
                        
                        // ignore if it can not be closed
                        if(text.find_first_of('\"', pos) != std::string::npos)
                            is_quoted = is_tag = true;
                        
                        continue;
                    }
                }
                else if(!is_tag)
                {
                    if(word.empty() && c == '-')
                    {
                        is_negative = true;
                    }
                    else if(!is_float && (word.empty() || is_number || is_negative) && c == '.')
                    {
                        is_float = true;
                    }
                    else if(isdigit(c) && (is_number || (word.empty() || is_negative || is_float)))
                    {
                        is_number = true;
                    }
                    else
                    {
                        is_tag = true;
                        is_number = is_negative = is_float = false;
                    }
                }
                
                word += c;
                pos++;
            }
            
            if(!word.empty())
            {
                if(is_number)
                {
                    if(is_float)
                    {
                        atoms.emplace_back(Atom(std::stod(word.c_str())));
                    }
                    else
                    {
                        atoms.emplace_back(Atom(static_cast<integer_t>(std::stol(word.c_str()))));
                    }
                }
                else
                {
                    atoms.emplace_back(Atom(Tag::create(jsonUnescape(word))));
                }
            }
        }
        
        return atoms;
    }
}



