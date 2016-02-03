/*
 * DataStorage.cpp
 *
 *  Created on: 02 февр. 2016 г.
 *      Author: Dmitriy Khaustov aka Dиmon (khaustov.dm@gmail.com)
 */

#include "DataStorage.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdio>
#include <utility>
#include <iostream>

#include "MD5.hpp"

bool DataStorage::_initialized = false;
uint64_t DataStorage::_blocks_count = 0;
uint64_t DataStorage::_block_size = 0;
uint32_t DataStorage::_blob_size = 0;
std::map<uint64_t, uint64_t> DataStorage::_id2index;
std::multimap<DataStorage::Hash, uint64_t, DataStorage::HashCmpr> DataStorage::_hash2index;

//	Метод осуществляет установку соответствующих параметров.
//	block_size — размер передаваемого блока данных в байтах
//	blob_size — размер хранимого blob в количестве блоков
//	Возвращаемые значения: 0 в случае успеха, число отличное от 0 в случае неудачи.
int DataStorage::init(uint64_t block_size, uint32_t blob_size)
{
	// Уже проинициализировано
	if (_initialized)
	{
		return -1;
	}

	// Нулевые данные некорректны
	if (!block_size || !blob_size)
	{
		return -1;
	}

	// TODO Надо бы делать проверку, указанных размеров с теми данными, которые уже были записаны ранее, иначе данные будут поврежденные

	_blocks_count = 0;
	_block_size = block_size;
	_blob_size = blob_size;

	// Открываем (или создаем, если нет) индексный файл
	FILE *f = fopen("./index.bin", "r");
	if (!f)
	{
		f = fopen("./index.bin", "w+");
		if (!f)
		{
			return -1;
		}
	}

	// Вычитываем индекс
	for (;;)
	{
		// id блока
		uint64_t block_id;
		// порядковый номер блока
		uint64_t index;
		// хеш данных
		Hash hash;

		if (
			fread(&block_id, sizeof(block_id), 1, f) != 1 ||
			fread(&index, sizeof(index), 1, f) != 1 ||
			fread(hash.data, sizeof(hash.data), 1, f) != 1
		)
		{
			if (feof(f))
			{
				break;
			}
			return -1;
		}

		// Счетчик уникальных по содержимому блоков на едницу больше максимального порядкового номера блока (потому что счет с нуля)
		if (_blocks_count <= index)
		{
			_blocks_count = index + 1;
		}

		// Сохраняем информацию о блоке в памяти для удобного дальнейшего поиска
		_hash2index.insert(std::make_pair(hash, block_id));
		_id2index.insert(std::make_pair(block_id, index));
	}

	fclose(f);

	// Инициализация успешна
	_initialized = true;
	return 0;
}

//	block_id — уникальный идентификатор блока
//	block_data —набор байт размером block_size, данные блока
//	Метод помещает блок на хранение.
//	Возвращаемые значения: 0 в случае успеха, число отличное от 0 в случае неудачи.
int DataStorage::put_block(uint64_t block_id, const char* block_data)
{
	// Еще не проинициализирован
	if (!_initialized)
	{
		return -1;
	}

	// Блок с таким id уже в хранилище, поэтому молча пропускаем без перезаписи
	if (_id2index.find(block_id) != _id2index.end())
	{
		return 0;
	}

	Hash hash;

	// Вычисляем хеш добавляемых данных
	MD5 md5(block_data, _block_size);
	memcpy(hash.data, md5.getBytesHash(), sizeof(hash.data));

	// Перебираем порядковые номера всех блоков, имеющих такой же хеш
	auto r = _hash2index.equal_range(hash);
	for (auto hi = r.first; hi != r.second; hi++)
	{
		auto pi = _id2index.find(hi->second);
		if (pi == _id2index.end())
		{
			return -1;
		}

		// Порядковый номер очередного блока
		auto index = pi->second;

		char path[1<<12];
		sprintf(path, "./blob-%lX.bin", index / _blob_size);

		int fd = open(path, 0);
		// Не удалось открыть файл блоба
		if (fd < 0)
		{
			return -1;
		}

		// Обображаем блоб в память. Отображение выбрано для того, чтоб не расходовать память в случае больших размеров
		char *blob = static_cast<char *>(mmap(nullptr, _block_size * _blob_size, PROT_READ, MAP_SHARED, fd, 0));

		close(fd);

		// Отображение не удалось
		if (blob == reinterpret_cast<void *>(-1))
		{
			return -1;
		}

		// Побайтовое сравнение нового и имеющегося блока, чтобы обойти возможную коллизию хешей
		bool is_duplicate = memcmp(blob + _block_size * (index % _blob_size), block_data, _block_size) == 0;

		// Снимаем отображение
		munmap(blob, _block_size);

		// Это действительно дубликат
		if (is_duplicate)
		{
			FILE *f = fopen("./index.bin", "a+");
			// Не удалось открыть индексный файл
			if (!f)
			{
				return -1;
			}
			// Делаем запись в индексный файл
			fwrite(&block_id, sizeof(block_id), 1, f);
			fwrite(&index, sizeof(index), 1, f);
			fwrite(hash.data, sizeof(hash.data), 1, f);
			fclose(f);

			// Регистируем новый блок в памяти для удобного дальнейшего поиска
			_hash2index.insert(std::make_pair(hash, block_id));
			_id2index.insert(std::make_pair(block_id, index));
			return 0;
		}
	}

	// Далее код, который выполнится, если дубликатов не найдено

	// Индекс
	auto index = _blocks_count;

	// Путь к блобу
	char path[1<<12];
	sprintf(path, "./blob-%lX.bin", index / _blob_size);

	FILE *f = fopen(path, "a+");
	// Не удалось открыть файл блоба
	if (!f)
	{
		return -1;
	}

	// Записываем очередной блок
	if (fwrite(block_data, _block_size, 1, f) != 1)
	{
		// Запись неудалась
		fclose(f);
		return -1;
	}

	fclose(f);

	f = fopen("./index.bin", "a+");

	// Не удалось открыть индексный файл
	if (!f)
	{
		// Усекаем файл блоба, чтобы отбросить только что добавленные данные,
		// для которых не удается сделать запись в индекс
		truncate(path, _block_size * (index % _blob_size));
		return -1;
	}

	// Делаем запись в индексный файл
	fwrite(&block_id, sizeof(block_id), 1, f);
	fwrite(&index, sizeof(index), 1, f);
	fwrite(hash.data, sizeof(hash.data), 1, f);
	fclose(f);

	// Регистируем новый блок в памяти для удобного дальнейшего поиска
	_hash2index.insert(std::make_pair(hash, block_id));
	_id2index.insert(std::make_pair(block_id, index));

	// Увеличиваем счетчик блоков
	_blocks_count++;

	return 0;
}

//	Метод заполняет block_data данными блока block_id
//	block_id — уникальный идентификатор блока
//	block_data — указатель на свободное место в памяти размером block_size
int DataStorage::get_block(uint64_t block_id, char* block_data)
{
	// Еще не проинициализирован
	if (!_initialized)
	{
		return -1;
	}

	auto i = _id2index.find(block_id);

	// Блок с таким id не зарегистрирован
	if (i == _id2index.end())
	{
		return -1;
	}

	// Порядковый номер блока
	auto index = i->second;

	// Получаем пусть к блобу, который соответствует найденному порядковому номеру
	char path[1<<12];
	sprintf(path, "./blob-%lX.bin", index / _blob_size);

	int fd = open(path, 0);
	// Не удалось открыть файл блоба
	if (fd < 0)
	{
		return -1;
	}

	// Отображаем блоб в память
	char *blob = static_cast<char *>(mmap(nullptr, _block_size * _blob_size, PROT_READ, MAP_SHARED, fd, 0));

	// Отобажение в память не удалось
	if (blob == reinterpret_cast<void *>(-1))
	{
		close(fd);
		return -1;
	}

	close(fd);

	// Копируем данные
	memcpy(block_data, blob + _block_size * (index % _blob_size), _block_size);

	// Снимаем отображение
	munmap(blob, _block_size);

	return 0;
}
