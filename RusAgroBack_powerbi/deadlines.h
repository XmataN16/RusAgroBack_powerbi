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

struct Status 
{
    int fieldsCount;
    int fieldsSquare;
};

struct Operation 
{
    std::string businessDirection;
    std::string PU;
    std::unordered_map<std::string, Status> statuses;
};

struct Crop 
{
    std::unordered_map<std::string, std::vector<Operation>> operations;
};

struct Result 
{
	std::unordered_map<std::string, std::vector<std::string>> sugar_beet;
};

struct LastResult 
{
    Crop sugar_beet;
    Crop corn;
    Crop soy;
    Crop sunflower;
    Crop corn_silage;
};

/*
LastResult lastResult = {
	.sugar_beet = {
		.operations = {
			"Уборка корнеплодов(ГА)" = {}
        }
    },
};
*/

// Функция для получения уникальных значений material_order
std::vector<std::string> get_unique_material_orders(const std::vector<object_db>& data_in_culture) 
{
	std::set<std::string> unique_set;
	for (const auto& obj : data_in_culture)
	{
		if (obj.material_order.has_value()) 
		{
			unique_set.insert(obj.material_order.value());
		}
	}
	return std::vector<std::string>(unique_set.begin(), unique_set.end());
}

void calc(data data_shbn[CULTURES_COUNT])
{
	Result lastResult;
	std::vector<object_db> data_sugar_beet = data_shbn[0].objects;
	std::vector<std::string> uniq_operations_sugar_beet = get_unique_material_orders(data_sugar_beet);

	for (size_t i = 0; i < uniq_operations_sugar_beet.size(); i++)
	{
		lastResult.sugar_beet[uniq_operations_sugar_beet[i]].push_back({ uniq_operations_sugar_beet[i]});
		std::cout << lastResult.sugar_beet[uniq_operations_sugar_beet[i]][i] << std::endl;
	}
	
    //std::cout << data_sugar_beet[0].material_order.value() << std::endl;
	
	
}

// result
/*{sugar_beet:
  {not_started:
	[
	  {ID: 54028
		Culture: Сахарная свекла
		NZP_ZP: ZP
		Business Direction: Белгород Юг
		Calendar Day: 2024-08-25
		Higher TM: BL-04-04-83-0050
		Material Order: Уборка корнеплодов (ГА)
		Planned Volume: 49
		Actual Volume: 0
		PU: РАИ ПУ Закутчанский
	  }
	]
  }
}
*/