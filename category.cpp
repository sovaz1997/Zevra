#include "category.hpp"

Category::Category(int cat, double val) : category(cat), value(val) {}

bool Category::operator<(Category& cat) {
	if(category < cat.category) {
		return true;
	} else if(category > cat.category) {
		return false;
	}

	return value < cat.value;
}

bool Category::operator>(Category& cat) {
	if(category > cat.category) {
		return true;
	} else if(category < cat.category) {
		return false;
	}

	return value > cat.value;
}

bool Category::operator<=(Category& cat) {
	if(category < cat.category) {
		return true;
	} else if(category > cat.category) {
		return false;
	}

	return value <= cat.value;
}

bool Category::operator>=(Category& cat) {
	if(category > cat.category) {
		return true;
	} else if(category < cat.category) {
		return false;
	}

	return value >= cat.value;
}

bool Category::operator==(Category& cat) {
	return (category == cat.category && value == cat.value);
}

bool Category::operator!=(Category& cat) {
	return (category != cat.category || value != cat.value);
}
