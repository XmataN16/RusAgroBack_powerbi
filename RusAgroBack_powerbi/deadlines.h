void print_data(std::vector<object_db> data)
{
	for (int i = 0; i < data.size(); i++)
	{
		std::cout << "ID:" << data[i].id << std::endl;
		if (data[i].culture.has_value())
		{
			std::cout << "culture:" << data[i].culture.value() << std::endl;
		}
		else
		{
			std::cout << "culture: NULL" << std::endl;
		}
		if (data[i].nzp_zp.has_value())
		{
			std::cout << "nzp_zp: " << data[i].nzp_zp.value() << std::endl;
		}
		else
		{
			std::cout << "nzp_zp: NULL" << std::endl;
		}
		if (data[i].business_dir.has_value())
		{
			std::cout << "business_dir: " << data[i].business_dir.value() << std::endl;
		}
		else
		{
			std::cout << "business_dir: NULL" << std::endl;
		}
		if (data[i].calendar_day.has_value())
		{
			std::cout << "calendar_day: " << tm_to_str(data[i].calendar_day).value() << std::endl;
		}
		else
		{
			std::cout << "calendar_day: NULL" << std::endl;
		}
		if (data[i].higher_tm.has_value())
		{
			std::cout << "higher_tm: " << data[i].higher_tm.value() << std::endl;
		}
		else
		{
			std::cout << "higher_tm: NULL" << std::endl;
		}
		if (data[i].material_order.has_value())
		{
			std::cout << "material_order: " << data[i].material_order.value() << std::endl;
		}
		else
		{
			std::cout << "material_order: NULL" << std::endl;
		}
		if (data[i].planned_volume.has_value())
		{
			std::cout << "planned_volume: " << data[i].planned_volume.value() << std::endl;
		}
		else
		{
			std::cout << "planned_volume: NULL" << std::endl;
		}
		if (data[i].actual_volume.has_value())
		{
			std::cout << "actual_volume: " << data[i].actual_volume.value() << std::endl;
		}
		else
		{
			std::cout << "actual_volume: NULL" << std::endl;
		}
		if (data[i].pu.has_value())
		{
			std::cout << "pu: " << data[i].pu.value() << std::endl;
		}
		else
		{
			std::cout << "pu: NULL" << std::endl;
		}
		if (data[i].input_operation.has_value())
		{
			std::cout << "input_operation: " << data[i].input_operation.value() << std::endl;
		}
		else
		{
			std::cout << "input_operation: NULL" << std::endl;
		}
		if (data[i].alt_operation.has_value())
		{
			std::cout << "alt_operation: " << data[i].alt_operation.value() << std::endl;
		}
		else
		{
			std::cout << "alt_operation: NULL" << std::endl;
		}
		if (data[i].input_deadline.has_value())
		{
			std::cout << "input_deadline: " << data[i].input_deadline.value() << std::endl;
		}
		else
		{
			std::cout << "input_deadline: NULL" << std::endl;
		}
		if (data[i].alt_deadline.has_value())
		{
			std::cout << "alt_deadline: " << data[i].alt_deadline.value() << std::endl;
		}
		else
		{
			std::cout << "alt_deadline: NULL" << std::endl;
		}
		std::cout << std::endl;
	}
}

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

// Функция для получения уникальных значений pu и business_dir
std::vector<std::pair<std::string, std::string>> getUniquePU(const std::vector<object_db>& dataInCulture)
{
	std::set<std::pair<std::string, std::string>> unique_set;
	for (const auto& obj : dataInCulture)
	{
		if (obj.pu.has_value() && obj.business_dir.has_value())
		{
			unique_set.emplace(obj.pu.value(), obj.business_dir.value());
		}
	}
	return std::vector<std::pair<std::string, std::string>>(unique_set.begin(), unique_set.end());
}



// Функция для фильтрации элементов по значению material_order
std::vector<object_db> filter(const std::vector<object_db>& input, std::string& order)
{
	std::vector<object_db> resultSlice;

	for (int i = 0; i < input.size(); i++)
	{
		if (input[i].material_order.value() == order)
		{
			resultSlice.push_back(input[i]);
		}
	}
	return resultSlice;
}

// Функция для фильтрации элементов по двум значения material_order
std::vector<object_db> filter2(const std::vector<object_db>& input, const std::string& order1, const std::string& order2)
{
	std::vector<object_db> resultSlice;

	for (int i = 0; i < input.size(); i++)
	{
		if (input[i].material_order.value() == order1 || input[i].material_order.value() == order2)
		{
			resultSlice.push_back(input[i]);
		}
	}
	return resultSlice;
}

// Функция для получения уникальных объектов
std::vector<object_db> getUniqueObjects(const std::vector<object_db>& orderedElements) 
{
	std::vector<object_db> uniqueObjects;
	std::unordered_set<std::string> uniqueKeys;

	for (const auto& element : orderedElements) 
	{
		std::string key = (element.higher_tm.value_or("")) + "_" +
			(element.material_order.value_or("")) + "_" +
			(element.culture.value_or("")) + "_" +
			(element.business_dir.value_or("")) + "_" +
			(element.nzp_zp.value_or(""));

		if (uniqueKeys.find(key) == uniqueKeys.end()) 
		{
			uniqueKeys.insert(key);
			uniqueObjects.push_back(element);
		}
	}

	return uniqueObjects;
}

void calcFactDates(std::vector<object_db>& uniqElements, std::vector<object_db>& AllElements)
{
	for (int i = 0; i < uniqElements.size(); i++)
	{
		float sumPlannedVolume = 0;
		float sumActualVolume = 0;
		std::tm max_date = create_date(1999, 1, 1);

		for (int j = 0; j < AllElements.size(); j++)
		{
			if ((uniqElements[i].higher_tm.value() == AllElements[j].higher_tm.value())
				and (uniqElements[i].material_order.value() == AllElements[j].material_order.value())
				and (uniqElements[i].culture.value() == AllElements[j].culture.value()))
			{
				sumPlannedVolume += AllElements[j].planned_volume.value();
				sumActualVolume += AllElements[j].actual_volume.value();
				std::tm maxDate = maxDate > AllElements[j].calendar_day.value() ? maxDate : AllElements[j].calendar_day.value();
			}
		}
		uniqElements[i].is_completed = sumPlannedVolume * 0.8 <= sumActualVolume && sumActualVolume != 0;
		uniqElements[i].actual_data = max_date;
		uniqElements[i].field_square = sumActualVolume;
	}
}

#include "sugar_beet_functions.h"
#include "soy_functions.h"
#include "sunflower_functions.h"
#include "corn_functions.h"
#include "corn_silage_functions.h"


void calc_sugar_beet(data data_shbn[CULTURES_COUNT], LastResult& lastResult)
{
	// Загрузка общих данных из БД
	std::vector<object_db> dataSugarBeet = data_shbn[0].objects;

	// Уникальные ПУ и Регионы из данных БД
	std::vector<std::pair<std::string, std::string>> uniqPUAndBusinessDir = getUniquePU(dataSugarBeet);

	// Вызов функций для расчета сроков по Сахарной свекле
	std::vector<object_db> currentSugarBeetElements = fillInputAndAlternativeOperationsForSugarBeet(dataSugarBeet);
	std::vector<object_db> orderedSugarBeetElements = orderSugarBeetOperations(currentSugarBeetElements);
	std::vector<object_db> uniqSBElements = getUniqueObjects(orderedSugarBeetElements);

	// Заполнение lastResult операциями
	for (const auto& op : sugarBeetOperations) 
	{
		lastResult.sugar_beet.operations[op] = std::vector<Operation>();

		// Заполнение lastResult бизнес-направлениями и PU
		for (const auto& reg : uniqPUAndBusinessDir) 
		{
			Operation operation(reg.second, reg.first);
			lastResult.sugar_beet.operations[op].push_back(operation);
		}
	}

	// Логика по расчету даты выполнения операций
	calcFactDates(uniqSBElements, orderedSugarBeetElements);

	std::vector<object_db> sugarBeetSownOperations = filter2(dataSugarBeet, u8"Посев без внесения удобрений (ГА)", u8"Посев с внесением удобрений (ГА)");
	// Получение уникальных элементов с рассчитанными фактическими датами
    // Нужно для определения первого статуса
	std::vector<object_db> sugarBeetUniqueSownOperations = getUniqueObjects(sugarBeetSownOperations);
	calcFactDates(sugarBeetUniqueSownOperations, sugarBeetSownOperations);

	// Логика по определению места записи для объекта
	for (int i = 0; i < uniqSBElements.size(); i++)
	{
		std::string currentStatus = defineSugarBeetElementStatus(uniqSBElements[i], uniqSBElements, sugarBeetUniqueSownOperations);
		updateLastResultSugarBeet(lastResult, uniqSBElements[i], currentStatus);

	}

	//printLastResult(lastResult);
}

void calc_soy(data data_shbn[CULTURES_COUNT], LastResult& lastResult)
{
	// Загрузка общих данных из БД
	std::vector<object_db> dataSoy = data_shbn[1].objects;

	// Уникальные ПУ и Регионы из данных БД
	std::vector<std::pair<std::string, std::string>> uniqPUAndBusinessDir = getUniquePU(dataSoy);

	// Вызов функций для расчета сроков по Сое
	std::vector<object_db> currentSoyElements = fillInputAndAlternativeOperationsForSoy(dataSoy);
	std::vector<object_db> orderedSoyElements = orderSoyOperations(currentSoyElements);
	std::vector<object_db> uniqSElements = getUniqueObjects(orderedSoyElements);

	// Заполнение lastResult операциями
	for (const auto& op : soyOperations)
	{
		lastResult.soy.operations[op] = std::vector<Operation>();

		// Заполнение lastResult бизнес-направлениями и PU
		for (const auto& reg : uniqPUAndBusinessDir)
		{
			Operation operation(reg.second, reg.first);
			lastResult.soy.operations[op].push_back(operation);
		}
	}

	// Логика по расчету даты выполнения операций
	calcFactDates(uniqSElements, orderedSoyElements);

	std::vector<object_db> SoySownOperations = filter2(dataSoy, u8"Посев без внесения удобрений (ГА)", u8"Посев с внесением удобрений (ГА)");
	// Получение уникальных элементов с рассчитанными фактическими датами
	// Нужно для определения первого статуса
	std::vector<object_db> SoyUniqueSownOperations = getUniqueObjects(SoySownOperations);
	calcFactDates(SoyUniqueSownOperations, SoySownOperations);

	// Логика по определению места записи для объекта
	for (int i = 0; i < uniqSElements.size(); i++)
	{
		std::string currentStatus = defineSoyElementStatus(uniqSElements[i], uniqSElements, SoyUniqueSownOperations);
		updateLastResultSoy(lastResult, uniqSElements[i], currentStatus);
	}

	//printLastResult(lastResult);
}

void calc_sunflower(data data_shbn[CULTURES_COUNT], LastResult& lastResult)
{
	// Загрузка общих данных из БД
	std::vector<object_db> dataSunflower = data_shbn[2].objects;

	// Уникальные ПУ и Регионы из данных БД
	std::vector<std::pair<std::string, std::string>> uniqPUAndBusinessDir = getUniquePU(dataSunflower);

	// Вызов функций для расчета сроков по Подсолненичнику
	std::vector<object_db> currentSunflowerElements = fillInputAndAlternativeOperationsForSunflower(dataSunflower);
	std::vector<object_db> orderedSunflowerElements = orderSunflowerOperations(currentSunflowerElements);
	std::vector<object_db> uniqSFElements = getUniqueObjects(orderedSunflowerElements);

	// Заполнение lastResult операциями
	for (const auto& op : sunflowerOperations)
	{
		lastResult.sunflower.operations[op] = std::vector<Operation>();

		// Заполнение lastResult бизнес-направлениями и PU
		for (const auto& reg : uniqPUAndBusinessDir)
		{
			Operation operation(reg.second, reg.first);
			lastResult.sunflower.operations[op].push_back(operation);
		}
	}

	// Логика по расчету даты выполнения операций
	calcFactDates(uniqSFElements, orderedSunflowerElements);

	std::vector<object_db> SunflowerSownOperations = filter2(dataSunflower, u8"Посев без внесения удобрений (ГА)", u8"Посев с внесением удобрений (ГА)");
	// Получение уникальных элементов с рассчитанными фактическими датами
	// Нужно для определения первого статуса
	std::vector<object_db> SunflowerUniqueSownOperations = getUniqueObjects(SunflowerSownOperations);
	calcFactDates(SunflowerUniqueSownOperations, SunflowerSownOperations);

	// Логика по определению места записи для объекта
	for (int i = 0; i < uniqSFElements.size(); i++)
	{
		std::string currentStatus = defineSunflowerElementStatus(uniqSFElements[i], uniqSFElements, SunflowerUniqueSownOperations);
		updateLastResultSunflower(lastResult, uniqSFElements[i], currentStatus);
	}

	//printLastResult(lastResult);
}

void calc_corn(data data_shbn[CULTURES_COUNT], LastResult& lastResult)
{
	// Загрузка общих данных из БД
	std::vector<object_db> dataCorn = data_shbn[3].objects;

	// Уникальные ПУ и Регионы из данных БД
	std::vector<std::pair<std::string, std::string>> uniqPUAndBusinessDir = getUniquePU(dataCorn);

	// Вызов функций для расчета сроков по Подсолненичнику
	std::vector<object_db> currentCornElements = fillInputAndAlternativeOperationsForCorn(dataCorn);
	std::vector<object_db> orderedCornElements = orderCornOperations(currentCornElements);
	std::vector<object_db> uniqCElements = getUniqueObjects(orderedCornElements);

	// Заполнение lastResult операциями
	for (const auto& op : cornOperations)
	{
		lastResult.corn.operations[op] = std::vector<Operation>();

		// Заполнение lastResult бизнес-направлениями и PU
		for (const auto& reg : uniqPUAndBusinessDir)
		{
			Operation operation(reg.second, reg.first);
			lastResult.corn.operations[op].push_back(operation);
		}
	}

	// Логика по расчету даты выполнения операций
	calcFactDates(uniqCElements, orderedCornElements);

	std::vector<object_db> CornSownOperations = filter2(dataCorn, u8"Посев без внесения удобрений (ГА)", u8"Посев с внесением удобрений (ГА)");
	// Получение уникальных элементов с рассчитанными фактическими датами
	// Нужно для определения первого статуса
	std::vector<object_db> CornUniqueSownOperations = getUniqueObjects(CornSownOperations);
	calcFactDates(CornUniqueSownOperations, CornSownOperations);

	// Логика по определению места записи для объекта
	for (int i = 0; i < uniqCElements.size(); i++)
	{
		std::string currentStatus = defineCornElementStatus(uniqCElements[i], uniqCElements, CornUniqueSownOperations);
		updateLastResultCorn(lastResult, uniqCElements[i], currentStatus);
	}

	//printLastResult(lastResult);
}

void calc_corn_silage(data data_shbn[CULTURES_COUNT], LastResult& lastResult)
{
	// Загрузка общих данных из БД
	std::vector<object_db> dataCornSilage = data_shbn[4].objects;

	// Уникальные ПУ и Регионы из данных БД
	std::vector<std::pair<std::string, std::string>> uniqPUAndBusinessDir = getUniquePU(dataCornSilage);

	// Вызов функций для расчета сроков по Подсолненичнику
	std::vector<object_db> currentCornSilageElements = fillInputAndAlternativeOperationsForCornSilage(dataCornSilage);
	std::vector<object_db> orderedCornSilageElements = orderCornOperations(currentCornSilageElements);
	std::vector<object_db> uniqCSElements = getUniqueObjects(orderedCornSilageElements);

	// Заполнение lastResult операциями
	for (const auto& op : cornSilageOperations)
	{
		lastResult.corn_silage.operations[op] = std::vector<Operation>();

		// Заполнение lastResult бизнес-направлениями и PU
		for (const auto& reg : uniqPUAndBusinessDir)
		{
			Operation operation(reg.second, reg.first);
			lastResult.corn_silage.operations[op].push_back(operation);
		}
	}

	// Логика по расчету даты выполнения операций
	calcFactDates(uniqCSElements, orderedCornSilageElements);

	std::vector<object_db> CornSilageSownOperations = filter2(dataCornSilage, u8"Посев без внесения удобрений (ГА)", u8"Посев с внесением удобрений (ГА)");
	// Получение уникальных элементов с рассчитанными фактическими датами
	// Нужно для определения первого статуса
	std::vector<object_db> CornSilageUniqueSownOperations = getUniqueObjects(CornSilageSownOperations);
	calcFactDates(CornSilageUniqueSownOperations, CornSilageSownOperations);

	// Логика по определению места записи для объекта
	for (int i = 0; i < uniqCSElements.size(); i++)
	{
		std::string currentStatus = defineCornSilageElementStatus(uniqCSElements[i], uniqCSElements, CornSilageUniqueSownOperations);
		updateLastResultCornSilage(lastResult, uniqCSElements[i], currentStatus);
	}

	printLastResult(lastResult);
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