#pragma once
const std::string sunflowerOperations[2] =
{
		u8"Культивация (ГА)",
		u8"Внесение СЗР 1-я противозлак. обр. (ГА)"
};

std::vector<object_db> fillInputAndAlternativeOperationsForSunflower(std::vector<object_db> dataSunflower)
{
	std::vector<object_db> resultDataSunflower;

	for (int row = 0; row < dataSunflower.size(); row++)
	{
		if (dataSunflower[row].material_order == sunflowerOperations[0])
		{
			dataSunflower[row].input_operation = u8"Посев без внесения удобрений (ГА)";
			dataSunflower[row].alt_operation = u8"Посев с внесением удобрений (ГА)";
			dataSunflower[row].input_deadline = 25;
			dataSunflower[row].alt_deadline = 25;
			resultDataSunflower.push_back(dataSunflower[row]);
		}
		if (dataSunflower[row].material_order == sunflowerOperations[1])
		{
			dataSunflower[row].input_operation = u8"Культивация (ГА)";
			dataSunflower[row].input_deadline = 7;
			resultDataSunflower.push_back(dataSunflower[row]);
		}
	}
	return resultDataSunflower;
}

// Основная функция для упорядочивания операций по подсолнечнику
std::vector<object_db> orderSunflowerOperations(const std::vector<object_db>& currentDataSunflower)
{
	std::string firstOrder = sunflowerOperations[0];
	std::string secondOrder = sunflowerOperations[1];

	std::vector<object_db> result;

	// Фильтрация и добавление элементов в результирующий вектор
	std::vector<object_db> firstSlice = filter(currentDataSunflower, firstOrder);
	std::vector<object_db> secondSlice = filter(currentDataSunflower, secondOrder);

	result.insert(result.end(), firstSlice.begin(), firstSlice.end());
	result.insert(result.end(), secondSlice.begin(), secondSlice.end());


	return result;
}

//определение статусов по подсолнечнику
std::string defineSunflowerElementStatus(object_db& elem, std::vector<object_db>& uniqSElements, std::vector<object_db>& SunflowerUniqueSownOperations)
{
	// Нахождение посеянной операции (sownOp)
	auto sownOpIt = std::find_if(SunflowerUniqueSownOperations.begin(), SunflowerUniqueSownOperations.end(),
		[&elem](const object_db& el)
		{
			return el.higher_tm == elem.higher_tm && el.pu == elem.pu;
		});

	object_db sownOp = (sownOpIt != SunflowerUniqueSownOperations.end()) ? *sownOpIt : object_db();
	std::optional<object_db> prevOp;
	std::optional<object_db> altOp;

	// Определение prevOp и altOp в зависимости от material_order
	if (elem.material_order == sunflowerOperations[0])
	{
		prevOp = sownOp;
	}
	else if (elem.material_order == sunflowerOperations[1])
	{
		auto prevOpIt = std::find_if(uniqSElements.begin(), uniqSElements.end(),
			[&elem](const object_db& el)
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqSElements.end()) ? *prevOpIt : std::optional<object_db>{};
		altOp = sownOp;
	}
	else
	{
		auto prevOpIt = std::find_if(uniqSElements.begin(), uniqSElements.end(),
			[&elem](const object_db& el)
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqSElements.end()) ? *prevOpIt : std::optional<object_db>{};
	}

	// Определение статуса элемента
	if (!sownOp.is_completed)
	{
		return "not_sown";
	}
	else if (
		((prevOp.has_value() && elem.actual_data <= add_days(prevOp->actual_data, elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data <= add_days(altOp->actual_data, elem.alt_deadline.value_or(0)))) &&
		!elem.is_completed
		)
	{
		return "not_started";
	}
	else if (
		((prevOp.has_value() && elem.actual_data <= add_days(prevOp->actual_data, elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data <= add_days(altOp->actual_data, elem.alt_deadline.value_or(0)))) &&
		elem.is_completed
		)
	{
		return "in_time";
	}
	else if (
		((prevOp.has_value() && elem.actual_data <= add_days(prevOp->actual_data, 2 + elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data <= add_days(altOp->actual_data, 2 + elem.alt_deadline.value_or(0)))) &&
		elem.is_completed
		)
	{
		return "deviation_less_two_days";
	}
	else if (
		((prevOp.has_value() && elem.actual_data > add_days(prevOp->actual_data, 2 + elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data > add_days(altOp->actual_data, 2 + elem.alt_deadline.value_or(0)))) &&
		elem.is_completed
		)
	{
		return "deviation_more_two_days";
	}
	else if (
		((prevOp.has_value() && elem.actual_data > add_days(prevOp->actual_data, elem.input_deadline.value_or(0))) ||
			(altOp.has_value() && elem.actual_data > add_days(altOp->actual_data, elem.alt_deadline.value_or(0)))) &&
		!elem.is_completed
		)
	{
		return "expired_not_completed";
	}
	else
	{
		return "in_time";
	}
}

// Функция обновления lastResult
void updateLastResultSunflower(LastResult& lastResult, object_db& el, std::string& currentStatus)
{
	for (int i = 0; i < sunflowerOperations->size(); i++)
	{
		if (el.material_order.has_value() && el.material_order.value() == sunflowerOperations[i])
		{
			for (int j = 0; j < lastResult.sunflower.operations[sunflowerOperations[i]].size(); j++)
			{
				if (lastResult.sunflower.operations[sunflowerOperations[i]][j].businessDirection == el.business_dir && lastResult.sunflower.operations[sunflowerOperations[i]][j].PU == el.pu)
				{
					lastResult.sunflower.operations[sunflowerOperations[i]][j].statuses[currentStatus].fieldsCount++;
					lastResult.sunflower.operations[sunflowerOperations[i]][j].statuses[currentStatus].fieldsSquare += el.field_square.value_or(0.0f);
					break;  // Выход из цикла, так как нужный элемент найден
				}
			}
		}
	}
}