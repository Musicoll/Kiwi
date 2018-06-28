/*
  ==============================================================================

    FaustCodeTokenizer.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class FaustTokeniser   : public juce::CodeTokeniser
{
public:
  //==============================================================================
  FaustTokeniser() = default;
  ~FaustTokeniser() = default;
  
  //==============================================================================
  int readNextToken (juce::CodeDocument::Iterator&) override;
  juce::CodeEditorComponent::ColourScheme getDefaultColourScheme() override;
  
  /** The token values returned by this tokeniser. */
  enum TokenType
  {
      tokenType_error = 0,
      tokenType_comment,
      tokenType_primitive,
      tokenType_operator,
      tokenType_identifier,
      tokenType_integer,
      tokenType_float,
      tokenType_string,
      tokenType_bracket,
      tokenType_punctuation,
      tokenType_rafter
  };
  
private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FaustTokeniser)
};


