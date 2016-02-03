/*
 * DataStorage.hpp
 *
 *  Created on: 02 февр. 2016 г.
 *      Author: Dmitriy Khaustov aka Dиmon (khaustov.dm@gmail.com)
 */

#ifndef DATASTORAGE_HPP_
#define DATASTORAGE_HPP_

#include <string.h>
#include <cstdint>
#include <map>

class DataStorage
{
private:
	// MD5-хеш блока, для быстрой первичной проверки дублей
	struct Hash {
		char data[16];
	};

	// Компаратор для строковых ключей
	struct HashCmpr
	{
		bool operator()(const Hash x, const Hash y) const
		{
			return memcmp(x.data, y.data, sizeof(x.data)) < 0;
		}
	};

	// Флаг инициализации
	static bool _initialized;

	// Счетчик добавленых блоков
	static uint64_t _blocks_count;

	/// Размер блокав байтах
	static uint64_t _block_size;
	/// Размер блоба в блоках
	static uint32_t _blob_size;

	/// Сопоставление id блока его порядковому номеру
	static std::map<uint64_t, uint64_t> _id2index;
	/// Сопоставление хеша блока набору id блоков с таким же хешем
	static std::multimap<Hash, uint64_t, HashCmpr> _hash2index;

public:
	//	Метод осуществляет установку соответствующих параметров.
	//	block_size — размер передаваемого блока данных в байтах
	//	blob_size — размер хранимого blob в количестве блоков
	//	Возвращаемые значения: 0 в случае успеха, число отличное от 0 в случае неудачи.
	static int init(uint64_t block_size, uint32_t blob_size);

	//	block_id — уникальный идентификатор блока
	//	block_data —набор байт размером block_size, данные блока
	//	Метод помещает блок на хранение.
	//	Возвращаемые значения: 0 в случае успеха, число отличное от 0 в случае неудачи.
	static int put_block(uint64_t block_id, const char *block_data);

	//	Метод заполняет block_data данными блока block_id
	//	block_id — уникальный идентификатор блока
	//	block_data — указатель на свободное место в памяти размером block_size
	static int get_block(uint64_t block_id, char *block_data);
};

#endif /* DATASTORAGE_HPP_ */
