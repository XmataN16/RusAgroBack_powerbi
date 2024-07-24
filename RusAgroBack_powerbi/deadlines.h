#pragma once

//Заполнение словаря словарей с культурами, операциями и сроками 
void set_deadlines(std::map<std::string, std::map<std::string, int>>& deadlines)
{
	//По сахарной свекле
	deadlines["Сахарная свекла"]["Внесение почвенного гербицида"] = 3;
	deadlines["Сахарная свекла"]["Первая бетанальная обработка"] = 18;
	deadlines["Сахарная свекла"]["Вторая бетанальная обработка"] = 10;
	deadlines["Сахарная свекла"]["Третья бетанальная обработка"] = 10;
	deadlines["Сахарная свекла"]["Четвертая бетанальная обработка"] = 10;
	//По сое
	deadlines["Соя"]["Внесение почвенного гербицида"] = 3;
	deadlines["Соя"]["Первая обработка гербицидами класса «Бензотиадиазоны»"] = 20;
	deadlines["Соя"]["Вторая обработка гербицидами класса «Бензотиадиазоны»"] = 12;
	//По кукурузе
	deadlines["Кукуруза"]["Внесение почвенного гербицида"] = 3;
	deadlines["Кукуруза"]["Обработка гербицидами класса «сульфонилмочевины+ трикетоны»"] = 30;
	//По подсолнечнику
	deadlines["Подсолнечник"]["Обработка гербицидами класса «сульфонилмочевины»"] = 25;
	deadlines["Подсолнечник"]["Обработка гербицидами класса «граминицидов»"] = 7;
}

void calc(data data_shbn[CULTURES_COUNT][REGIONS_COUNT])
{
	// Проход по всем культурам
	for (int culture = 0; culture < CULTURES_COUNT; culture++) 
	{
		// Создание массива для текущей культуры
		std::vector<int> cultureResult;
		// Все операции по культуре
		std::unordered_map<std::string, std::string> cultureOperations;

		// Заполнения объекта операциями по текущей культуре
		for (int region = 0; region < REGIONS_COUNT; region++)
		{
			for (int row = 0; row < data_shbn[culture][region].row_count; row++)
				if (data_shbn[culture][region].culture[row] == CULTURES_RUS[culture])
			{
				if (cultureOperations.find(data_shbn[culture][region].material_order[row].value() == cultureOperations.end()))
				{
					cultureOperations[item.operation] = item.operation;
				}
			}
		}

		for (const auto& operation : cultureOperations) {
		}

		results.push_back(cultureResult);
	}
}