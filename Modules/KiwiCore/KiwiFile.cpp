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

#include <fstream>

#include "KiwiFile.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      FILE                                        //
    // ================================================================================ //
    
    FilePath::FilePath() : m_absolute_path()
    {
    }
    
    FilePath::FilePath(FilePath const& other) : m_absolute_path(other.m_absolute_path)
    {
    }
    
    FilePath::FilePath(std::string const& absolute_path) : m_absolute_path(absolute_path)
    {
    }
    
    FilePath::FilePath(FilePath && other):m_absolute_path(std::move(other.m_absolute_path))
    {
    }
    
    FilePath& FilePath::operator=(FilePath const& other)
    {
        if (&other != this)
        {
            m_absolute_path = other.m_absolute_path;
        }
        
        return *this;
    }
    
    FilePath& FilePath::operator=(FilePath && other)
    {
        if (&other != this)
        {
            m_absolute_path = std::move(other.m_absolute_path);
        }
        
        return *this;
    }
    
    bool FilePath::exist() const
    {
        std::ifstream f(m_absolute_path);
        return f.good();
    }
    
    bool FilePath::isKiwiFile() const
    {
        return getExtension() == "kiwi"; // CHeck if exists.
    }
    
    std::string FilePath::getAbsolutePath() const
    {
        return m_absolute_path;
    }
    
    std::string FilePath::getPath() const
    {
        std::string path;
        std::size_t found = m_absolute_path.find_last_of("/\\");
        
        if (found != std::string::npos)
        {
            path =  m_absolute_path.substr(0,found);
        }
        
        return path;
    }
    
    std::string FilePath::getName() const
    {
        std::string file_name;
        std::size_t found = m_absolute_path.find_last_of("/\\");
        
        if (found != std::string::npos)
        {
            file_name = m_absolute_path.substr(found + 1);
        }
        
        return file_name;
    }
    
    std::string FilePath::getExtension() const
    {
        std::string extension;
        std::size_t found = m_absolute_path.find_last_of(".");
        
        if (found != std::string::npos)
        {
            extension = m_absolute_path.substr(found + 1);
        }
        
        return extension;
    }
    
    bool FilePath::operator==(FilePath const& other) const
    {
        return m_absolute_path == other.m_absolute_path;
    }
}