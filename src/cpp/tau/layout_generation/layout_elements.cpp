// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "regions.h"
#endif

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace layout_generation {

LINKAGE_RESTRICTION EvenlySplitLayoutElementsContainer::EvenlySplitLayoutElementsContainer(bool elementsAboveAndBelowEachOther):
	LayoutElementsContainer(elementsAboveAndBelowEachOther)
{}

LINKAGE_RESTRICTION void EvenlySplitLayoutElementsContainer::getDeclarationDump(simple_json_builder & target) const
{
	LayoutElementsContainer::getDeclarationDump(target);
	target.name(JsonFieldNames::Layout::LayoutData::ContainerObject::EVENLY_SPLIT_COLLECTION_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		target.name(JsonFieldNames::Layout::LayoutData::ContainerObject::EvenlySplitCollection::SUBREGIONS_());
		{
			simple_json_builder_array_scoped_guard guard = target.getArrayScopedGuard();
			for (size_t i = 0; i < m_regions.size(); ++i) {
				simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
				m_regions[i].get()->getDeclarationDump(target);
			}
		}
	}
	target.endObject(); // close the bracket opened in the LayoutElementsContainer::getDeclarationDump()
}

LINKAGE_RESTRICTION void UnevenlySplitElementsPair::getDeclarationDump(simple_json_builder & target) const
{
	LayoutElementsContainer::getDeclarationDump(target);
	target.name(JsonFieldNames::Layout::LayoutData::ContainerObject::UNEVENLY_SPLIT_PAIR_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		target.name(JsonFieldNames::Layout::LayoutData::ContainerObject::UnevenlySplitPair::SPLIT_RATIO_()).pushDoubleValue(m_ratio);

		target.name(JsonFieldNames::Layout::LayoutData::ContainerObject::UnevenlySplitPair::FIRST_SUBREGION_());
		{
			simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
			m_first.get()->getDeclarationDump(target);
		}
				
		target.name(JsonFieldNames::Layout::LayoutData::ContainerObject::UnevenlySplitPair::SECOND_SUBREGION_());
		{
			simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
			m_second.get()->getDeclarationDump(target);
		}
	}
	target.endObject(); // close the bracket opened in the LayoutElementsContainer::getDeclarationDump()
}

LINKAGE_RESTRICTION void EmptySpace::getDeclarationDump(simple_json_builder & target) const
{	
	RectLayoutElement::getDeclarationDump(target);
	target.name(JsonFieldNames::Layout::LayoutData::VOID_SPACE_OBJECT_());
	target.startObject();
	target.endObject();
}

LINKAGE_RESTRICTION ImageBoxLayoutElement & ImageBoxLayoutElement::imageID(common::ImageID const & id)
{
	m_imageID = id;
	return *this;
}

LINKAGE_RESTRICTION void ImageBoxLayoutElement::getDeclarationDump(simple_json_builder & target) const
{
	RectLayoutElement::getDeclarationDump(target);
	target.name(JsonFieldNames::Layout::LayoutData::IMAGE_BOX_OBJECT_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		if (m_imageID.getValue().size() > 0) {
			target.name(JsonFieldNames::Layout::LayoutData::ImageBoxObject::IMAGE_ID_()).pushStringValue(m_imageID.getValue());
		}		
	}
}

LINKAGE_RESTRICTION void LabelElement::getDeclarationDump(simple_json_builder & target) const
{
	RectLayoutElement::getDeclarationDump(target);
	target.name(JsonFieldNames::Layout::LayoutData::TEXT_BOX_OBJECT_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		target.name(JsonFieldNames::Layout::LayoutData::TextBoxObject::TEXT_()).pushStringValue(m_initialText);
	}
}

LINKAGE_RESTRICTION	BooleanInputLayoutElement & BooleanInputLayoutElement::note(std::string const & note)
{
	m_note = note;
	return *this;
}

LINKAGE_RESTRICTION	void BooleanInputLayoutElement::getInputDataTypeInfo(simple_json_builder & target) const
{
	target.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::BOOL_INPUT_OBJECT_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		target
			.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::BooleanInputObject::INITIAL_VALUE_())
			.pushBoolValue(m_initialValue)
			.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::BooleanInputObject::TEXT_())
			.pushStringValue(m_note);
	}
}

LINKAGE_RESTRICTION	NumberInputLayoutElement & NumberInputLayoutElement::isSigned(bool isSigned)
{
	m_allowNegatives = isSigned;
	return *this;
}

LINKAGE_RESTRICTION	NumberInputLayoutElement & NumberInputLayoutElement::allowFloatPoint(bool isFloatPoint)
{
	m_allowFloatPoint = isFloatPoint;
	return *this;
}

LINKAGE_RESTRICTION void NumberInputLayoutElement::utility_appendJsonForBoundary(simple_json_builder & target, std::string const & nameToUse, const NumberInputLayoutElement::NumericBoundary & boundary) const
{
	if (boundary.isSet) {
		target.name(nameToUse);
		{
			simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
			target
				.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::NumericInputObject::NumericInputObjectBound::VALUE_())
				.pushDoubleValue(boundary.boundaryValue)
				.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::NumericInputObject::NumericInputObjectBound::IS_INCLUSIVE_())
				.pushBoolValue(boundary.isInclusive);
		}
	}
}

LINKAGE_RESTRICTION void NumberInputLayoutElement::getInputDataTypeInfo(simple_json_builder & target) const
{
	target.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::NUMERIC_INPUT_OBJECT_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		target
			.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::NumericInputObject::INITIAL_VALUE_())
			.pushDoubleValue(m_initialValue)
			.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::NumericInputObject::IS_FLOAT_POINT_())
			.pushBoolValue(m_allowFloatPoint)
			.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::NumericInputObject::IS_SIGNED_())
			.pushBoolValue(m_allowNegatives);
		utility_appendJsonForBoundary(target, 
			JsonFieldNames::Layout::LayoutData::ClientInputObject::NumericInputObject::LOWER_BOUND_(), m_lowerBoundary);
		utility_appendJsonForBoundary(target,
			JsonFieldNames::Layout::LayoutData::ClientInputObject::NumericInputObject::UPPER_BOUND_(), m_upperBoundary);
	}
}

LINKAGE_RESTRICTION	NumberInputLayoutElement & NumberInputLayoutElement::lowerBoundary(double value, bool isInclusive)
{
	m_lowerBoundary.setValue(value, isInclusive);
	return *this;
}

LINKAGE_RESTRICTION NumberInputLayoutElement & NumberInputLayoutElement::upperBoundary(double value, bool isInclusive)
{
	m_upperBoundary.setValue(value, isInclusive);
	return *this;
}

LINKAGE_RESTRICTION NumberInputLayoutElement & NumberInputLayoutElement::clearUpperBoundary()
{
	m_upperBoundary.clearValue();
	return *this;
}

LINKAGE_RESTRICTION NumberInputLayoutElement & NumberInputLayoutElement::clearLowerBoundary()
{
	m_lowerBoundary.clearValue();
	return *this;
}

LINKAGE_RESTRICTION void TextInputLayoutElement::getInputDataTypeInfo(simple_json_builder & target) const
{
	target.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::TEXT_INPUT_OBJECT_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		target
			.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::TextInputObject::INITIAL_VALUE_())
			.pushStringValue(m_initialValue);
		if (m_lengthLimitSet) {
			target
				.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::TextInputObject::LENGTH_LIMIT_())
				.pushIntValue((int)m_lengthLimit);
		}
	}
}

LINKAGE_RESTRICTION TextInputLayoutElement & TextInputLayoutElement::setLengthLimit(size_t limit)
{
	m_lengthLimit = limit;
	m_lengthLimitSet = true;
	return *this;
}

LINKAGE_RESTRICTION TextInputLayoutElement & TextInputLayoutElement::initialValue(std::string const & value)
{
	m_initialValue = value;
	return *this;
}

LINKAGE_RESTRICTION void ButtonLayoutElement::getDeclarationDump(simple_json_builder & target) const
{
	RectLayoutElement::getDeclarationDump(target);
	target.name(JsonFieldNames::Layout::LayoutData::BUTTON_OBJECT_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		target
			.name(JsonFieldNames::Layout::LayoutData::ButtonObject::TEXT_())
			.pushStringValue(m_text);
		
		//TODO : rethink the storing and handling of this isEnabled flag (currently there is duplication of the code, dumping it to json)
		target
			.name(JsonFieldNames::Layout::LayoutData::ButtonObject::IS_ENABLED_())
			.pushBoolValue(m_isEnabled);

		if (m_valuesToInformServerAbout.size() > 0){
			target.name(JsonFieldNames::Layout::LayoutData::ButtonObject::INFORM_SERVER_ABOUT_VALUES_());
			{
				simple_json_builder_array_scoped_guard guard = target.getArrayScopedGuard();
				for (size_t i = 0; i < m_valuesToInformServerAbout.size(); ++i) {
					target.pushStringValue(m_valuesToInformServerAbout[i]);
				}
			}
		}
		if (common::getStringRef(m_switchToAnotherLayoutPageID).size() > 0) {
			target
				.name(JsonFieldNames::Layout::LayoutData::ButtonObject::SWITCH_LAYOUT_ON_CLICK_())
				.pushStringValue(m_switchToAnotherLayoutPageID);
		}
	}
}

LINKAGE_RESTRICTION ButtonLayoutElement & ButtonLayoutElement::switchToAnotherLayoutPageOnClick(common::LayoutPageID const & targetLayoutPageID)
{
	m_switchToAnotherLayoutPageID = targetLayoutPageID;
	return *this;
}

LINKAGE_RESTRICTION ButtonLayoutElement & ButtonLayoutElement::informServerAboutValueOnClick(common::ElementID const & elementIdForValueInput)
{
	m_valuesToInformServerAbout.push_back(elementIdForValueInput);
	return *this;
}

LINKAGE_RESTRICTION ButtonLayoutElement & ButtonLayoutElement::note(std::string const & text)
{
	m_text = text;
	return *this;
}

}; // namespace tau::layout_generation
}; // namespace tau

#undef LINKAGE_RESTRICTION 
