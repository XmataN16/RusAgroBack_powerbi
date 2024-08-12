#pragma once
const std::string sugarBeetOperations[5] =
{
		u8"Внесение СЗР Почвенные гербициды (ГА)",
		u8"Внес. СЗР 1-я бетан. обр. бак.смесь (ГА)",
		u8"Внес. СЗР 2-я бетан. обр. бак.смесь (ГА)",
		u8"Внес. СЗР 3-я бетан. обр. бак.смесь (ГА)",
		u8"Внес. СЗР 4-я бетан. обр. бак.смесь (ГА)"
};

std::vector<object_db> fillInputAndAlternativeOperationsForSugarBeet(std::vector<object_db> dataSugarBeet)
{
	std::vector<object_db> resultDataSugarBeet;

	for (int row = 0; row < dataSugarBeet.size(); row++)
	{
		if (dataSugarBeet[row].material_order == sugarBeetOperations[0])
		{
			dataSugarBeet[row].input_operation = u8"Посев без внесения удобрений(ГА)";
			dataSugarBeet[row].alt_operation = u8"Посев с внесением удобрений (ГА)";
			dataSugarBeet[row].input_deadline = 3;
			dataSugarBeet[row].alt_deadline = 3;
			resultDataSugarBeet.push_back(dataSugarBeet[row]);
		}
		else if (dataSugarBeet[row].material_order == sugarBeetOperations[1])
		{
			dataSugarBeet[row].input_operation = u8"Внесение СЗР Почвенные гербициды (ГА)";
			dataSugarBeet[row].alt_operation = u8"Посев без внесения удобрений (ГА)";
			dataSugarBeet[row].input_deadline = 18;
			dataSugarBeet[row].alt_deadline = 12;
			resultDataSugarBeet.push_back(dataSugarBeet[row]);
		}
		else if (dataSugarBeet[row].material_order == sugarBeetOperations[2])
		{
			dataSugarBeet[row].input_operation = u8"Внес. СЗР 1-я бетан. обр. бак.смесь (ГА)";
			dataSugarBeet[row].input_deadline = 10;
			resultDataSugarBeet.push_back(dataSugarBeet[row]);
		}
		else if (dataSugarBeet[row].material_order == sugarBeetOperations[3])
		{
			dataSugarBeet[row].input_operation = u8"Внес. СЗР 2-я бетан. обр. бак.смесь (ГА)";
			dataSugarBeet[row].input_deadline = 10;
			resultDataSugarBeet.push_back(dataSugarBeet[row]);
		}
		else if (dataSugarBeet[row].material_order == sugarBeetOperations[4])
		{
			dataSugarBeet[row].input_operation = u8"Внес. СЗР 3-я бетан. обр. бак.смесь (ГА)";
			dataSugarBeet[row].input_deadline = 10;
			resultDataSugarBeet.push_back(dataSugarBeet[row]);
		}
	}
	return resultDataSugarBeet;
}

// Основная функция для упорядочивания операций с сахарной свеклой
std::vector<object_db> orderSugarBeetOperations(const std::vector<object_db>& currentDataSugarBeet)
{
	std::string firstOrder = sugarBeetOperations[0];
	std::string secondOrder = sugarBeetOperations[1];
	std::string thirdOrder = sugarBeetOperations[2];
	std::string fourthOrder = sugarBeetOperations[3];
	std::string fifthOrder = sugarBeetOperations[4];

	std::vector<object_db> result;

	// Фильтрация и добавление элементов в результирующий вектор
	std::vector<object_db> firstSlice = filter(currentDataSugarBeet, firstOrder);
	std::vector<object_db> secondSlice = filter(currentDataSugarBeet, secondOrder);
	std::vector<object_db> thirdSlice = filter(currentDataSugarBeet, thirdOrder);
	std::vector<object_db> fourthSlice = filter(currentDataSugarBeet, fourthOrder);
	std::vector<object_db> fifthSlice = filter(currentDataSugarBeet, fifthOrder);

	result.insert(result.end(), firstSlice.begin(), firstSlice.end());
	result.insert(result.end(), secondSlice.begin(), secondSlice.end());
	result.insert(result.end(), thirdSlice.begin(), thirdSlice.end());
	result.insert(result.end(), fourthSlice.begin(), fourthSlice.end());
	result.insert(result.end(), fifthSlice.begin(), fifthSlice.end());

	return result;
}

std::string defineSugarBeetElementStatus(object_db& elem, std::vector<object_db>& uniqSBElements, std::vector<object_db>& sugarBeetUniqueSownOperations)
{
	// Нахождение посеянной операции (sownOp)
	auto sownOpIt = std::find_if(sugarBeetUniqueSownOperations.begin(), sugarBeetUniqueSownOperations.end(),
		[&elem](const object_db& el) 
		{
			return el.higher_tm == elem.higher_tm && el.pu == elem.pu;
		});

	object_db sownOp = (sownOpIt != sugarBeetUniqueSownOperations.end()) ? *sownOpIt : object_db();
	std::optional<object_db> prevOp;
	std::optional<object_db> altOp;

	// Определение prevOp и altOp в зависимости от material_order
	if (elem.material_order == sugarBeetOperations[0])
	{
		prevOp = sownOp;
	}
	else if (elem.material_order == sugarBeetOperations[1])
	{
		auto prevOpIt = std::find_if(uniqSBElements.begin(), uniqSBElements.end(),
			[&elem](const object_db& el) 
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqSBElements.end()) ? *prevOpIt : std::optional<object_db>{};
		altOp = sownOp;
	}
	else 
	{
		auto prevOpIt = std::find_if(uniqSBElements.begin(), uniqSBElements.end(),
			[&elem](const object_db& el) 
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqSBElements.end()) ? *prevOpIt : std::optional<object_db>{};
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
void updateLastResultSugarBeet(LastResult& lastResult, object_db& el, std::string& currentStatus)
{
	for (int i = 0; i < sugarBeetOperations->size(); i++)
	{
		if (el.material_order.has_value() && el.material_order.value() == sugarBeetOperations[i])
		{
			for (int j = 0; j < lastResult.sugar_beet.operations[sugarBeetOperations[i]].size(); j++)
			{
				if (lastResult.sugar_beet.operations[sugarBeetOperations[i]][j].businessDirection == el.business_dir && lastResult.sugar_beet.operations[sugarBeetOperations[i]][j].PU == el.pu)
				{
					lastResult.sugar_beet.operations[sugarBeetOperations[i]][j].statuses[currentStatus].fieldsCount++;
					lastResult.sugar_beet.operations[sugarBeetOperations[i]][j].statuses[currentStatus].fieldsSquare += el.field_square.value_or(0.0f);
					break;  // Выход из цикла, так как нужный элемент найден
				}
			}
		}
	}
}