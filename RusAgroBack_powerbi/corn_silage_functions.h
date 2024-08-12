#pragma once
const std::string cornSilageOperations[2] =
{
		u8"Внесение СЗР Почвенные гербициды (ГА)",
		u8"Культивация (ГА)"
};

std::vector<object_db> fillInputAndAlternativeOperationsForCornSilage(std::vector<object_db> dataCornSilage)
{
	std::vector<object_db> resultDataCornSilage;

	for (int row = 0; row < dataCornSilage.size(); row++)
	{
		if (dataCornSilage[row].material_order == cornSilageOperations[0])
		{
			dataCornSilage[row].input_operation = u8"Посев без внесения удобрений (ГА)";
			dataCornSilage[row].alt_operation = u8"Посев с внесением удобрений (ГА)";
			dataCornSilage[row].input_deadline = 3;
			dataCornSilage[row].alt_deadline = 3;
			resultDataCornSilage.push_back(dataCornSilage[row]);
		}
		if (dataCornSilage[row].material_order == cornSilageOperations[1])
		{
			dataCornSilage[row].input_operation = u8"Культивация (ГА)";
			dataCornSilage[row].alt_operation = u8"Внесение СЗР Почвенные гербициды (ГА)";
			dataCornSilage[row].input_deadline = 30;
			dataCornSilage[row].alt_deadline = 25;
			resultDataCornSilage.push_back(dataCornSilage[row]);
		}
	}
	return resultDataCornSilage;
}

// Основная функция для упорядочивания операций по кукурузе на силос
std::vector<object_db> orderCornSilageOperations(const std::vector<object_db>& currentDataCornSilage)
{
	std::string firstOrder = cornSilageOperations[0];
	std::string secondOrder = cornSilageOperations[1];

	std::vector<object_db> result;

	// Фильтрация и добавление элементов в результирующий вектор
	std::vector<object_db> firstSlice = filter(currentDataCornSilage, firstOrder);
	std::vector<object_db> secondSlice = filter(currentDataCornSilage, secondOrder);

	result.insert(result.end(), firstSlice.begin(), firstSlice.end());
	result.insert(result.end(), secondSlice.begin(), secondSlice.end());


	return result;
}

//определение статусов по кукурузе на силос
std::string defineCornSilageElementStatus(object_db& elem, std::vector<object_db>& uniqCSElements, std::vector<object_db>& CornSilageUniqueSownOperations)
{
	// Нахождение посеянной операции (sownOp)
	auto sownOpIt = std::find_if(CornSilageUniqueSownOperations.begin(), CornSilageUniqueSownOperations.end(),
		[&elem](const object_db& el)
		{
			return el.higher_tm == elem.higher_tm && el.pu == elem.pu;
		});

	object_db sownOp = (sownOpIt != CornSilageUniqueSownOperations.end()) ? *sownOpIt : object_db();
	std::optional<object_db> prevOp;
	std::optional<object_db> altOp;

	// Определение prevOp и altOp в зависимости от material_order
	if (elem.material_order == cornSilageOperations[0])
	{
		prevOp = sownOp;
	}
	else if (elem.material_order == cornSilageOperations[1])
	{
		auto prevOpIt = std::find_if(uniqCSElements.begin(), uniqCSElements.end(),
			[&elem](const object_db& el)
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqCSElements.end()) ? *prevOpIt : std::optional<object_db>{};
		altOp = sownOp;
	}
	else
	{
		auto prevOpIt = std::find_if(uniqCSElements.begin(), uniqCSElements.end(),
			[&elem](const object_db& el)
			{
				return el.higher_tm == elem.higher_tm && el.material_order == elem.input_operation;
			});
		prevOp = (prevOpIt != uniqCSElements.end()) ? *prevOpIt : std::optional<object_db>{};
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
void updateLastResultCornSilage(LastResult& lastResult, object_db& el, std::string& currentStatus)
{
	for (int i = 0; i < cornSilageOperations->size(); i++)
	{
		if (el.material_order.has_value() && el.material_order.value() == cornSilageOperations[i])
		{
			for (int j = 0; j < lastResult.corn_silage.operations[cornSilageOperations[i]].size(); j++)
			{
				if (lastResult.corn_silage.operations[cornSilageOperations[i]][j].businessDirection == el.business_dir && lastResult.corn_silage.operations[cornSilageOperations[i]][j].PU == el.pu)
				{
					lastResult.corn_silage.operations[cornSilageOperations[i]][j].statuses[currentStatus].fieldsCount++;
					lastResult.corn_silage.operations[cornSilageOperations[i]][j].statuses[currentStatus].fieldsSquare += el.field_square.value_or(0.0f);
					break;  // Выход из цикла, так как нужный элемент найден
				}
			}
		}
	}
}