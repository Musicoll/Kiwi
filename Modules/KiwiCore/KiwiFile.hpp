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

#ifndef KIWI_CORE_FILE_HPP_INCLUDED
#define KIWI_CORE_FILE_HPP_INCLUDED

#include <string>

namespace kiwi
{
    //! @brief Class used to represent a file and its absolute path.
    class File final
    {
    public:
        //! @brief Initializes a file with a null absolute path.
        File();
        //! @brief Initializes a file with absolute_path
        explicit File(std::string const& absolute_path);
        //! @brief Copies a file.
        File(File const& other);
        //! @brief Move constructor of file.
        File(File && other);
        
        //! @brief Returns the path of the file without name and extension.
        std::string getPath() const;
        
        //! @brief Returns the absolute path of the file including file name and extension.
        std::string getAbsolutePath() const;
        
        //! @brief Returns the name of the file including extension.
        std::string getName() const;
        
        //! @brief Returns the extension of the file.
        std::string getExtension() const;
        
        //! @brief Checks if the absolute_path point to an existing file.
        bool exist() const;
        //! @brief Checks if the file represents a document
        bool isKiwiFile() const;
        
        bool operator==(File const& other) const;
        File &operator=(File const& other);
        File &operator=(File && other);
        
        ~File() = default;
        
    private:
        std::string m_absolute_path;
    };
}

#endif // KIWI_CORE_FILE_HPP_INCLUDED
