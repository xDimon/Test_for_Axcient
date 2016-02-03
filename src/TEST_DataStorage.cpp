//============================================================================
// Name        : TEST_DataStorage.cpp
// Author      : Dmitriy Khaustov (khaustov.dm@gmail.com)
// Version     :
// Copyright   :
//============================================================================

#include <iostream>

#include "DataStorage.hpp"

using namespace std;

int main() {
	char buff[] = {0, 0, 0};
	cout << "init(0, 0) => "   << (DataStorage::init(0, 0)  ? "fail" : "ok  ") << endl;
	cout << "init(0, 2) => "   << (DataStorage::init(0, 1)  ? "fail" : "ok  ") << endl;
	cout << "init(2, 0) => "   << (DataStorage::init(2, 0)  ? "fail" : "ok  ") << endl;
	cout << "init(2, 2) => "   << (DataStorage::init(2, 2)  ? "fail" : "ok  ") << endl;
	cout << endl;

	buff[0] = 0;
	cout << "get( 5) => "       << (DataStorage::get_block(5, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(10) => "      << (DataStorage::get_block(10, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(20) => "      << (DataStorage::get_block(20, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(30) => "      << (DataStorage::get_block(30, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(40) => "      << (DataStorage::get_block(40, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(50) => "      << (DataStorage::get_block(50, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(60) => "      << (DataStorage::get_block(60, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(70) => "      << (DataStorage::get_block(70, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(80) => "      << (DataStorage::get_block(80, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	cout << endl;

	cout << "put(10, 'aa') => " << (DataStorage::put_block(10, "aa")  ? "fail" : "ok  ") << endl;
	cout << "put(20, 'bb') => " << (DataStorage::put_block(20, "bb")  ? "fail" : "ok  ") << endl;//
	cout << "put(30, 'cc') => " << (DataStorage::put_block(30, "cc")  ? "fail" : "ok  ") << endl;
	cout << "put(40, 'dd') => " << (DataStorage::put_block(40, "dd")  ? "fail" : "ok  ") << endl;//
	cout << "put(50, 'ee') => " << (DataStorage::put_block(50, "ee")  ? "fail" : "ok  ") << endl;
	cout << "put(60, 'aa') => " << (DataStorage::put_block(60, "aa")  ? "fail" : "ok  ") << endl;
	cout << "put(70, 'cc') => " << (DataStorage::put_block(70, "cc")  ? "fail" : "ok  ") << endl;
	cout << endl;

	buff[0] = 0;
	cout << "get( 5) => "       << (DataStorage::get_block(5, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(10) => "      << (DataStorage::get_block(10, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(20) => "      << (DataStorage::get_block(20, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(30) => "      << (DataStorage::get_block(30, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(40) => "      << (DataStorage::get_block(40, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(50) => "      << (DataStorage::get_block(50, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(60) => "      << (DataStorage::get_block(60, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(70) => "      << (DataStorage::get_block(70, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	buff[0] = 0;
	cout << "get(80) => "      << (DataStorage::get_block(80, buff) ? "fail" : "ok  ") << " => " << buff << endl;
	cout << endl;

	return 0;
}
