/*
#include <simstd/string>

#include <cassert>
#include <string>

ssize_t simstd::Test::a_string(simstd::Test::aPrintFunc printFunc)
{
	{
		string asd("newGoodString");
		std::string stdasd("newGoodString");
		asd.replace(3, 4, "Super", 5);
		stdasd.replace(3, 4, "Super", 5);
		printFunc("asd (%Iu, %Iu): '%s'\n", asd.size(), asd.capacity(), asd.c_str());
		printFunc("stdasd (%Iu, %Iu): '%s'\n", stdasd.size(), stdasd.capacity(), stdasd.c_str());
		assert(Cstr::compare(asd.c_str(), stdasd.c_str()) == 0);
	}
	{
		string asd("newGoodString");
		std::string stdasd("newGoodString");
		asd.replace(3, 4, "Bad", 3);
		stdasd.replace(3, 4, "Bad", 3);
		printFunc("asd (%Iu, %Iu): '%s'\n", asd.size(), asd.capacity(), asd.c_str());
		printFunc("stdasd (%Iu, %Iu): '%s'\n", stdasd.size(), stdasd.capacity(), stdasd.c_str());
		assert(Cstr::compare(asd.c_str(), stdasd.c_str()) == 0);
	}
	{
		string asd("newGoodString");
		std::string stdasd("newGoodString");
		asd.replace(3, 0, "Super", 5);
		stdasd.replace(3, 0, "Super", 5);
		printFunc("asd (%Iu, %Iu): '%s'\n", asd.size(), asd.capacity(), asd.c_str());
		printFunc("stdasd (%Iu, %Iu): '%s'\n", stdasd.size(), stdasd.capacity(), stdasd.c_str());
		assert(Cstr::compare(asd.c_str(), stdasd.c_str()) == 0);
	}
	{
		string asd("Iam");
		asd.replace(3, 444, "Super", 8);
		printFunc("asd (%Iu, %Iu): '%s'\n", asd.size(), asd.capacity(), asd.c_str());
		std::string stdasd("Iam");
		stdasd.replace(3, 444, "Super", 8);
		printFunc("stdasd (%Iu, %Iu): '%s'\n", stdasd.size(), stdasd.capacity(), stdasd.c_str());
		assert(Cstr::compare(asd.c_str(), stdasd.c_str()) == 0);
	}
	{
		string asd("IamStar");
		asd.replace(3, 0, "SuperStar", 5);
		printFunc("asd (%Iu, %Iu): '%s'\n", asd.size(), asd.capacity(), asd.c_str());
		std::string stdasd("IamStar");
		stdasd.replace(3, 0, "SuperStar", 5);
		printFunc("stdasd (%Iu, %Iu): '%s'\n", stdasd.size(), stdasd.capacity(), stdasd.c_str());
		assert(Cstr::compare(asd.c_str(), stdasd.c_str()) == 0);
	}
	{
		string asd("IamSuperStar");
		asd.replace(3, 0, asd.c_str() + 3, 5);
		printFunc("asd (%Iu, %Iu): '%s'\n", asd.size(), asd.capacity(), asd.c_str());
		std::string stdasd("IamSuperStar");
		stdasd.replace(3, 0, stdasd.c_str() + 3, 5);
		printFunc("stdasd (%Iu, %Iu): '%s'\n", stdasd.size(), stdasd.capacity(), stdasd.c_str());
		assert(Cstr::compare(asd.c_str(), stdasd.c_str()) == 0);
	}

	const char * cc = "const char *";

	string a;
	string b("qwerty12345");
	string c(b);

	printFunc("cc: '%s'\n", cc);

	printFunc("a (%Iu, %Iu): '%s'\n", a.size(), a.capacity(), a.c_str());
	printFunc("b (%Iu, %Iu): '%s'\n", b.size(), b.capacity(), b.c_str());

	a = b;
	printFunc("c (%Iu, %Iu): '%s'\n", c.size(), c.capacity(), c.c_str());
	printFunc("a (%Iu, %Iu): '%s'\n", a.size(), a.capacity(), a.c_str());

	string str1(5, 'h');
	std::string stdstr1(5, 'h');
	printFunc("str1: '%s' size: %d\n", str1.c_str(), (int)str1.size());
	assert(Cstr::compare(str1.c_str(), stdstr1.c_str()) == 0);
	assert(Cstr::compare(str1.c_str(), "hhhhh") == 0);
	assert(str1.size() == 5);
	assert(*(str1.cend() - 1) == 'h');
	assert(*str1.cend() == 0);
	assert(*str1.cbegin() == 'h');

	const char * pcwstr = "PCWSTR";
	string str2(pcwstr);
	std::string stdstr2(pcwstr);
	printFunc("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), pcwstr) == 0);
	assert(str2.size() == 6);
	assert(*(str2.cend() - 1) == 'R');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'P');

	char * pwstr = (char *)"1pwstr2";
	string str3 = pwstr;
	std::string stdstr3 = pwstr;
	printFunc("str3: '%s' size: %d\n\n", str3.c_str(), (int)str3.size());
	assert(Cstr::compare(str3.c_str(), stdstr3.c_str()) == 0);
	assert(Cstr::compare(str3.c_str(), pwstr) == 0);
	assert(str3.size() == 7);
	assert(*(str3.cend() - 1) == '2');
	assert(*str3.cend() == 0);
	assert(*str3.cbegin() == '1');

	str1 = "que1";
	stdstr1 = "que1";
	printFunc("str1: '%s' size: %d\n", str1.c_str(), (int)str1.size());
	assert(Cstr::compare(str1.c_str(), stdstr1.c_str()) == 0);
	assert(Cstr::compare(str1.c_str(), "que1") == 0);
	assert(str1.size() == 4);
	assert(*(str1.cend() - 1) == '1');
	assert(*str1.cend() == 0);
	assert(*str1.cbegin() == 'q');

	str2 = str1;
	stdstr2 = stdstr1;
	printFunc("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "que1") == 0);
	assert(str2.size() == 4);
	assert(*(str2.cend() - 1) == '1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'q');
	assert(str1.c_str() == str2.c_str());

	str3.clear();
	stdstr3.clear();
	printFunc("str3: '%s' size: %d\n\n", str3.c_str(), (int)str3.size());
	assert(Cstr::compare(str3.c_str(), stdstr3.c_str()) == 0);
	assert(Cstr::compare(str3.c_str(), "") == 0);
	assert(str3.size() == 0);
//		assert(*(str3.cend()-1) == '2');
	assert(*str3.cend() == 0);
	assert(*str3.cbegin() == 0);
	assert(str3.empty());
	assert(str3.cbegin() == str3.cend());

	str2.append(str1);
	stdstr2.append(stdstr1);
	printFunc("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "que1que1") == 0);
	assert(str2.size() == 8);
	assert(*(str2.cend() - 1) == '1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'q');
	assert(str1.c_str() != str2.c_str());

	str2.replace(4, 0, "test2");
	stdstr2.replace(4, 0, "test2");
	printFunc("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "que1test2que1") == 0);
	assert(str2.size() == 13);
	assert(*(str2.cend() - 1) == '1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'q');
	assert(str2.find("tes") == 4);
	assert(str2.find("teqs") == string::npos);
	assert(str2.find("ue") == 1);
	assert(str2.rfind('u') == 10);
	assert(str2.find_first_of("12") == 3);
	assert(str2.find_first_not_of("12") == 0);
	assert(str2.find_first_of("21") == 3);
	assert(str2.find_first_not_of("21") == 0);
	assert(str2.find_first_not_of("que1") == 4);
	assert(str2.find_last_of("12") == 12);
	assert(str2.find_last_of("2d") == 8);
	assert(str2.find_last_not_of("12") == 11);
	assert(str2.find_last_not_of("2d") == 12);

	str2.replace(4, 5, "TEST3");
	stdstr2.replace(4, 5, "TEST3");
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "que1TEST3que1") == 0);
	assert(str2.size() == 13);

	str2.replace(0, 512, "T4");
	stdstr2.replace(0, 512, "T4");
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "T4") == 0);
	assert(str2.size() == 2);

	str2.replace(2, 128, "testing5");
	stdstr2.replace(2, 128, "testing5");
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "T4testing5") == 0);
	assert(str2.size() == 10);

	str2.replace(0, 1024, "");
	stdstr2.replace(0, 1024, "");
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "") == 0);
	assert(str2.size() == 0);

	str2.replace(0, 1, "again6");
	stdstr2.replace(0, 1, "again6");
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "again6") == 0);
	assert(str2.size() == 6);

	str2.erase(4, 20);
	stdstr2.erase(4, 20);
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "agai") == 0);
	assert(str2.size() == 4);

	str2.erase(str2.cbegin() + 2);
	stdstr2.erase(stdstr2.begin() + 2);
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);
	assert(Cstr::compare(str2.c_str(), "agi") == 0);
	assert(str2.size() == 3);

	str2.append(str2);
	stdstr2.append(stdstr2);
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	printFunc("stdstr2: '%s' size: %d capacity: %d\n", stdstr2.c_str(), (int)stdstr2.size(), (int)stdstr2.capacity());
	str2.append(str2);
	stdstr2.append(stdstr2);
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	printFunc("stdstr2: '%s' size: %d capacity: %d\n", stdstr2.c_str(), (int)stdstr2.size(), (int)stdstr2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);

	const char * qqq = str2.c_str() + 3;
	str2.append(qqq, 5);
	stdstr2.append(qqq, 5);
	printFunc("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	printFunc("stdstr2: '%s' size: %d capacity: %d\n", stdstr2.c_str(), (int)stdstr2.size(), (int)stdstr2.capacity());
	assert(Cstr::compare(str2.c_str(), stdstr2.c_str()) == 0);

	return 0;
}
*/
