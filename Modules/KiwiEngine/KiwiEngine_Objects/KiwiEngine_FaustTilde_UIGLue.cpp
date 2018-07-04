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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_FaustTilde.h>
#include <faust/dsp/llvm-dsp.h>
#include <faust/gui/UI.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    
    class FaustTilde::UIGlue : public UI
    {
    public:
        class Parameter
        {
        public:
            int         type;
            FAUSTFLOAT* zone;
            FAUSTFLOAT  min;
            FAUSTFLOAT  max;
            FAUSTFLOAT  step;
            FAUSTFLOAT  deft;
            FAUSTFLOAT  saved;
            bool        dirty; // If the parameter should be deleted
        };
        
        
        UIGlue(FaustTilde& owner) : m_owner(owner)
        {
            
        }
        
        void addButton(const char* label, FAUSTFLOAT* zone) override
        {
            addParameter(label, 0, zone, 0, 0, 0, 0);
        }
        
        void addCheckButton(const char* label, FAUSTFLOAT* zone) override
        {
            addParameter(label, 1, zone, 0.f, 1.f, 1.f, 0.f);
        }
        
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        {
            addParameter(label, 2, zone, min, max, step, init);
        }
        
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        {
            addParameter(label, 2, zone, min, max, step, init);
        }
        
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override
        {
            addParameter(label, 2, zone, min, max, step, init);
        }
        
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override {};
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) override {};
        void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) override {}
        
        void declare(FAUSTFLOAT*, const char* key, const char* value) override
        {
            m_owner.log(std::string("faust~: declare ") + std::string(key) + std::string(" - ") + std::string(value));
        }
        
        void openTabBox(const char* label) override {};
        void openHorizontalBox(const char* label) override {}
        void openVerticalBox(const char* label) override {}
        void closeBox() override {}
        
        void log()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            m_owner.log("faust~: number of parameters " + std::to_string(m_parameters.size()));
            for(auto const& param : m_parameters)
            {
                m_owner.log(" ");
                m_owner.log("faust~: parameter " + param.first);
                m_owner.log("faust~: type " + std::to_string(param.second.type));
                m_owner.log("faust~: default " + std::to_string(param.second.saved));
                m_owner.log("faust~: minimum " + std::to_string(param.second.min));
                m_owner.log("faust~: maximum " + std::to_string(param.second.max));
                m_owner.log("faust~: step " + std::to_string(param.second.step));
            }
        }
        
        void set(const std::string& name)
        {
            if(m_mutex_glue.try_lock())
            {
                auto it = m_parameters.find(name);
                if(it != m_parameters.end())
                {
                    if(it->second.dirty)
                    {
                        m_mutex_glue.unlock();
                        m_owner.warning(std::string("FAUST interfaces \"") + name + std::string("\" not valid anymore"));
                        return;
                    }
                    if(it->second.type == 0)
                    {
                        *(it->second.zone) = 0;
                        *(it->second.zone) = 1;
                    }
                    else
                    {
                        m_owner.warning(std::string("FAUST interface \"") + name + std::string("\" is a requires a value"));
                    }
                }
                else
                {
                    m_owner.warning(std::string("no such FAUST interface \"") + name + std::string("\""));
                }
                m_mutex_glue.unlock();
            }
            else
            {
                m_owner.warning(std::string("FAUST interfaces being processed - please wait"));
            }
        }
        
        void set(const std::string& name, FAUSTFLOAT value)
        {
            if(m_mutex_glue.try_lock())
            {
                auto it = m_parameters.find(name);
                if(it != m_parameters.end())
                {
                    if(it->second.dirty)
                    {
                        m_mutex_glue.unlock();
                        m_owner.warning(std::string("FAUST interfaces \"") + name + std::string("\" not valid anymore"));
                        return;
                    }
                    if(it->second.type == 1)
                    {
                        *(it->second.zone) = static_cast<FAUSTFLOAT>(value < std::numeric_limits<FAUSTFLOAT>::epsilon());
                    }
                    else if(it->second.type == 2)
                    {
                        const FAUSTFLOAT min = it->second.min;
                        const FAUSTFLOAT max = it->second.max;
                        *(it->second.zone) = std::max(std::min(max, value), min);
                    }
                    else
                    {
                        m_owner.warning(std::string("FAUST interface \"") + name + std::string("\" is doesn't requires a value"));
                    }
                }
                else
                {
                    m_owner.warning(std::string("no such FAUST interface \"") + name + std::string("\""));
                }
                m_mutex_glue.unlock();
            }
            else
            {
                m_owner.warning(std::string("FAUST interfaces being processed - please wait"));
            }
        }
        
        void resetToDefault()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                *param.second.zone = param.second.deft;
            }
        }
        
        void saveStates()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                param.second.saved = *param.second.zone;
            }
        }
        
        void recallStates()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                *param.second.zone = param.second.saved;
            }
        }
        
        void prepareChanges()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto& param : m_parameters)
            {
                param.second.dirty = true;
            }
        }
        
        void finishChanges()
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            for(auto it = m_parameters.begin(); it != m_parameters.end();)
            {
                if(it->second.dirty)
                {
                    it = m_parameters.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }
        
    private:
        
        void addParameter(const char* name, int type, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step, FAUSTFLOAT init)
        {
            std::lock_guard<std::mutex> guard(m_mutex_glue);
            auto it = m_parameters.find(name);
            if(it != m_parameters.end())
            {
                if(it->second.type != type ||
                   it->second.min != min ||
                   it->second.max != max ||
                   it->second.step != step ||
                   it->second.deft != init)
                {
                    it->second = Parameter({type, zone, min, max, step, init, init, false});
                    *zone = init;
                }
                else
                {
                    it->second.saved = *it->second.zone;
                    it->second.zone = zone;
                    it->second.dirty = false;
                    *zone = it->second.saved;
                }
            }
            else
            {
                m_parameters[name] = Parameter({type, zone, min, max, step, init, init, false});
                *zone = init;
            }
        }
        
        std::map<std::string, Parameter> m_parameters;
        FaustTilde&                      m_owner;
        std::mutex                       m_mutex_glue;
    };
}}
