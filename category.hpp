#ifndef CATEGORY_HPP
#define CATEGORY_HPP

class Category {
private:
	int category;
	double value;
public:
	Category(int cat, double val);

	bool operator<(Category& cat);
	bool operator>(Category& cat);
	bool operator<=(Category& cat);
	bool operator>=(Category& cat);
	bool operator==(Category& cat);
	bool operator!=(Category& cat);
};

#endif
