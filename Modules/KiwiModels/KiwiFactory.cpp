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

#include "KiwiFactory.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    ObjectModel* FactoryModel::create(const sTag name, Infos const& detail)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        std::map<sTag, sCreator>& creators(getCreators());
        
        const auto it = creators.find(name);
        if(it != creators.end())
        {
            ObjectModel* obj = it->second->create(detail);
            if(obj)
            {
                obj->read(detail.dico);
            }
            return obj;
        }
        else
        {
            //Console::error("The factory doesn't know the object " + name->getName());
            return nullptr;
        }
    }
    
    bool FactoryModel::has(const sTag name)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        const auto& creators = getCreators();
        return creators.find(name) != creators.end();
    }
    
    void FactoryModel::remove(const sTag name)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        std::map<sTag, sCreator>& creators(getCreators());
        creators.erase(name);
    }
    
    std::vector<sTag> FactoryModel::names()
    {
        std::lock_guard<std::mutex> guard(getMutex());
        std::vector<sTag> names;
        for(auto it : getCreators())
        {
            names.push_back(it.first);
        }
        return names;
    }
    
}

