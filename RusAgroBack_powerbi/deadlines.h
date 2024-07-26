#pragma once

class ResultArray
{
public:


	ResultArray()
	{

	}


private:

};



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

struct Result 
{
	std::unordered_map<std::string, std::vector<std::string>> sugar_beet;
	std::unordered_map<std::string, std::vector<std::string>> corn;
};

struct CultureResult 
{
	std::vector<std::string> non_feed;
	std::vector<std::string> not_started;
};

void calc(data data_shbn[CULTURES_COUNT][REGIONS_COUNT])
{
	Result result;
	CultureResult sugar_beet_result;
	CultureResult corn;

	for (int culture = 0; culture < CULTURES_COUNT; culture++)
	{
		for (int region = 0; region < REGIONS_COUNT; region++)
		{
			for (int row = 0; row < data_shbn[culture][region].row_count; row++)
			{
				// Делить по ПУ не надо, Безоносов отфильтрует у себя...
				
			}
		}
	}
	for (const auto& el : data_shbn) 
	{
		if (el.culture == "sugar_beet") 
		{
			// Logic for calculating status
			// Assign status to el.status
			// End of status calculation logic
			if (el.status == "Не посеяно") sugar_beet_result.non_feed.push_back(cur_el);
			if (el.status == "Срок не наступил") sugar_beet_result.not_started.push_back(cur_el);
		}
	}

	result.sugar_beet = sugar_beet_result;

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