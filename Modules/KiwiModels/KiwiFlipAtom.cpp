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

#include "KiwiFlipAtom.h"

namespace Kiwi
{    
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    FlipAtom::FlipAtom(FlipAtom const& other) noexcept
    {
        if(other.isBool())
        {
            m_quark.reset<QuarkBool>((bool)other);
        }
        else if(other.isLong())
        {
            m_quark.reset<QuarkLong>((long)other);
        }
        else if(other.isDouble())
        {
            m_quark.reset<QuarkDouble>((double)other);
        }
        else if(other.isTag())
        {
            m_quark.reset<QuarkTag>((sTag)other);
        }
        else if(other.isVector())
        {
            m_quark.reset<QuarkVector>((FlipAtomVector const&)other);
        }
        else if(other.isDico())
        {
            m_quark.reset<QuarkDico>((FlipAtomDico const&)other);
        }
        else
        {
            m_quark.reset();
        }
    }
   
    
    FlipAtom& FlipAtom::operator=(FlipAtom const& other) noexcept
    {
        if(other.isBool())
        {
            m_quark.reset<QuarkBool>((bool)other);
        }
        else if(other.isLong())
        {
            m_quark.reset<QuarkLong>((long)other);
        }
        else if(other.isDouble())
        {
            m_quark.reset<QuarkDouble>((double)other);
        }
        else if(other.isTag())
        {
            m_quark.reset<QuarkTag>((sTag)other);
        }
        else if(other.isVector())
        {
            m_quark.reset<QuarkVector>((FlipAtomVector const&)other);
        }
        else if(other.isDico())
        {
            m_quark.reset<QuarkDico>((FlipAtomDico const&)other);
        }
        else
        {
            m_quark.reset();
        }
        return *this;
    }
    
    ostream& FlipAtom::toJson(ostream &output, const FlipAtom &atom, ulong& indent)
    {
        if(atom.isBool())
        {
            output << (bool)atom;
        }
        else if(atom.isLong())
        {
            output << (long)atom;
        }
        else if(atom.isDouble())
        {
            output << (double)atom;
        }
        else if(atom.isTag())
        {
            output << jsonEscape(((sTag)atom)->getName());
        }
        else if(atom.isVector())
        {
            FlipAtomVector const& vec = atom;
            output << '[';
            for(FlipAtomVector::size_type i = 0; i < vec.size();)
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
            FlipAtomDico const& dico = atom;
            output << '{' << endl;
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
                    output << ',' << endl;
                }
                else
                {
                    output << endl;
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
    
    ostream& operator<<(ostream &output, const FlipAtom &atom)
    {
        const bool boolalpha = output.flags() & ios::boolalpha;
        if(!boolalpha)
        {
            output << std::boolalpha;
        }
        ulong indent = 0;
        FlipAtom::toJson(output, atom, indent);
        if(!boolalpha)
        {
            output << std::noboolalpha;
        }
        return output;
    }
    
    FlipAtomVector FlipAtom::parse(string const& text)
    {
        FlipAtomVector atoms;
        const ulong textlen = text.length();
        string::size_type pos = text.find_first_not_of(' ', 0);
        
        while(pos < textlen)
        {
            string word;
            word.reserve(20); // is it more efficient ?
            bool isTag      = false;
            bool isNumber   = false;
            bool isFloat    = false;
            bool isNegative = false;
            bool isQuoted   = false;
            
            while(pos < textlen)
            {
                const char c = text[pos];
                
                if(c == ' ')
                {
                    if(!isQuoted)
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
                    if(isQuoted) // closing quote
                    {
                        pos++;
                        break;
                    }
                    
                    if(word.empty()) // begin quote
                    {
                        pos++;
                        
                        // ignore if it can not be closed
                        if(text.find_first_of('\"', pos) != string::npos)
                            isQuoted = isTag = true;
                        
                        continue;
                    }
                }
                else if(!isTag)
                {
                    if(word.empty() && c == '-')
                    {
                        isNegative = true;
                    }
                    else if(!isFloat && (word.empty() || isNumber || isNegative) && c == '.')
                    {
                        isFloat = true;
                    }
                    else if(isdigit(c) && (isNumber || (word.empty() || isNegative || isFloat)))
                    {
                        isNumber = true;
                    }
                    else
                    {
                        isTag = true;
                        isNumber = isNegative = isFloat = false;
                    }
                }
                
                word += c;
                pos++;
            }
            
            if(!word.empty())
            {
                if(isNumber)
                {
                    if(isFloat)
                    {
                        atoms.push_back(FlipAtom(stod(word.c_str())));
                    }
                    else
                    {
                        atoms.push_back(FlipAtom(stol(word.c_str())));
                    }
                }
                else
                {
                    atoms.push_back(FlipAtom(Tag::create(jsonUnescape(word))));
                }
            }
        }
        
        return atoms;
    }
}



