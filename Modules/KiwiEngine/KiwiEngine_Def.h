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

#pragma once

#include <cstddef>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <exception>
#include <functional>

#include <KiwiTool/KiwiTool_Atom.h>

#include <KiwiTool/KiwiTool_Scheduler.h>

namespace kiwi
{
    namespace model
    {
        class Link;
        class Object;
        class Patcher;
    }
    
    namespace engine
    {
        class Link;
        class Object;
        class Patcher;
        class Instance;
    }
}
