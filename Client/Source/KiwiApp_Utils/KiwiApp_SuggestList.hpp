/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_KIWIAPP_SUGGEST_LIST_HPP_INCLUDED
#define KIWI_KIWIAPP_SUGGEST_LIST_HPP_INCLUDED

#include <string>
#include <algorithm>
#include <cctype> // ::tolower, ::toupper
#include <vector>

namespace kiwi
{
    // ================================================================================ //
    //                                    SUGGEST LIST                                  //
    // ================================================================================ //
    
    //! @brief A string container that provide suggestion list based on given patterns.
    //! @details This class maintain
    class SuggestList
    {
    public: // methods
        
        using entries_t = std::vector<std::string>;
        
        //! @brief Constructor.
        SuggestList() = default;
        
        //! @brief Constructor.
        SuggestList(entries_t entries) :
        m_options(),
        m_entries(entries.begin(), entries.end()),
        m_last_filter_pattern(""),
        m_need_update(true)
        {
            updateFilteredEntries();
        }
        
        //! @brief Destructor.
        ~SuggestList() = default;
        
        //! @brief options
        struct Options
        {
            // Score bonuses
            int adjacency_bonus             = 5;    //! For adjacent matches.
            
            // Score penalties
            int leading_letter_penalty      = -3;   //! For every letter in str before the first match.
            int max_leading_letter_penalty  = -9;   //! Maximum penalty for leading letters
            int unmatched_letter_penalty    = -1;   //! For every letter that doesn't matter
        };
        
        // iterators
        using iterator                      = entries_t::iterator;
        using const_iterator                = entries_t::const_iterator;
        
        iterator begin()                    { return m_filtered_entries.begin(); }
        iterator end()                      { return m_filtered_entries.end(); }
        const_iterator begin() const        { return m_filtered_entries.cbegin(); }
        const_iterator end() const          { return m_filtered_entries.cend(); }
        
        //! @brief Set the sorting options.
        void setOptions(Options options)    { m_options = options; }
        
        //! @brief Add a suggestion entry to the list.
        //! @details This will also update the filtered list.
        //! @see addEntries
        void addEntry(std::string const& entry)
        {
            m_entries.emplace_back(entry);
            updateFilteredEntries();
        }
        
        //! @brief Add suggestion entries to the list.
        //! @detail This will also update the filtered list.
        //! @see addEntry
        void addEntries(std::vector<std::string> const& entries)
        {
            m_entries.insert(m_entries.end(), entries.begin(), entries.end());
            updateFilteredEntries();
        }
        
        //! @brief Returns the size of the current filtered selection
        entries_t::size_type size() const noexcept { return m_filtered_entries.size(); }
        
        //! @brief Returns true if there is no suggestion entry.
        bool empty() const noexcept { return m_filtered_entries.empty(); }
        
        //! @brief Clear all entries.
        void clear() { m_entries.clear(); m_filtered_entries.clear(); }
        
        //! @brief Returns the current filter pattern applied to the list
        std::string const& getCurrentFilter() const { return m_last_filter_pattern; }
        
        //! @brief Apply a pattern matching filter to the entries.
        void applyFilter(std::string const& pattern)
        {
            if(m_need_update || (m_last_filter_pattern != pattern))
            {
                m_last_filter_pattern = pattern;
                if(m_last_filter_pattern == "")
                {
                    m_filtered_entries = m_entries;
                    return;
                }
                
                char const* pattern_cstr = m_last_filter_pattern.c_str();
                
                struct ScoredEntry
                {
                    ScoredEntry(std::string const& _str, int _score) : string(_str), score(_score) {}
                    bool operator<(ScoredEntry const& entry) const { return (score > entry.score); }
                    
                    std::string string = "";
                    int score = 0;
                };
                
                std::set<ScoredEntry> scored_entries;
                
                for(auto const& str : m_entries)
                {
                    const auto r = computeScore(pattern_cstr, str.c_str(), m_options);
                    if(r.first)
                    {
                        scored_entries.insert({str, r.second});
                    }
                }
                
                m_filtered_entries.clear();
                
                for(auto const& scored_entry : scored_entries)
                {
                    m_filtered_entries.emplace_back(std::move(scored_entry.string));
                }
                
                m_need_update = false;
            }
        }
        
    private: // methods
        
        //! @brief Returns true if each character in pattern is found sequentially within str
        //! @details if found then score is also set.
        //! Score value has no intrinsic meaning. Range varies with pattern.
        //! You should only compare scores with same search pattern.
        //! @remark The code of this method is an adaptation of the Forrest Smith code described in this article :
        //! https://blog.forrestthewoods.com/reverse-engineering-sublime-text-s-fuzzy-match-4cffeed33fdb
        static std::pair<bool, int> computeScore(char const* pattern, char const* str,
                                                 Options const& options)
        {
            int score = 0;
            char const* pattern_iter = pattern;
            char const* str_iter = str;
            bool prev_matched = false;
            
            // Use "best" matched letter if multiple string letters match the pattern
            char const* best_letter = nullptr;
            int best_letter_score = 0;
            
            // Loop over chars
            while(*str_iter != end_char)
            {
                const char pattern_letter = *pattern_iter;
                const char str_letter = *str_iter;
                const char pattern_letter_lower = tolower(pattern_letter);
                const char str_letter_lower = tolower(str_letter);
                
                const bool pattern_valid_char = (pattern_letter != end_char);
                const bool next_match = pattern_valid_char && (pattern_letter_lower == str_letter_lower);
                const bool rematch = best_letter && (tolower(*best_letter) == str_letter_lower);
                const bool advanced = (next_match && best_letter);
                const bool pattern_repeat = best_letter && pattern_valid_char && (tolower(*best_letter) == pattern_letter_lower);
                
                if(advanced || pattern_repeat)
                {
                    score += best_letter_score;
                    best_letter = nullptr;
                    best_letter_score = 0;
                }
                
                if(next_match || rematch)
                {
                    int new_score = 0;
                    
                    // Apply penalty for each letter before the first pattern match
                    if(pattern_iter == pattern)
                    {
                        const int count = static_cast<int>(str_iter - str);
                        int penalty = options.leading_letter_penalty * count;
                        if(penalty < options.max_leading_letter_penalty)
                            penalty = options.max_leading_letter_penalty;
                        
                        score += penalty;
                    }
                    
                    // Apply bonus for consecutive bonuses
                    if (prev_matched)
                        new_score += options.adjacency_bonus;
                    
                    // Update pattern iter if the next pattern letter was matched
                    if (next_match)
                        ++pattern_iter;
                    
                    // Update best letter in str which may be for a "next" letter or a rematch
                    if (new_score >= best_letter_score)
                    {
                        // Apply penalty for now skipped letter
                        if (best_letter != nullptr)
                            score += options.unmatched_letter_penalty;
                        
                        best_letter = str_iter;
                        best_letter_score = new_score;
                    }
                    
                    prev_matched = true;
                }
                else
                {
                    score += options.unmatched_letter_penalty;
                    prev_matched = false;
                }
                
                ++str_iter;
            }
            
            // Apply score for last match
            if(best_letter)
            {
                score += best_letter_score;
            }
            
            // Did not match full pattern
            if(*pattern_iter != end_char)
            {
                return {false, 0};
            }
            
            return {true, score};
        }
        
        void updateFilteredEntries()
        {
            m_need_update = true;
            applyFilter(m_last_filter_pattern);
        }
        
    private: // members
        
        static constexpr char end_char = '\0';
        
        Options     m_options = {};
        entries_t   m_entries = {};
        entries_t   m_filtered_entries = {};
        std::string m_last_filter_pattern = "";
        bool        m_need_update = true;
    };
}

#endif // KIWI_KIWIAPP_SUGGEST_LIST_HPP_INCLUDED
