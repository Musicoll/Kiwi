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

#ifndef __DEF_KIWI_TOOLS__
#define __DEF_KIWI_TOOLS__

#include <stdio.h>
#include <stdarg.h>
#include <cwchar>
#include <locale>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <cmath>
#include <array>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <thread>
#include <typeinfo>
#include <typeindex>
#include <codecvt>
#include <cxxabi.h>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

using namespace std;

#define _USE_MATH_DEFINES

#ifndef M_PI
#define M_E         2.71828182845904523536028747135266250   /* e              */
#define M_LOG2E     1.44269504088896340735992468100189214   /* log2(e)        */
#define M_LOG10E    0.434294481903251827651128918916605082  /* log10(e)       */
#define M_LN2       0.693147180559945309417232121458176568  /* loge(2)        */
#define M_LN10      2.30258509299404568401799145468436421   /* loge(10)       */
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi           */
#define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)     */
#define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)        */
#define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)      */
#endif

namespace Kiwi
{
    class Atom;
    
    class Tag;
    typedef shared_ptr<const Tag>       sTag;
    typedef weak_ptr<const Tag>         wTag;
    
    class Clock;
    typedef shared_ptr<Clock>           sClock;
    typedef weak_ptr<Clock>             wClock;
    
    class Beacon;
    typedef shared_ptr<Beacon>          sBeacon;
    typedef weak_ptr<Beacon>            wBeacon;

    typedef unsigned long               ulong;
    typedef shared_ptr<const Tag>       sTag;
    typedef vector<Atom>                Vector;
    typedef map<sTag, Atom>             Dico;
    
    class Error : public exception
    {
        const string m_message;
    public:
        inline Error() noexcept : m_message() {};
        
        Error(string const& message) noexcept :
        m_message(message)
        {
            ;
        }
        
        virtual ~Error() noexcept
        {
            ;
        }
        
        virtual const char* what() const noexcept
        {
            return m_message.c_str();
        }
    };
    
    class multiinheritable_enable_shared_from_this: public enable_shared_from_this<multiinheritable_enable_shared_from_this>
    {
    public:
        virtual ~multiinheritable_enable_shared_from_this(){}
    };
    
    template <class T> class inheritable_enable_shared_from_this : virtual public multiinheritable_enable_shared_from_this
    {
    public:
        shared_ptr<T> shared_from_this() noexcept
        {
            return dynamic_pointer_cast<T>(multiinheritable_enable_shared_from_this::shared_from_this());
        }
        
        shared_ptr<const T> shared_from_this() const noexcept
        {
            return dynamic_pointer_cast<const T>(multiinheritable_enable_shared_from_this::shared_from_this());
        }
    };
    
    template <class Type> constexpr Type clip(const Type& n, const Type& lower, const Type& upper)
    {
        return max(lower, min(n, upper));
    }
    
    template <class Type, typename numericType> constexpr Type lerp(const Type& startval, const Type& endval, const numericType& factor)
    {
        return (startval + factor * (endval - startval));
    }
    
    template <class Type> Type wrap(const Type& n, const Type& lower, const Type& upper)
    {
        const Type increment = upper - lower;
        Type new_value = n;
        while(new_value < lower)
        {
            new_value += increment;
        }
        while(new_value > upper)
        {
            new_value -= increment;
        }
        return new_value;
    }
    
    inline string trimDecimal(string& text)
    {
        string::size_type pos = text.find('.');
        if(pos != string::npos)
        {
            string::size_type size = text.size();
            while(size > pos && text[size - 1] == '0')
            {
                text.pop_back();
                size--;
            }
        }
        return text;
    }
    
    inline string toString(bool __val)
    {
        return (__val) ? "true" : "false";
    }
    
    inline string toString(int __val)
    {
        return to_string(__val);
    }
    
    inline string toString(unsigned __val)
    {
        return to_string(__val);
    }
    
    inline string toString(long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(ulong __val)
    {
        return to_string(__val);
    }
    
    inline string toString(long long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(unsigned long long __val)
    {
        return to_string(__val);
    }
    
    inline string toString(float __val, bool trim = true)
    {
        if(trim)
        {
            string text = to_string(__val);
            return trimDecimal(text);
        }
        else
        {
            return to_string(__val);
        }
    }
    
    inline string toString(double __val, bool trim = true)
    {
        if(trim)
        {
            string text = to_string(__val);
            return trimDecimal(text);
        }
        else
        {
            return to_string(__val);
        }
    }
    
    inline string toString(long double __val, bool trim = true)
    {
        if(trim)
        {
            string text = to_string(__val);
            return trimDecimal(text);
        }
        else
        {
            return to_string(__val);
        }
    }
    
    template<class T> inline T fromString(string const& __val)
    {
        return T();
    }
    
    template<> inline bool fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return (bool)stol(__val.c_str()+pos);
        }
        else
        {
            return false;
        }
    }
    
    template<> inline int fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return stoi(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline long fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return stol(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline ulong fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("0123456789");
        if(pos != string::npos)
        {
            return stoul(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline long long fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789");
        if(pos != string::npos)
        {
            return stoll(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline unsigned long long fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("0123456789");
        if(pos != string::npos)
        {
            return stoull(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline float fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789.");
        if(pos != string::npos)
        {
            return stof(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline double fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789.");
        if(pos != string::npos)
        {
            return stod(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    template<> inline long double fromString(string const& __val)
    {
        string::size_type pos = __val.find_first_of("-0123456789.");
        if(pos != string::npos)
        {
            return stold(__val.c_str()+pos);
        }
        else
        {
            return 0;
        }
    }
    
    static inline string jsonEscape(string const& text)
    {
        ostringstream ss;
        for(auto iter = text.cbegin(); iter != text.cend(); iter++)
        {
            switch (*iter)
            {
                case '\\': ss << "\\\\"; break;
                case '"': ss << "\\\""; break;
                case '/': ss << "\\/"; break;
                case '\b': ss << "\\b"; break;
                case '\f': ss << "\\f"; break;
                case '\n': ss << "\\n"; break;
                case '\r': ss << "\\r"; break;
                case '\t': ss << "\\t"; break;
                default: ss << *iter; break;
            }
        }
        return '\"' + ss.str() + '\"';
    }
    
    static inline string jsonUnescape(string const& text)
    {
        bool state = false;
        ostringstream ss;
        for(auto iter = text.cbegin(); iter != text.cend(); iter++)
        {
            if(state)
            {
                switch(*iter)
                {
                    case '"': ss << '\"'; break;
                    case '/': ss << '/'; break;
                    case 'b': ss << '\b'; break;
                    case 'f': ss << '\f'; break;
                    case 'n': ss << '\n'; break;
                    case 'r': ss << '\r'; break;
                    case 't': ss << '\t'; break;
                    case '\\': ss << '\\'; break;
                    default: ss << *iter; break;
                }
                state = false;
            }
            else
            {
                switch(*iter)
                {
                    case '"': return ss.str();
                    case '\\': state = true; break;
                    default: ss << *iter; break;
                }
            }
        }
        return ss.str();
    }
    
};

#endif


