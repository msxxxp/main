#ifndef A_HPP_
#define A_HPP_

class A {
public:
	~A();
	A();
	explicit A(int a);
	A(const A & other);
	A(A && other);

	A & operator =(const A & other);
	A & operator =(A && other);

	A operator +(const A & other) const;

	void swap(A & other);

	int val() const;

private:
	int m_a;
};

#endif
