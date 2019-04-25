// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once
#include "../common/element_id.h"
#include "simple_json_builder.h"
#include "my_shared_ptr.h"
#include "json_field_names.h"
#include <string>
#include <vector>

namespace tau {
namespace layout_generation {

class AbstractLayoutElement
{
public:
	virtual ~AbstractLayoutElement() {};
	virtual void getDeclarationDump(simple_json_builder & target) const = 0;
};

template <typename FinalChildType>
	class RectLayoutElement:
		public AbstractLayoutElement
	{
		common::ElementID m_id;
	public:
		RectLayoutElement():m_id("") {};

		FinalChildType & ID(common::ElementID const & id)
		{
			m_id = id;
			return static_cast<FinalChildType & >(*this);
		};

		virtual void getDeclarationDump(simple_json_builder & target) const
		{
			target.name(JsonFieldNames::Layout::LayoutData::ID_()).pushStringValue(m_id);
		};
	};

	
template <typename FinalChildType>
	class LayoutElementsContainer:
		public RectLayoutElement<FinalChildType>
	{
	protected:
		bool m_elementsOneAboveAnother;

		LayoutElementsContainer(bool elementsAboveAndBelowEachOther):m_elementsOneAboveAnother(elementsAboveAndBelowEachOther)
		{};

		virtual void getDeclarationDump(simple_json_builder & target) const
		{
			RectLayoutElement<FinalChildType>::getDeclarationDump(target);
			target.name(JsonFieldNames::Layout::LayoutData::CONTAINER_OBJECT_());

		//	TODO: get rid of the manual call to startObject(): it is not possible to use the guard here, because the object should not be closed at the end of this function (there are additional fields added later).
			target.startObject();
			target
				.name(JsonFieldNames::Layout::LayoutData::ContainerObject::SPLIT_DIRECTION_())
				.pushStringValue(
					m_elementsOneAboveAnother ? 
						JsonValues::SplitDirection::HORIZONTAL_() :
						JsonValues::SplitDirection::VERTICAL_()
					);
		}
	};

class FramingElementWrapper:
	public RectLayoutElement<FramingElementWrapper>
{
	my_shared_ptr<AbstractLayoutElement>::MyInternalType m_wrapped;
public:
	template <typename T>
	FramingElementWrapper(const T & toWrap) : m_wrapped(my_shared_ptr<T>::createInstance(toWrap)) {};
	virtual void getDeclarationDump(simple_json_builder & target) const;
};

class EvenlySplitLayoutElementsContainer:
	public LayoutElementsContainer<EvenlySplitLayoutElementsContainer>
{
	std::vector<my_shared_ptr<AbstractLayoutElement>::MyInternalType> m_regions;
public:
	EvenlySplitLayoutElementsContainer(bool elementsAboveAndBelowEachOther);

	template <typename T>
	EvenlySplitLayoutElementsContainer & push(const T & element)
	{		
		my_shared_ptr<AbstractLayoutElement>::MyInternalType toAdd = my_shared_ptr<T>::createInstance(element);
		m_regions.push_back(toAdd);
		return *this;
	};
	virtual void getDeclarationDump(simple_json_builder & target) const;
};

class UnevenlySplitElementsPair:
	public LayoutElementsContainer<UnevenlySplitElementsPair>
{
	my_shared_ptr<AbstractLayoutElement>::MyInternalType m_first; // Top or left (depending on split type)
	my_shared_ptr<AbstractLayoutElement>::MyInternalType m_second; // Bottom or right (depending on split type)
	double m_ratio;
public:

	template <typename T, typename U>
	UnevenlySplitElementsPair(const T & first, const U & second, bool elementsAboveAndBelowEachOther, double ratio)
		:LayoutElementsContainer(elementsAboveAndBelowEachOther)
		, m_first(my_shared_ptr<T>::createInstance(first)), m_second(my_shared_ptr<U>::createInstance(second)), m_ratio(ratio)
	{};
	virtual void getDeclarationDump(simple_json_builder & target) const;
};

class EmptySpace:
	public RectLayoutElement<EmptySpace>
{
public:
	EmptySpace() {};
	virtual void getDeclarationDump(simple_json_builder & target) const;
};

class ImageBoxLayoutElement :
	public RectLayoutElement<ImageBoxLayoutElement>
{
	common::ImageID m_imageID;
public:
	ImageBoxLayoutElement():m_imageID("") {};
	ImageBoxLayoutElement(common::ImageID const & id):m_imageID(id) {};
	ImageBoxLayoutElement & imageID(common::ImageID const & id);
	virtual void getDeclarationDump(simple_json_builder & target) const;
};

class LabelElement:
	public RectLayoutElement<LabelElement>
{
	std::string m_initialText;
public:
	LabelElement(std::string const & text):m_initialText(text) {};
	virtual void getDeclarationDump(simple_json_builder & target) const;
};

// Base class for all the elements, with which the user can interact directly:
template <typename FinalChildType>
	class UserInteractionElement:
		public RectLayoutElement<FinalChildType>
	{
	protected:
		bool m_isEnabled;
	public:
		UserInteractionElement():
	  		m_isEnabled(true)
		{};
		
		FinalChildType & setEnabled(bool isEnabled)
		{
			m_isEnabled = isEnabled;
			return static_cast<FinalChildType &>(*this);
		};
	};

template <typename FinalChildType>
	class ClientInputLayoutElement:
		public UserInteractionElement<FinalChildType>
	{
		bool m_shouldNotifyServerOnModification;

	public:
		ClientInputLayoutElement()
			:m_shouldNotifyServerOnModification(true)
		{
		};

		virtual void getDeclarationDump(simple_json_builder & target) const
		{
			RectLayoutElement<FinalChildType>::getDeclarationDump(target);
			target.name(JsonFieldNames::Layout::LayoutData::CLIENT_INPUT_OBJECT_());
			{
				simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
				target
					.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::NOTIFY_SERVER_ABOUT_MODIFICATIONS_())
					.pushBoolValue(m_shouldNotifyServerOnModification);
				target
					.name(JsonFieldNames::Layout::LayoutData::ClientInputObject::IS_ENABLED_())
					.pushBoolValue(UserInteractionElement<FinalChildType>::m_isEnabled);
				getInputDataTypeInfo(target);
			}
		}

		FinalChildType & shouldNotifyOnEdit(bool shouldNotify)
		{
			m_shouldNotifyServerOnModification = shouldNotify;
			return static_cast<FinalChildType &>(*this);
		};

	protected:
		virtual void getInputDataTypeInfo(simple_json_builder & target) const = 0;
	};

class BooleanInputLayoutElement:
	public ClientInputLayoutElement<BooleanInputLayoutElement>
{
	std::string m_note;
	bool m_initialValue;
protected:
	virtual void getInputDataTypeInfo(simple_json_builder & target) const;
public:
	// Note: The initialValue remains as a required parameter because it is not possible to decide on default value.
	// It is both reasonable to have initially checked or unchecked checkBox on the application start.
	// So, the library user should decide on the initial state during construction.
	BooleanInputLayoutElement(bool checkedInitially):
		m_initialValue(checkedInitially)
	{};	
	BooleanInputLayoutElement & note(std::string const & note);
};

class NumberInputLayoutElement:
	public ClientInputLayoutElement<NumberInputLayoutElement>
{
	struct NumericBoundary {
		bool isSet;
		bool isInclusive;
		double boundaryValue;
		NumericBoundary():isSet(false), isInclusive(false), boundaryValue(0) {}
		void setValue(double value, bool inclusive)
		{
			isSet = true;
			isInclusive = inclusive;
			boundaryValue = value;		
		};
		void clearValue()
		{
			isSet = false;
		};
	};
	bool m_allowNegatives;
	bool m_allowFloatPoint;
	double m_initialValue;
	NumericBoundary m_lowerBoundary;
	NumericBoundary m_upperBoundary;
private:
	void utility_appendJsonForBoundary(simple_json_builder & target, std::string const & nameToUse, const NumberInputLayoutElement::NumericBoundary & boundary) const;
protected:
	virtual void getInputDataTypeInfo(simple_json_builder & target) const;
public:
	NumberInputLayoutElement(double initialValue):
		m_allowNegatives(true), m_allowFloatPoint(false), m_initialValue(initialValue)
	{};
	
	NumberInputLayoutElement & isSigned(bool isSigned);
	NumberInputLayoutElement & allowFloatPoint(bool isFloatPoint);
	NumberInputLayoutElement & lowerBoundary(double value, bool isInclusive);
	NumberInputLayoutElement & clearLowerBoundary();
	NumberInputLayoutElement & upperBoundary(double value, bool isInclusive);
	NumberInputLayoutElement & clearUpperBoundary();
};

class TextInputLayoutElement:
	public ClientInputLayoutElement<TextInputLayoutElement>
{
	std::string m_initialValue;
	size_t m_lengthLimit;
	bool m_lengthLimitSet;
protected:
	virtual void getInputDataTypeInfo(simple_json_builder & target) const;
public:
	TextInputLayoutElement():
		ClientInputLayoutElement(), m_lengthLimit(0), m_lengthLimitSet(false)
	{};
	TextInputLayoutElement & initialValue(std::string const & value);
	TextInputLayoutElement & setLengthLimit(size_t limit);
};

struct ColorChannels
{
	int m_r;
	int m_g;
	int m_b;
	ColorChannels()
		: m_r(0), m_g(0), m_b(0) {};
	ColorChannels(int r, int g, int b)
		: m_r(r), m_g(g), m_b(b) {};
	ColorChannels(ColorChannels const & other)
		: m_r(other.m_r), m_g(other.m_g), m_b(other.m_b) {};
};

struct BackgroundInfo
{
	bool isValid;
	ColorChannels data;
	BackgroundInfo(): isValid(false) {};
	BackgroundInfo(ColorChannels const & color): isValid(true), data(color) {};
};

class ButtonLayoutElement:
	public UserInteractionElement<ButtonLayoutElement>
{
	std::string m_text;
	BackgroundInfo m_customBackground;
	BackgroundInfo m_customBackgroundOnClick;
	
	std::vector<common::ElementID> m_valuesToInformServerAbout;
	common::LayoutPageID m_switchToAnotherLayoutPageID;
	common::ImageID m_imageID;
public:
	ButtonLayoutElement() : m_switchToAnotherLayoutPageID(""), m_imageID("") {};

	ButtonLayoutElement & switchToAnotherLayoutPageOnClick(common::LayoutPageID const & targetLayoutPageID);
	ButtonLayoutElement & informServerAboutValueOnClick(common::ElementID const & elementIdForValueInput);
	ButtonLayoutElement & note(std::string const & text);
	ButtonLayoutElement & imageID(common::ImageID const & imageID);
	ButtonLayoutElement & background(BackgroundInfo const & background);
	ButtonLayoutElement & pressedDownBackground(BackgroundInfo const & background);
	
	virtual void getDeclarationDump(simple_json_builder & target) const;
};
}; // namespace tau::layout_generation
}; // namespace tau

#ifdef TAU_HEADERONLY
#include "layout_elements.cpp"
#endif// TAU_HEADERONLY

