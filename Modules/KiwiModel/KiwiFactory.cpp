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

#include "KiwiFactory.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    Object* Factory::create(std::string const& name, std::string const& text)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        const auto& creators = getCreators();
        
        const auto it = creators.find(name);
        if(it != creators.end())
        {
            Object* obj = it->second->create(name, text);
            /*
            if(obj)
            {
                obj->read(detail.dico);
            }
            */
            return obj;
        }
        else
        {
            //Console::error("The factory doesn't know the object " + name->getName());
            return nullptr;
        }
    }
    
    //Object* create(std::string const& name, std::string const& text)
    
    bool Factory::has(std::string const& name)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        const auto& creators = getCreators();
        return creators.find(name) != creators.end();
    }
    
    void Factory::remove(std::string const& name)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        getCreators().erase(name);
    }
    
    std::vector<std::string const> Factory::names()
    {
        std::lock_guard<std::mutex> guard(getMutex());
        std::vector<std::string const> names;
        for(auto it : getCreators())
        {
            names.push_back(it.first);
        }
        return names;
    }
    
}

