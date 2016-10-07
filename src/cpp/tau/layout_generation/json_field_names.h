// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include <string>
namespace tau {
namespace layout_generation {

struct JsonFieldNames
{
	static const char * VERSION_()			{ static const char * result = "version";					return result; }
	static const char * STARTING_LAYOUT_()	{ static const char * result = "startLayoutID";				return result; }
	static const char * HEIGHT_()			{ static const char * result = "defaultLayoutHeight";			return result; }
	static const char * TITLE_()			{ static const char * result = "title";						return result; }
	static const char * ID_()				{ static const char * result = ID_ALIAS_();						return result; }
	static const char * LAYOUTS_ARRAY_()	{ static const char * result = "layouts";						return result; }
	struct Layout{
		static const char * LAYOUT_ID_()	{ static const char * result = "layoutID";		return result; }
		static const char * WIDTH_()		{ static const char * result = "layoutWidth";		return result; }
		static const char * HEIGHT_()		{ static const char * result = "layoutHeight";	return result; }
		static const char * LAYOUT_DATA_()	{ static const char * result = "layoutData";		return result; }
		struct LayoutData{
			static const char * ID_()				{ static const char * result = ID_ALIAS_();				return result; }
			static const char * BUTTON_OBJECT_()	{ static const char * result = "ButtonObject";	return result; }
			static const char * VOID_SPACE_OBJECT_(){ static const char * result = "VoidSpaceObject"; return result; }
			static const char * CONTAINER_OBJECT_() { static const char * result = "ContainerObject"; return result; }
			static const char * TEXT_BOX_OBJECT_()	{ static const char * result = "TextBoxObject";	return result; }
			static const char * CLIENT_INPUT_OBJECT_(){ static const char * result = "ClientInputObject"; return result; }
			static const char * IMAGE_BOX_OBJECT_() { static const char * result = "ImageBoxObject"; return result; }

			struct ButtonObject{
				static const char * IS_ENABLED_()                   { static const char * result = IS_ENABLED_FLAG_ALIAS_();	return result; }
				static const char * TEXT_()							{ static const char * result = TEXT_VARIABLE_ALIAS_();	return result; }
				static const char * SWITCH_LAYOUT_ON_CLICK_()		{ static const char * result = "switchLayoutOnClick";		return result; }
				static const char * INFORM_SERVER_ABOUT_VALUES_()	{ static const char * result = "informServerAboutValues"; return result; }
			};
			struct VoidSpaceObject{};
			struct ContainerObject{
				static const char * SPLIT_DIRECTION_()			{ static const char * result = "splitDirection";			return result; }
				static const char * EVENLY_SPLIT_COLLECTION_()	{ static const char * result = "EvenlySplitCollection";	return result; }
				static const char * UNEVENLY_SPLIT_PAIR_()		{ static const char * result = "UnevenlySplitPair";		return result; }
				struct EvenlySplitCollection{			
					static const char * SUBREGIONS_()		{ static const char * result = "subregions"; return result; }
				};
				struct UnevenlySplitPair{
					static const char * SPLIT_RATIO_()		{ static const char * result = "splitRatio";		return result; }
					static const char * FIRST_SUBREGION_()	{ static const char * result = "firstSubregion";	return result; }
					static const char * SECOND_SUBREGION_() { static const char * result = "secondSubregion"; return result; }
				};
			};
			struct TextBoxObject{
				static const char * TEXT_()					{ static const char * result = TEXT_VARIABLE_ALIAS_(); return result; }
			};
			struct ClientInputObject{
				static const char * IS_ENABLED_()           { static const char * result = IS_ENABLED_FLAG_ALIAS_();	return result; }

				struct TextInputObject {					
					static const char * INITIAL_VALUE_()		{ static const char * result = INITIAL_VALUE_ALIAS_(); return result; }
					static const char * LENGTH_LIMIT_()			{ static const char * result = "stringLengthLimit"; return result; }
				};
				static const char * TEXT_INPUT_OBJECT_()	{ static const char * result = "textInputObject"; return result; }

				struct NumericInputObject {
					static const char * INITIAL_VALUE_()	{ static const char * result = INITIAL_VALUE_ALIAS_(); return result; }
					
					static const char * IS_FLOAT_POINT_()	{ static const char * result = "floatPoint"; return result; }
					static const char * IS_SIGNED_()		{ static const char * result = "signed"; return result; }

					static const char * LOWER_BOUND_()		{ static const char * result = "lowerBound"; return result; }
					static const char * UPPER_BOUND_()		{ static const char * result = "upperBound"; return result; }
					struct NumericInputObjectBound {
						static const char * VALUE_()		{ static const char * result = "value"; return result; }
						static const char * IS_INCLUSIVE_()	{ static const char * result = "isInclusive"; return result; }
					};

				};
				static const char * NUMERIC_INPUT_OBJECT_()	{ static const char * result = "numericInputObject"; return result; }

				struct BooleanInputObject {
					static const char * INITIAL_VALUE_()	{ static const char * result = INITIAL_VALUE_ALIAS_(); return result; }
					static const char * TEXT_()				{ static const char * result = TEXT_VARIABLE_ALIAS_(); return result; }
				};
				static const char * BOOL_INPUT_OBJECT_()	{ static const char * result = "booleanInputObject"; return result; }
				
				static const char * NOTIFY_SERVER_ABOUT_MODIFICATIONS_()	{ static const char * result = "notifyServerAboutModifications"; return result; }
			};
			struct ImageBoxObject{
				static const char * IMAGE_ID_() { static const char * result = "displayedImage"; return result; }
			};
		};
	};
	private: // reused string constants
		static const char * ID_ALIAS_()				 {static const char * result = "id"; return result; }
		static const char * TEXT_VARIABLE_ALIAS_()   {static const char * result = "text"; return result; }
		static const char * INITIAL_VALUE_ALIAS_()   {static const char * result = "initialValue"; return result; }
		static const char * IS_ENABLED_FLAG_ALIAS_() {static const char * result = "isEnabled"; return result; }
};

struct JsonValues
{
	static const char * TRUE_()		{static const char * result = "true";		return result; }
	static const char * FALSE_()	{static const char * result = "false";	return result; }
	struct SplitDirection {
		static const char * VERTICAL_()		{static const char * result = "vertical";		return result; }
		static const char * HORIZONTAL_()	{static const char * result = "horizontal";	return result; }
	};
	static std::string getBooleanValueDeclaration(std::string const & jsonName, bool value);
};

}; // namespace layout_generation
}; // namespace tau
#ifdef TAU_HEADERONLY
#include "json_field_names.cpp"
#endif// TAU_HEADERONLY

