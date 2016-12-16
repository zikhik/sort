#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

struct line
{
	string name;
	string surname;
	short year;
	size_t size() const
	{
		return (10 + name.size() + 10 + surname.size() + sizeof(int));
	}
};

bool operator < (const line& line1, const line& line2)
{
	return (line1.name < line2.name);
}

bool operator >(const line& line1, const line& line2)
{
	return (line1.name > line2.name);
}

ostream & operator<<(ostream & output, line const & str)
{
	output << str.surname << " " << str.name << " " << str.year;
	return output;
}

istream & operator>>(istream & input, line & str)
{
	input >> str.surname >> str.name >> str.year;
	return input;
}

bool operator != (line s, string str)
{
	return (s.surname != str);
}

struct constr
{
	line line_;
	ifstream *f;
	constr(const line& s_, ifstream* f_) : line_(s_), f(f_) {}
};

bool operator < (const constr& constr1, const constr& constr2)
{
	return (constr1.line_ > constr2.line_);
}

void sortirovka(const string input_name, const string output_name, const short mem_size)
{
	ifstream fin(input_name);
	if (!fin.is_open()) throw("file_not_open");
	ofstream fout(output_name);
	short k = 0;
	while (!fin.eof())
	{
		vector<line> v; line s;
		ofstream fout_(to_string(k + 1));
		for (unsigned long int size = 0; (size + 50) < mem_size * 1024 * 1024 * 0.52;)
		{
			if (!fin.eof() && (fin >> s) && (s != ""))  v.push_back(s);
			size += s.size();
		}
		sort(v.begin(), v.end());
		for (auto i : v)
		{
			if (i != "") fout_ << i << endl;
		}
		++k;
		fout_.close();
	}
	fin.close();
	priority_queue<constr> pq;
	for (size_t i = 0; i < k; ++i)
	{
		ifstream* f_ = new ifstream(to_string(i + 1));
		line str;
		*f_ >> str;
		constr si(str, f_);
		pq.push(si);
	}
	while (!pq.empty())
	{
		constr si = pq.top();
		pq.pop();
		if (si.line_ != "") fout << si.line_ << endl;
		if (!(*si.f).eof() && (*si.f >> si.line_))
		{
			pq.push(si);
		}
		else
		{
			(*(si.f)).close();
		}
	}
	for (size_t i = 0; i < k; ++i)
	{
		remove((to_string(i + 1)).c_str());
	}
	fout.close();
}
