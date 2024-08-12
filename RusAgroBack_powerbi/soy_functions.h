#pragma once
const std::string soyOperations[3] =
{
		u8"Внесение СЗР Почвенные гербициды (ГА)",
		u8"Культивация (ГА)",
		u8"Внесение СЗР 2-я баковая смесь (ГА)"
};

std::vector<object_db> fillInputAndAlternativeOperationsForSoy(std::vector<object_db> dataSoy)
{
	std::vector<object_db> resultDataSoy;

	for (int row = 0; row < dataSoy.size(); row++)
	{
		if (dataSoy[row].material_order == soyOperations[0])
		{
			dataSoy[row].input_operation = u8"Посев без внесения удобрений (ГА)";
			dataSoy[row].alt_operation = u8"Посев с внесением удобрений (ГА)";
			dataSoy[row].input_deadline = 3;
			dataSoy[row].alt_deadline = 3;
			resultDataSoy.push_back(dataSoy[row]);
		}
		if (dataSoy[row].material_order == soyOperations[1])
		{
			dataSoy[row].input_operation = u8"Внесение СЗР Почвенные гербициды (ГА)";
			dataSoy[row].alt_operation = u8"Посев с внесением удобрений (ГА)";
			dataSoy[row].input_deadline = 20;
			dataSoy[row].alt_deadline = 15;
			resultDataSoy.push_back(dataSoy[row]);
		}
		if (dataSoy[row].material_order == soyOperations[2])
		{
			dataSoy[row].input_operation = u8"Культивация (ГА)";
			dataSoy[row].input_deadline = 12;
			resultDataSoy.push_back(dataSoy[row]);
		}
	}
	return resultDataSoy;
}

// Основная функция для упорядочивания операций по сое
std::vector<object_db> orderSoyOperations(const std::vector<object_db>& currentDataSoy)
{
	std::string firstOrder = soyOperations[0];
	std::string secondOrder = soyOperations[1];
	std::string thirdOrder = soyOperations[2];

	std::vector<object_db> result;

	// Фильтрация и добавление элементов в результирующий вектор
	std::vector<object_db> firstSlice = filter(currentDataSoy, firstOrder);
	std::vector<object_db> secondSlice = filter(currentDataSoy, secondOrder);
	std::vector<object_db> thirdSlice = filter(currentDataSoy, thirdOrder);

	result.insert(result.end(), firstSlice.begin(), firstSlice.end());
	result.insert(result.end(), secondSlice.begin(), secondSlice.end());
	result.insert(result.end(), thirdSlice.begin(), thirdSlice.end());

	return result;
}

//определение статусов в сое
std::string defineSoyElementStatus(object_db& elem, std::vector<object_db>& uniqSElements, std::vector<object_db>& SoyUniqueSownOperations)
{
	// Нахождение посеянной операции (sownOp)
	auto sownOpIt = std::find_if(SoyUniqueSownOperations.begin(), SoyUniqueSownOperations.end(),
		[&elem](const object_db& el)
		{
			return el.higher_tm == elem.higher_tm && el.pu == elem.pu;
		});

	object_db sownOp = (sownOpIt != SoyUniqueSownOperations.end()) ? *sownOpIt : object_db();
	std::optional<object_db> prevOp;
	std::optional<object_db> altOp;

	// Определение prevOp и altOp в зависимости от material_order
	if (elem.material_order == soyOperations[0])
	{
		prevOp = sownOp;
	}
	else if (elem.material_order == soyOperations[1])
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
void updateLastResultSoy(LastResult& lastResult, object_db& el, std::string& currentStatus)
{
	for (int i = 0; i < soyOperations->size(); i++)
	{
		if (el.material_order.has_value() && el.material_order.value() == soyOperations[i])
		{
			for (int j = 0; j < lastResult.soy.operations[soyOperations[i]].size(); j++)
			{
				if (lastResult.soy.operations[soyOperations[i]][j].businessDirection == el.business_dir && lastResult.soy.operations[soyOperations[i]][j].PU == el.pu)
				{
					lastResult.soy.operations[soyOperations[i]][j].statuses[currentStatus].fieldsCount++;
					lastResult.soy.operations[soyOperations[i]][j].statuses[currentStatus].fieldsSquare += el.field_square.value_or(0.0f);
					break;  // Выход из цикла, так как нужный элемент найден
				}
			}
		}
	}
}